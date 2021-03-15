#pragma once
#include "descriptions.h"

#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>


class Aligner {
private:
  using StopSet = std::unordered_set<std::string_view>;
  using StopIdx = std::unordered_map<std::string_view, int>;
  using StopCoords = std::unordered_map<std::string_view, Sphere::Point>;
  using StopAxes = std::vector<std::pair<double, std::string_view>>;
public:
  Aligner(const Descriptions::StopsDict& stops,
    const Descriptions::BusesDict& buses,
    double max_width, double max_height, double padding);

  Svg::Point operator()(const std::string& stop_name) const;
private:
  StopSet ControlStops(const Descriptions::BusesDict& buses) const;
  StopCoords ComputeControlBasedCoords(
    const Descriptions::StopsDict& stops,
    const Descriptions::BusesDict& buses) const;
  int DistributeIdx(const StopAxes& coords, StopIdx& stops_to_idx);
private:
  const double width = 0;
  const double height = 0;
  const double padding = 0;
  double x_step, y_step;

  StopIdx stops_to_xidx;
  StopIdx stops_to_yidx;

  const Descriptions::SetMap neighs;
  const Descriptions::StopsDict& stops_dict;
  const Descriptions::BusesDict buses_dict;
};
