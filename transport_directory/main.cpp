#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "sphere.h"
#include "transport_catalog.h"
#include "utils.h"
#include "test_all.h"

#include <iostream>

using namespace std;

int main() {
  ifstream json("example2.json");
  const auto input_doc = Json::Load(json);
  const auto& input_map = input_doc.GetRoot().AsMap();

  const TransportCatalog db(
    Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
    input_map.at("routing_settings").AsMap(),
    input_map.at("render_settings").AsMap()
  );

  Json::PrintValue(
    Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
    cout
  );
  cout << endl;

  return 0;
}
