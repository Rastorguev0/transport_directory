#include "rnd_big_data.h"
#include "profile.h"
#include "transport_catalog.h"
#include "requests.h"

void SpeedTest() {
  LOG_DURATION("Total");
  const auto input_map = [] {
    LOG_DURATION("Generatin data");
    return Generator(100, 100, 100, 2000).GenerateBigData();
  }();
  const TransportCatalog db(
    Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
    input_map.at("routing_settings").AsMap(),
    input_map.at("render_settings").AsMap()
  );

  LOG_DURATION("Processing requests");
  Json::PrintValue(
    Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
    cout
  );
  cout << endl;
}

void TestAll() {
	SpeedTest();
}