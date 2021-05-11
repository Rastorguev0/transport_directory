#include "aligner.h"

#include <algorithm>

using namespace std;

Aligner::Aligner(const Descriptions::StopsDict& stops,
  const Descriptions::BusesDict& buses,
  const vector<YellowPages::Company>& companies,
  double max_width, double max_height, double padding)

  : width(max_width), height(max_height), padding(padding),
  stops_dict(stops), buses_dict(buses),
  neighs(Descriptions::DefineNeighbors(stops, buses)) {

  const StopCoords coords = ComputeControlBasedCoords(stops, buses);
  StopAxes longs, lats;
  longs.reserve(stops_dict.size() + companies.size());
  lats.reserve(stops_dict.size() + companies.size());
  for (const auto& [name, point] : coords) {
    longs.emplace_back(point.longitude, name);
    lats.emplace_back(point.latitude, name);
  }
  for (const auto& company : companies) {
    string name = CompanyMainName(company);
    neighs[name];
    for (const auto& neigh : company.nearby_stops()) {
      neighs[name].insert(neigh.name());
      neighs[neigh.name()].insert(name);
    }
    longs.emplace_back(company.address().coords().lon(), name);
    lats.emplace_back(company.address().coords().lat(), name);
  }
  std::sort(begin(longs), end(longs));
  std::sort(begin(lats), end(lats));

  const int lon_count = DistributeIdx(longs, stops_to_xidx);
  const int lat_count = DistributeIdx(lats, stops_to_yidx);
  x_step = lon_count > 0 ? (width - 2 * padding) / lon_count : 0;
  y_step = lat_count > 0 ? (height - 2 * padding) / lat_count : 0;
}

Aligner::StopSet Aligner::ControlStops(const Descriptions::BusesDict& buses) const {
  StopSet controls;
  unordered_map<string, int> stops_count;
  vector<unordered_set<string>> round_stops;

  for (const auto& [_, bus] : buses) {
    for (const auto& end : bus->endpoints) {
      controls.insert(end);
    }
    if (bus->is_roundtrip) round_stops.emplace_back();
    for (const auto& stop : bus->stops) {
      stops_count[stop]++;
      if (bus->is_roundtrip) round_stops.back().insert(stop);
    }
  }

  for (const auto& [name, count] : stops_count) {
    if (count > 2) controls.insert(name);
  }
  unordered_map<string, int> transfer_map;
  for (const auto& route : round_stops) {
    for (auto stop : route) {
      transfer_map[stop]++;
    }
  }
  for (const auto& [name, count] : transfer_map) {
    if (count > 1) controls.insert(name);
  }

  return controls;
}

Aligner::StopCoords Aligner::ComputeControlBasedCoords(
  const Descriptions::StopsDict& stops_dict,
  const Descriptions::BusesDict& buses_dict) const {

  const StopSet controls = ControlStops(buses_dict);
  StopCoords result;
  auto next_control = [&controls](const string& stop) {
    return controls.count(stop);
  };

  for (const auto& [_, bus] : buses_dict) {
    const auto& stops = bus->stops;
    if (stops.size() < 2) continue;
    auto ctrl_it1 = stops.begin();
    for (auto ctrl_it2 = find_if(ctrl_it1 + 1, end(stops), next_control);
         ctrl_it2 < end(stops);
         ctrl_it2 = find_if(ctrl_it1 + 1, end(stops), next_control)) {

      double lon_step = (stops_dict.at(*ctrl_it2)->position.longitude
        - stops_dict.at(*ctrl_it1)->position.longitude) / (ctrl_it2 - ctrl_it1);
      double lat_step = (stops_dict.at(*ctrl_it2)->position.latitude
        - stops_dict.at(*ctrl_it1)->position.latitude) / (ctrl_it2 - ctrl_it1);

      for (auto it = ctrl_it1; it != ctrl_it2 + 1; it++) {
        result[*it] = {
          .latitude = stops_dict.at(*ctrl_it1)->position.latitude + lat_step * (it - ctrl_it1),
          .longitude = stops_dict.at(*ctrl_it1)->position.longitude + lon_step * (it - ctrl_it1),
        };
      }
      ctrl_it1 = ctrl_it2;
    }
  }

  for (const auto& [name, stop] : stops_dict) {
    if (!result.count(name)) {
      result[name] = stop->position;
    }
  }
  return result;
}

int Aligner::DistributeIdx(const StopAxes& axes, StopIdx& stops_to_idx) {
  int max_idx = 0;
  for (auto it = axes.begin(); it != axes.end(); it++) {
    int idx = -1;
    for (const auto& n : neighs.at(string(it->second))) {
      if (stops_to_idx.count(n)) {
        idx = max(stops_to_idx.at(n), idx);
      }
    }
    stops_to_idx[it->second] = ++idx;
    max_idx = max(idx, max_idx);
  }
  return max_idx;
}

Svg::Point Aligner::operator()(const string& place_name) const {
  return {
    stops_to_xidx.at(place_name) * x_step + padding,
    height - padding - stops_to_yidx.at(place_name) * y_step
  };
}