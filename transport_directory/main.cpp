#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "transport_catalog.h"

#include <iostream>
#include <fstream>
#include <string_view>

using namespace std;

string ReadFileData(const string& file_name) {
  ifstream file(file_name, ios::binary | ios::ate);
  const ifstream::pos_type end_pos = file.tellg();
  file.seekg(0, ios::beg);

  string data(end_pos, '\0');
  file.read(&data[0], end_pos);
  return data;
}


int main(int argc, const char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return 5;
  }

  const string_view mode(argv[1]);
  //ifstream in("example2_process_requests.json");
  const auto input_doc = Json::Load(cin);
  const auto& input_map = input_doc.GetRoot().AsMap();
  const string file = input_map.at("serialization_settings").AsMap().at("file").AsString();

  if (mode == "make_base") {
    TransportCatalog db(
      Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
      input_map.at("routing_settings").AsMap(),
      input_map.at("render_settings").AsMap()
    );
    ofstream out(file, ios::binary);
    db.Serialize(out);
  }
  else if (mode == "process_requests") {
    istringstream in(ReadFileData(file));
    TransportCatalog db(in);
    Json::PrintValue(
      Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
      cout
    );
    cout << endl;
  }

  return 0;
}
