#include "transport_router.h"

using namespace std;

Serialize::TransportRouter TransportRouter::Serialize() const {
  Serialize::TransportRouter result;

  Serialize::RoutingSettings settings;
  settings.set_bus_wait_time(routing_settings_.bus_wait_time);
  settings.set_bus_velocity(routing_settings_.bus_velocity);

  *(result.mutable_settings_()) = settings;
  *(result.mutable_graph_()) = graph_.Serialize();
  *(result.mutable_router_()) = router_->Serialize();
  for (const auto& [id, svi] : stops_vertex_ids_) {
    Serialize::StopVertexIdx s_svi;
    s_svi.set_name(id);
    s_svi.set_in(svi.in);
    s_svi.set_out(svi.out);
    *(result.add_stops_vertex_ids_()) = s_svi;
  }
  for (const auto& info : vertices_info_) {
    Serialize::VertexInfo s_info;
    s_info.set_stop_name(info.stop_name);
    *(result.add_vertices_info_()) = s_info;
  }
  for (const auto& var: edges_info_) {
    Serialize::EdgeInfo s_info;
    if (holds_alternative<BusEdgeInfo>(var)) {
      auto info = get<BusEdgeInfo>(var);
      Serialize::BusEdgeInfo bei;
      bei.set_bus_name(info.bus_name);
      bei.set_start_stop_idx(info.start_stop_idx);
      bei.set_finish_stop_idx(info.finish_stop_idx);
      bei.set_span_count(info.span_count);
      *(s_info.mutable_bus()) = bei;
    }
    else {
      *(s_info.mutable_wait()) = Serialize::WaitEdgeInfo();
    }
    *(result.add_edges_info_()) = s_info;
  }
  return result;
}

TransportRouter::TransportRouter(const Serialize::TransportRouter& router) {
  routing_settings_ = RoutingSettings{
    router.settings_().bus_wait_time(), router.settings_().bus_velocity()
  };
  graph_ = BusGraph(router.graph_());
  router_ = make_unique<Router>(router.router_(), graph_);
  for (const auto& item : router.stops_vertex_ids_()) {
    stops_vertex_ids_[item.name()] = StopVertexIds{ item.in(), item.out() };
  }
  vertices_info_.reserve(router.vertices_info__size());
  for (const auto& item : router.vertices_info_()) {
    vertices_info_.push_back({ item.stop_name() });
  }
  edges_info_.reserve(router.edges_info__size());
  for (const auto& item : router.edges_info_()) {
    if (item.has_bus()) {
      edges_info_.push_back(BusEdgeInfo{ item.bus().bus_name(), item.bus().start_stop_idx(),
        item.bus().finish_stop_idx(), item.bus().span_count() });
    }
    else {
      edges_info_.push_back(WaitEdgeInfo{});
    }
  }
}