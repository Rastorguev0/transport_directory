#pragma once
#include "descriptions.h"

#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>


class Aligner {
public:
  Aligner(const Descriptions::StopsDict& stops,
    double max_width, double max_height, double padding);

  Svg::Point operator()(const std::string& stop_name) const;
private:
  using StopSet = std::unordered_set<std::string_view>;
  struct Idx {
    int x;
    int y;
  };
  double width = 0;
  double height = 0;
  double padding = 0;
  double x_step, y_step;
  const Descriptions::StopsDict& stops_dict;
  std::unordered_map<std::string_view, Idx> stops_to_idx;
};
