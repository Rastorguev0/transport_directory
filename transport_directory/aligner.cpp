#include "aligner.h"

#include <algorithm>

using namespace std;

Aligner::Aligner(const Descriptions::StopsDict& stops,
  const Descriptions::BusesDict& buses,
  double max_width, double max_height, double padding)
  : width(max_width), height(max_height), padding(padding),
  stops_dict(stops), buses_dict(buses) {

  StopAxes longs, lats;
  longs.reserve(stops_dict.size());
  lats.reserve(stops_dict.size());
  for (const auto& [name, stop] : stops_dict) {
    longs.emplace_back(stop->position.longitude, name);
    lats.emplace_back(stop->position.latitude, name);
  }
  std::sort(begin(longs), end(longs));
  std::sort(begin(lats), end(lats));
  const int lon_count = DistributeIdx(longs, stops_to_xidx);
  const int lat_count = DistributeIdx(lats, stops_to_yidx);

  x_step = lon_count > 1 ? (width - 2 * padding) / (lon_count - 1) : 0;
  y_step = lat_count > 1 ? (height - 2 * padding) / (lat_count - 1) : 0;
}

int Aligner::DistributeIdx(const StopAxes& axes, StopIdx& stops_to_idx) {
  int idx = 0;
  StopSet cur_stops_line;

  for (auto it = axes.begin(); it != axes.end(); it++) {
    if (AreNeighbours(cur_stops_line, { it->second }) ||
      AreNeighbours({ it->second }, cur_stops_line)) {
      cur_stops_line.clear();
      stops_to_idx[it->second] = ++idx;
    }
    else {
      stops_to_idx[it->second] = idx;
    }
    cur_stops_line.insert(it->second);
  }
  return idx + 1;
}

bool Aligner::AreNeighbours(const StopSet& stops1, const StopSet& stops2) const {
  bool neighbors = false;
  for (auto stop1 : stops1) {
    if (neighbors) break;
    for (auto stop2 : stops2) {
      if (stops_dict.at(string(stop1))->distances.count(string(stop2))) {
        neighbors = ConfirmStopsAreNeighbours(buses_dict, string(stop1), string(stop2));
        if (neighbors) break;
      }
    }
  }
  return neighbors;
}

bool ConfirmStopsAreNeighbours(const Descriptions::BusesDict& buses,
  const std::string& stop1, const std::string& stop2) {
  bool neighbors = false;
  for (const auto& [name, bus] : buses) {
    if (neighbors) break;
    auto stop_it = find(begin(bus->stops), end(bus->stops), stop1);
    for ( ;
      stop_it != end(bus->stops);
      stop_it = find(stop_it + 1, end(bus->stops), stop1)) {
      if (stop_it > begin(bus->stops)) neighbors |= *(stop_it - 1) == stop2;
      if (stop_it < end(bus->stops) - 1) neighbors |= *(stop_it + 1) == stop2;
      if (neighbors) break;
    }
  }
  return neighbors;
}

Svg::Point Aligner::operator()(const string& stop_name) const {
  return {
    stops_to_xidx.at(stop_name) * x_step + padding,
    height - padding - stops_to_yidx.at(stop_name) * y_step
  };
}