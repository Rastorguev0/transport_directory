#include "sphere.h"

#include <unordered_set>

using namespace std;

namespace Sphere {
  const double PI = 3.1415926535;

  double ConvertDegreesToRadians(double degrees) {
    return degrees * PI / 180.0;
  }

  Point Point::FromDegrees(double latitude, double longitude) {
    return {
      ConvertDegreesToRadians(latitude),
      ConvertDegreesToRadians(longitude)
    };
  }

  bool ByLatitude(Point lhs, Point rhs) {
    return lhs.latitude < rhs.latitude;
  }

  bool ByLongitude(Point lhs, Point rhs) {
    return lhs.longitude < rhs.longitude;
  }

  const double EARTH_RADIUS = 6'371'000;

  double Distance(Point lhs, Point rhs) {
    lhs = Point::FromDegrees(lhs.latitude, lhs.longitude);
    rhs = Point::FromDegrees(rhs.latitude, rhs.longitude);
    return acos(
      sin(lhs.latitude) * sin(rhs.latitude)
      + cos(lhs.latitude) * cos(rhs.latitude) * cos(abs(lhs.longitude - rhs.longitude))
    ) * EARTH_RADIUS;
  }


  Svg::Point Projector::operator()(Point point) const {
    return {
        (point.longitude - min_lon_) * zoom_rate_ + padding_,
        (max_lat_ - point.latitude) * zoom_rate_ + padding_,
    };
  }

  Aligner::Aligner(double max_width, double max_height, double padding)
  : width(max_width), height(max_height), padding(padding) {}

  map<string, Svg::Point> Aligner::Align(
    vector<pair<Sphere::Point, string>>& points) const {

    unordered_set<double> lats, longs;
    for (const auto& [p, _] : points) {
      lats.insert(p.latitude);
      longs.insert(p.longitude);
    }

    double x_step = longs.size() > 1 ? (width - 2 * padding) / (longs.size() - 1)
                                     : 0;
    double y_step = lats.size() > 1 ? (height - 2 * padding) / (lats.size() - 1)
                                    : 0;

    sort(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) {
      return ByLongitude(lhs.first, rhs.first);
      });

    map<string, Svg::Point> result;
    for (size_t i = 0; i < points.size(); ++i) {
      result[points[i].second] = {
        .x = i * x_step + padding, .y = 0
      };
    }

    sort(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) {
      return ByLatitude(lhs.first, rhs.first);
      });

    for (size_t i = 0; i < points.size(); ++i) {
      result[points[i].second].y = height - padding - i * y_step;
    }
    assert(result.size() == points.size());
    return result;

  }
}
