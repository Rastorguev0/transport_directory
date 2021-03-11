#pragma once
#include "svg.h"

#include <cmath>
#include <algorithm>

namespace Sphere {
  double ConvertDegreesToRadians(double degrees);

  struct Point {
    double latitude;
    double longitude;

    static Point FromDegrees(double latitude, double longitude);
  };

  bool ByLatitude(Point lhs, Point rhs);
  bool ByLongitude(Point lhs, Point rhs);

  double Distance(Point lhs, Point rhs);

  class Projector {
  public:
    template <typename PointInputIt>
    Projector(PointInputIt points_begin, PointInputIt points_end,
      double max_width, double max_height, double padding);

    Svg::Point operator()(Point point) const;

  private:
    double min_lon_ = 0;
    double max_lat_ = 0;
    const double padding_;
    double zoom_rate_ = 0;
  };


  template <typename PointInputIt>
  Projector::Projector(PointInputIt points_begin, PointInputIt points_end,
    double max_width, double max_height, double padding)
    : padding_(padding)
  {
    if (points_begin == points_end) {
      return;
    }

    const auto [left_it, right_it] =
      std::minmax_element(points_begin, points_end, ByLongitude);
    min_lon_ = left_it->longitude;
    const double max_lon = right_it->longitude;

    const auto [bottom_it, top_it] =
      std::minmax_element(points_begin, points_end, ByLatitude);
    const double min_lat = bottom_it->latitude;
    max_lat_ = top_it->latitude;

    double x_rate = max_lon - min_lon_ == 0 ? 0
      : (max_width - 2 * padding) / (max_lon - min_lon_);
    double y_rate = max_lat_ - min_lat == 0 ? 0
      : (max_height - 2 * padding) / (max_lat_ - min_lat);
    zoom_rate_ = std::min(x_rate, y_rate);
    if (zoom_rate_ == 0) zoom_rate_ = std::max(x_rate, y_rate);
  }

}
