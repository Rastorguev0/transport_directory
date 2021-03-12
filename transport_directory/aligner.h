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
  using StopAxes = std::vector<std::pair<double, std::string_view>>;
public:
  Aligner(const Descriptions::StopsDict& stops,
    const Descriptions::BusesDict& buses,
    double max_width, double max_height, double padding);

  Svg::Point operator()(const std::string& stop_name) const;
private:
  int DistributeIdx(const StopAxes& coords, StopIdx& stops_to_idx);
  bool AreNeighbours(const StopSet& stops1, const StopSet& stops2) const;
private:
  double width = 0;
  double height = 0;
  double padding = 0;
  double x_step, y_step;

  StopIdx stops_to_xidx;
  StopIdx stops_to_yidx;
  const Descriptions::StopsDict& stops_dict;
  const Descriptions::BusesDict buses_dict;
};

//very SLOW
bool ConfirmStopsAreNeighbours(const Descriptions::BusesDict& buses,
  const std::string& stop1, const std::string& stop2);
