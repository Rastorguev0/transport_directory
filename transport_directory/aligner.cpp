#include "aligner.h"

#include <algorithm>

using namespace std;

Aligner::Aligner(const Descriptions::StopsDict& stops,
  double max_width, double max_height, double padding)
  : width(max_width), height(max_height), padding(padding), stops_dict(stops) {

  map<double, StopSet> longs, lats;
  for (const auto& [name, stop] : stops_dict) {
    longs[stop->position.longitude].insert(name);
    lats[stop->position.latitude].insert(name);
  }

  x_step = longs.size() > 1 ? (width - 2 * padding) / (longs.size() - 1) : 0;
  y_step = lats.size() > 1 ? (height - 2 * padding) / (lats.size() - 1) : 0;

  int idx = 0;
  for (const auto& [lng, stops] : longs) {
    for (auto stop : stops) {
      stops_to_idx[stop].x = idx++;
    }
  }
  idx = 0;
  for (const auto& [lat, stops] : lats) {
    for (auto stop : stops) {
      stops_to_idx[stop].y = idx++;
    }
  }
}

Svg::Point Aligner::operator()(const string& stop_name) const {
  return {
    stops_to_idx.at(stop_name).x * x_step + padding,
    height - padding - stops_to_idx.at(stop_name).y * y_step
  };
}