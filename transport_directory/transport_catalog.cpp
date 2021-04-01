#include "transport_catalog.h"
#include <memory>

using namespace std;

TransportCatalog::TransportCatalog(vector<Descriptions::InputQuery> data,
  const Json::Dict& routing_settings_json,
  const Json::Dict& render_settings_json) {
  auto stops_end = partition(begin(data), end(data), [](const auto& item) {
    return holds_alternative<Descriptions::Stop>(item);
    });

  auto stops_dict = make_shared<Descriptions::StopsDict>();
  for (const auto& item : Range{ begin(data), stops_end }) {
    const auto& stop = get<Descriptions::Stop>(item);
    (*stops_dict)[stop.name] = make_unique<Descriptions::Stop>(stop);
    stops_.insert({ stop.name, {} });
  }

  auto buses_dict = make_shared<Descriptions::BusesDict>();
  for (const auto& item : Range{ stops_end, end(data) }) {
    const auto& bus = get<Descriptions::Bus>(item);

    (*buses_dict)[bus.name] = make_unique<Descriptions::Bus>(bus);
    buses_[bus.name] = Bus{
      bus.stops.size(),
      ComputeUniqueItemsCount(AsRange(bus.stops)),
      ComputeRoadRouteLength(bus.stops, *stops_dict),
      ComputeGeoRouteDistance(bus.stops, *stops_dict)
    };

    for (const string& stop_name : bus.stops) {
      stops_.at(stop_name).bus_names.insert(bus.name);
    }
  }

  router_ = make_unique<TransportRouter>(*stops_dict, *buses_dict, routing_settings_json);
  painter_ = make_unique<Paint::Painter>(render_settings_json, buses_dict, stops_dict);
}


const TransportCatalog::Stop* TransportCatalog::GetStop(const string& name) const {
  return GetValuePointer(stops_, name);
}

const TransportCatalog::Bus* TransportCatalog::GetBus(const string& name) const {
  return GetValuePointer(buses_, name);
}

optional<TransportRouter::RouteInfo> TransportCatalog::FindRoute(const string& stop_from, const string& stop_to) const {
  return router_->FindRoute(stop_from, stop_to);
}

int TransportCatalog::ComputeRoadRouteLength(
  const vector<string>& stops,
  const Descriptions::StopsDict& stops_dict
) {
  int result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
  }
  return result;
}

double TransportCatalog::ComputeGeoRouteDistance(
  const vector<string>& stops,
  const Descriptions::StopsDict& stops_dict
) {
  double result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Sphere::Distance(
      stops_dict.at(stops[i - 1])->position, stops_dict.at(stops[i])->position
    );
  }
  return result;
}

std::string TransportCatalog::RenderMap() const {
  return painter_->Paint();
}

std::string TransportCatalog::RenderRoute(const Paint::RouteChain& links) const {
  return painter_->PaintRoute(links);
}