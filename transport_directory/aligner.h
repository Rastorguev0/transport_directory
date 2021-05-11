#pragma once
#include "descriptions.h"
#include "company.pb.h"
#include "companies_catalog.h"

#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>


class Aligner {
private:
  using StopSet = std::unordered_set<std::string>;
  using StopIdx = std::unordered_map<std::string, int>;
  using StopCoords = std::unordered_map<std::string, Sphere::Point>;
  using StopAxes = std::vector<std::pair<double, std::string>>;
public:
  Aligner(const Descriptions::StopsDict& stops,
    const Descriptions::BusesDict& buses,
    const std::vector<YellowPages::Company>& companies,
    double max_width, double max_height, double padding);

  Svg::Point operator()(const std::string& place_name) const;
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

  Descriptions::SetMap neighs;
  const Descriptions::StopsDict& stops_dict;
  const Descriptions::BusesDict& buses_dict;
};
