#include "transport_catalog.h"

using namespace std;

TransportCatalog::TransportCatalog(istream& is) {
  Serialize::TransportCatalog tc;
  tc.ParseFromIstream(&is);
  for (const auto& stop_r : tc.stops_()) {
    Responses::Stop result;
    for (const auto& bus : stop_r.buses()) {
      result.bus_names.insert(bus);
    }
    stops_[stop_r.stop()] = result;
  }
  for (const auto& bus_r : tc.buses_()) {
    buses_[bus_r.bus()] = {
      bus_r.stop_count(),
      bus_r.unique_stop_count(),
      static_cast<int>(bus_r.road_length()),
      bus_r.geo_road_length()
    };;
  }
  router_ = make_unique<TransportRouter>(tc.router_());
}

void TransportCatalog::Serialize(ostream& os) const {
  Serialize::TransportCatalog tc;
  for (const auto& [name, stop] : stops_) {
    Serialize::StopRequest result;
    result.set_stop(name);
    for (const auto& bus : stop.bus_names) {
      result.add_buses(bus);
    }
    *(tc.add_stops_()) = result;
  }
  for (const auto& [name, bus] : buses_) {
    Serialize::BusRequest result;
    result.set_bus(name);
    result.set_stop_count(bus.stop_count);
    result.set_unique_stop_count(bus.unique_stop_count);
    result.set_road_length(bus.road_route_length);
    result.set_geo_road_length(bus.geo_route_length);
    *(tc.add_buses_()) = result;
  }
  *(tc.mutable_router_()) = router_->Serialize();
  tc.SerializeToOstream(&os);
}