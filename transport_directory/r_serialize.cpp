#include "router.h"

using namespace std;

template<>
Serialize::Router Graph::Router<double>::Serialize() const {
  Serialize::Router result;
  for (const auto& routes : routes_internal_data_) {
    Serialize::RoutesInternalData s_route;
    for (const auto& r : routes) {
      Serialize::RouteInternalData s_r;
      s_r.set_has(false);
      if (r) {
        s_r.set_has(true);
        Serialize::RouteData s_rd;
        s_rd.set_weight(r.value().weight);
        s_rd.set_has(false);
        if (r.value().prev_edge) {
          s_rd.set_has(true);
          s_rd.set_prev_edge(r.value().prev_edge.value());
        }
        *(s_r.mutable_data()) = s_rd;
      }
      *(s_route.add_data()) = s_r;
    }
    *(result.add_routes_internal_data_()) = s_route;
  }
  return result;
}

template<>
Graph::Router<double>::Router(const Serialize::Router& router, const Graph& graph) : graph_(graph) {
  routes_internal_data_.reserve(router.routes_internal_data__size());
  for (const auto& route_internal_data : router.routes_internal_data_()) {
    vector<optional<RouteInternalData>> dest;
    dest.reserve(route_internal_data.data_size());
    for (const auto& data : route_internal_data.data()) {
      if (data.has()) {
        RouteInternalData dest_data;
        dest_data.weight = data.data().weight();
        if (data.data().has())
          dest_data.prev_edge = data.data().prev_edge();
        dest.push_back(dest_data);
      }
      else {
        dest.push_back(nullopt);
      }
    }
    routes_internal_data_.push_back(dest);
  }
}