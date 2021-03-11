#include "sphere.h"

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

}
