#include "transport_catalog.h"

using namespace std;

TransportCatalog::TransportCatalog(vector<Descriptions::InputQuery> data,
                                   const Json::Dict& routing_settings_json,
                                   const Json::Dict& render_settings_json) {
  auto stops_end = partition(begin(data), end(data), [](const auto& item) {
    return holds_alternative<Descriptions::Stop>(item);
  });

  Descriptions::StopsDict stops_dict;
  for (const auto& item : Range{begin(data), stops_end}) {
    const auto& stop = get<Descriptions::Stop>(item);
    stops_dict[stop.name] = &stop;
    stops_.insert({stop.name, {}});
  }

  Descriptions::BusesDict buses_dict;
  for (const auto& item : Range{stops_end, end(data)}) {
    const auto& bus = get<Descriptions::Bus>(item);

    buses_dict[bus.name] = &bus;
    buses_[bus.name] = Bus{
      bus.stops.size(),
      ComputeUniqueItemsCount(AsRange(bus.stops)),
      ComputeRoadRouteLength(bus.stops, stops_dict),
      ComputeGeoRouteDistance(bus.stops, stops_dict)
    };

    for (const string& stop_name : bus.stops) {
      stops_.at(stop_name).bus_names.insert(bus.name);
    }
  }

  router_ = make_unique<TransportRouter>(stops_dict, buses_dict, routing_settings_json);

  painter_ = make_unique<Painter>(render_settings_json, ComputeMapBorders(stops_dict));
  SketchRoutes(buses_dict, stops_dict);
  SketchBusNames(buses_dict, stops_dict);
  SketchStops(stops_dict);
  SketchStopNames(stops_dict);
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

Borders TransportCatalog::ComputeMapBorders(const Descriptions::StopsDict& stops_dict) {
  Borders result{
    stops_dict.begin()->second->position.latitude,
    stops_dict.begin()->second->position.longitude,
    stops_dict.begin()->second->position.latitude,
    stops_dict.begin()->second->position.longitude,
  };
  for (const auto& [_, stop] : stops_dict) {
    if (stop->position.latitude < result.min_lat) {
      result.min_lat = stop->position.latitude;
    }
    if (stop->position.longitude < result.min_lon) {
      result.min_lon = stop->position.longitude;
    }
    if (stop->position.latitude > result.max_lat) {
      result.max_lat = stop->position.latitude;
    }
    if (stop->position.longitude > result.max_lon) {
      result.max_lon = stop->position.longitude;
    }
  }
  return result;
}

void TransportCatalog::SketchRoutes(
  const Descriptions::BusesDict& buses,
  const Descriptions::StopsDict& stops
) const {
  for (const auto& [_, bus] : buses) {
    vector<Sphere::Point> points;
    points.reserve(bus->stops.size());
    for (const string& stop : bus->stops) {
      points.push_back(stops.at(stop)->position);
    }
    painter_->PaintPolyline(points);
  }
}

void TransportCatalog::SketchBusNames(
  const Descriptions::BusesDict& buses,
  const Descriptions::StopsDict& stops
) const {
  for (const auto& [_, bus] : buses) {
    Sphere::Point stop_location = stops.at(bus->stops.at(0))->position;
    painter_->PaintBusName(stop_location, bus->name);
    if (!bus->is_roundtrip) {
      size_t index = bus->stops.size() / 2;
      if (bus->stops.at(0) != bus->stops.at(index)) {
        stop_location = stops.at(bus->stops.at(index))->position;
        painter_->PaintBusName(stop_location, bus->name, true);
      }
    }
  }
}

void TransportCatalog::SketchStops(const Descriptions::StopsDict& stops) const {
  for (const auto& [_, stop] : stops) {
    painter_->PaintCircle(stop->position);
  }
}

void TransportCatalog::SketchStopNames(const Descriptions::StopsDict& stops) const {
  for (const auto& [_, stop] : stops) {
    painter_->PaintStopName(stop->position, stop->name);
  }
}

std::string TransportCatalog::RenderMap() const {
  return painter_->Paint();
}