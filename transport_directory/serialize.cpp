#include "descriptions.pb.h"
#include "graph.pb.h"
#include "painter.pb.h"
#include "svg.pb.h"
#include "transport_catalog.pb.h"
#include "transport_router.pb.h"
#include "address.pb.h"
#include "company.pb.h"
#include "database.pb.h"
#include "name.pb.h"
#include "phone.pb.h"
#include "rubric.pb.h"
#include "sphere.pb.h"
#include "url.pb.h"
#include "working_time.pb.h"
#include "graph.h"
#include "svg.h"
#include "painter.h"
#include "descriptions.h"
#include "transport_router.h"
#include "transport_catalog.h"
#include "companies_catalog.h"

using namespace std;


/* GRAPH SERIALIZATION */

template<>
GraphProto::DirectedWeightedGraph Graph::DirectedWeightedGraph<double>::Serialize() const {
  GraphProto::DirectedWeightedGraph result;
  for (const auto& edge : edges_) {
    auto& edge_proto = *result.add_edges();
    edge_proto.set_from(edge.from);
    edge_proto.set_to(edge.to);
    edge_proto.set_weight(edge.weight);
  }

  for (const auto& incidence_list : incidence_lists_) {
    auto& incidence_list_proto = *result.add_incidence_lists();
    for (const auto edge_id : incidence_list) {
      incidence_list_proto.add_edge_ids(edge_id);
    }
  }
  return result;
}

template<>
Graph::DirectedWeightedGraph<double>::DirectedWeightedGraph(const GraphProto::DirectedWeightedGraph& proto) {
  edges_.reserve(proto.edges_size());
  for (const auto& edge_proto : proto.edges()) {
    auto& edge = edges_.emplace_back();
    edge.from = edge_proto.from();
    edge.to = edge_proto.to();
    edge.weight = edge_proto.weight();
  }

  incidence_lists_.reserve(proto.incidence_lists_size());
  for (const auto& incidence_list_proto : proto.incidence_lists()) {
    auto& incidence_list = incidence_lists_.emplace_back();
    incidence_list.reserve(incidence_list_proto.edge_ids_size());
    for (const auto edge_id : incidence_list_proto.edge_ids()) {
      incidence_list.push_back(edge_id);
    }
  }
}


/* ROUTER SERIALIZATION */

template<>
GraphProto::Router Graph::Router<double>::Serialize() const {
  GraphProto::Router proto;
  for (const auto& source_data : routes_internal_data_) {
    auto& source_data_proto = *proto.add_sources_data();
    for (const auto& route_data : source_data) {
      auto& route_data_proto = *source_data_proto.add_targets_data();
      if (route_data) {
        route_data_proto.set_exists(true);
        route_data_proto.set_weight(route_data->weight);
        if (route_data->prev_edge) {
          route_data_proto.set_has_prev_edge(true);
          route_data_proto.set_prev_edge(*route_data->prev_edge);
        }
      }
    }
  }
  return proto;
}

template<>
Graph::Router<double>::Router(const GraphProto::Router& proto, const Graph& graph) : graph_(graph) {
  routes_internal_data_.reserve(proto.sources_data_size());
  for (const auto& source_data_proto : proto.sources_data()) {
    auto& source_data = routes_internal_data_.emplace_back();
    source_data.reserve(source_data_proto.targets_data_size());
    for (const auto& route_data_proto : source_data_proto.targets_data()) {
      auto& route_data = source_data.emplace_back();
      if (route_data_proto.exists()) {
        route_data = RouteInternalData{ route_data_proto.weight(), std::nullopt };
        if (route_data_proto.has_prev_edge()) {
          route_data->prev_edge = route_data_proto.prev_edge();
        }
      }
    }
  }
}


/* TRANSPORT_ROUTER SERIALIZATION */

TCProto::TransportRouter TransportRouter::Serialize() const {
  TCProto::TransportRouter proto;
  auto& routing_settings_proto = *proto.mutable_routing_settings();
  routing_settings_proto.set_bus_wait_time(routing_settings_.bus_wait_time);
  routing_settings_proto.set_bus_velocity(routing_settings_.bus_velocity);

  (*proto.mutable_graph()) = graph_.Serialize();
  (*proto.mutable_router()) = router_->Serialize();

  for (const auto& [name, vertex_ids] : stops_vertex_ids_) {
    auto& vertex_ids_proto = *proto.add_stops_vertex_ids();
    vertex_ids_proto.set_name(name);
    vertex_ids_proto.set_in(vertex_ids.in);
    vertex_ids_proto.set_out(vertex_ids.out);
  }

  for (const auto& [stop_name] : vertices_info_) {
    proto.add_vertices_info()->set_stop_name(stop_name);
  }

  for (const auto& edge_info : edges_info_) {
    auto& edge_info_proto = *proto.add_edges_info();
    if (holds_alternative<BusEdgeInfo>(edge_info)) {
      const auto& bus_edge_info = get<BusEdgeInfo>(edge_info);
      auto& bus_edge_info_proto = *edge_info_proto.mutable_bus_data();
      bus_edge_info_proto.set_bus_name(bus_edge_info.bus_name);
      bus_edge_info_proto.set_start_stop_idx(bus_edge_info.start_stop_idx);
      bus_edge_info_proto.set_finish_stop_idx(bus_edge_info.finish_stop_idx);
      bus_edge_info_proto.set_span_count(bus_edge_info.span_count);
    }
    else {
      edge_info_proto.mutable_wait_data();
    }
  }
  return proto;
}

TransportRouter::TransportRouter(const TCProto::TransportRouter& proto) {
  routing_settings_.bus_wait_time = proto.routing_settings().bus_wait_time();
  routing_settings_.bus_velocity = proto.routing_settings().bus_velocity();

  graph_ = Graph::DirectedWeightedGraph<double>(proto.graph());
  router_ = make_unique<Router>(proto.router(), graph_);

  for (const auto& stop_vertex_ids_proto : proto.stops_vertex_ids()) {
    stops_vertex_ids_[stop_vertex_ids_proto.name()] = {
        stop_vertex_ids_proto.in(),
        stop_vertex_ids_proto.out(),
    };
  }

  vertices_info_.reserve(proto.vertices_info_size());
  for (const auto& vertex_info_proto : proto.vertices_info()) {
    vertices_info_.emplace_back().stop_name = vertex_info_proto.stop_name();
  }

  edges_info_.reserve(proto.edges_info_size());
  for (const auto& edge_info_proto : proto.edges_info()) {
    auto& edge_info = edges_info_.emplace_back();
    if (edge_info_proto.has_bus_data()) {
      const auto& bus_info_proto = edge_info_proto.bus_data();
      edge_info = BusEdgeInfo{
          bus_info_proto.bus_name(),
          bus_info_proto.start_stop_idx(),
          bus_info_proto.finish_stop_idx(),
          bus_info_proto.span_count()
      };
    }
    else {
      edge_info = WaitEdgeInfo{};
    }
  }
}


/* PAINTER SERIALIZATION */

SvgProto::Point SerializePoint(Svg::Point point) {
  SvgProto::Point proto;
  proto.set_x(point.x);
  proto.set_y(point.y);
  return proto;
}

Svg::Point DeserializePoint(const SvgProto::Point& proto) {
  return { proto.x(), proto.y() };
}

SvgProto::Color SerializeColor(const Svg::Color& color) {
  SvgProto::Color proto;
  if (holds_alternative<monostate>(color)) {
    proto.set_is_none(true);
  }
  else if (holds_alternative<string>(color)) {
    const string& name = get<string>(color);
    proto.set_name(name);
  }
  else {
    const bool has_opacity = holds_alternative<Svg::Rgba>(color);
    const Svg::Rgb& rgb = has_opacity ? get<Svg::Rgba>(color).color : get<Svg::Rgb>(color);
    auto& rgba_proto = *proto.mutable_rgba();
    rgba_proto.set_red(rgb.red);
    rgba_proto.set_green(rgb.green);
    rgba_proto.set_blue(rgb.blue);
    if (has_opacity) {
      rgba_proto.set_has_opacity(true);
      rgba_proto.set_opacity(get<Svg::Rgba>(color).alpha);
    }
  }
  return proto;
}

Svg::Color DeserializeColor(const SvgProto::Color& proto) {
  Svg::Color result;
  if (proto.is_none()) {
    return monostate{};
  }

  if (!proto.has_rgba()) {
    return proto.name();
  }

  const auto& rgba_proto = proto.rgba();
  const auto red = static_cast<uint8_t>(rgba_proto.red());
  const auto green = static_cast<uint8_t>(rgba_proto.green());
  const auto blue = static_cast<uint8_t>(rgba_proto.blue());
  const Svg::Rgb rgb{ red, green, blue };
  if (rgba_proto.has_opacity()) {
    return Svg::Rgba{ rgb, rgba_proto.opacity() };
  }
  else {
    return rgb;
  }
}


TCProto::Painter Paint::Painter::Serialize() const {
  TCProto::Painter proto;

  auto& settings = *proto.mutable_render_settings();
  settings.set_max_width(settings_.width);
  settings.set_max_height(settings_.height);
  settings.set_padding(settings_.padding);
  settings.set_outer_margin(settings_.outer_margin);
  settings.set_company_radius(settings_.company_radius);
  settings.set_company_radius(settings_.company_line_width);
  for (const Svg::Color& color : settings_.color_palette) {
    *(settings.add_palette()) = SerializeColor(color);
  }
  settings.set_line_width(settings_.line_width);
  (*settings.mutable_underlayer_color()) = SerializeColor(settings_.underlayer_color);
  settings.set_underlayer_width(settings_.underlayer_width);
  settings.set_stop_radius(settings_.stop_radius);
  (*settings.mutable_bus_label_offset()) = SerializePoint(settings_.bus_label_offset);
  settings.set_bus_label_font_size(settings_.bus_label_font_size);
  (*settings.mutable_stop_label_offset()) = SerializePoint(settings_.stop_label_offset);
  settings.set_stop_label_font_size(settings_.stop_label_font_size);
  for (const string& layer : settings_.layers) {
    settings.add_layers(layer);
  }

  for (const auto& [name, point] : places_coords_) {
    auto& stop_coords_proto = *proto.add_stops_coords();
    stop_coords_proto.set_name(name);
    (*stop_coords_proto.mutable_point()) = SerializePoint(point);
  }

  for (const auto& [name, color] : bus_colors_) {
    auto& bus_color_proto = *proto.add_bus_colors();
    bus_color_proto.set_name(name);
    (*bus_color_proto.mutable_color()) = SerializeColor(color);
  }

  for (const auto& [name, bus] : *buses_dict_) {
    auto& protobus =  *proto.add_bus_descriptions();
    protobus.set_name(name);
    protobus.set_is_roundtrip(bus->is_roundtrip);
    for (const string& stop : bus->stops) {
      protobus.add_stops(stop);
    }
    for (const string& stop : bus->endpoints) {
      protobus.add_endpoints(stop);
    }
  }
  return proto;
}

Paint::RenderSettings DeserializeSettings(const TCProto::Painter& p) {
  Paint::RenderSettings result;
  result.width = p.render_settings().max_width();
  result.height = p.render_settings().max_height();
  result.padding = p.render_settings().padding();
  result.stop_radius = p.render_settings().stop_radius();
  result.line_width = p.render_settings().line_width();
  result.stop_label_font_size = p.render_settings().stop_label_font_size();
  result.stop_label_offset = DeserializePoint(p.render_settings().stop_label_offset());
  result.underlayer_color = DeserializeColor(p.render_settings().underlayer_color());
  result.underlayer_width = p.render_settings().underlayer_width();
  for (const auto& color : p.render_settings().palette()) {
    result.color_palette.push_back(DeserializeColor(color));
  }
  result.bus_label_font_size = p.render_settings().bus_label_font_size();
  result.bus_label_offset = DeserializePoint(p.render_settings().bus_label_offset());
  for (const auto& l : p.render_settings().layers()) {
    result.layers.push_back(l);
  }
  result.outer_margin = p.render_settings().outer_margin();
  result.company_radius = p.render_settings().company_radius();
  result.company_line_width = p.render_settings().company_line_width();
  return result;
}

Descriptions::BusesDict DeserializeBusesDict(const TCProto::Painter& p) {
  Descriptions::BusesDict result;
  for (const auto& item : p.bus_descriptions()) {
    Descriptions::Bus bus;
    bus.name = item.name();
    for (const auto& stop : item.stops()) {
      bus.stops.push_back(stop);
    }
    for (const auto& end : item.endpoints()) {
      bus.endpoints.push_back(end);
    }
    bus.is_roundtrip = item.is_roundtrip();
    result[item.name()] = make_unique<Descriptions::Bus>(move(bus));
  }
  return result;
}

std::map<std::string, Svg::Point> DeserialzieStopsCoords(const TCProto::Painter& p) {
  std::map<std::string, Svg::Point> result;
  for (const auto& item : p.stops_coords()) {
    result[item.name()] = DeserializePoint(item.point());
  }
  return result;
}

std::unordered_map<std::string, Svg::Color> DeserializeBusColors(const TCProto::Painter& p) {
  std::unordered_map<std::string, Svg::Color> result;
  for (const auto& item : p.bus_colors()) {
    result[item.name()] = DeserializeColor(item.color());
  }
  return result;
}

Paint::Painter::Painter(const TCProto::Painter& proto) 
  : settings_(DeserializeSettings(proto)),
  buses_dict_(make_unique<Descriptions::BusesDict>(DeserializeBusesDict(proto))),
  places_coords_(DeserialzieStopsCoords(proto)),
  bus_colors_(DeserializeBusColors(proto)),
  base_map_(MakeDocument()) {}


/* COMPANIES_CATALOG SERIALIZATION */

YellowPages::Database CompaniesCatalog::Serialize() const {
  YellowPages::Database proto;
  for (const auto& company : companies_) {
    (*proto.add_companies()) = company;
    
  }
  for (const auto& [number, rubric] : rubrics_mapping_) {
    auto& protorubric = proto.mutable_rubrics()->operator[](number);
    protorubric.set_name(rubric);
  }
  return proto;
}

CompaniesCatalog::CompaniesCatalog(const YellowPages::Database& proto)
{
  for (const auto& item : proto.rubrics()) {
    rubrics_mapping_[item.first] = item.second.name();
  }
  companies_.reserve(proto.companies_size());
  for (auto& protocompany : proto.companies()) {
    companies_.push_back(protocompany);
    Distribute(&companies_.back());
  }
}


/* TRANSPORT_CATALOG SERIALIZATION */

void TransportCatalog::Serialize(ostream& os) const {
  TCProto::TransportCatalog db_proto;
  for (const auto& [name, stop] : stops_) {
    TCProto::StopResponse& stop_proto = *db_proto.add_stops();
    stop_proto.set_name(name);
    for (const string& bus_name : stop.bus_names) {
      stop_proto.add_bus_names(bus_name);
    }
  }

  for (const auto& [name, bus] : buses_) {
    TCProto::BusResponse& bus_proto = *db_proto.add_buses();
    bus_proto.set_name(name);
    bus_proto.set_stop_count(bus.stop_count);
    bus_proto.set_unique_stop_count(bus.unique_stop_count);
    bus_proto.set_road_route_length(bus.road_route_length);
    bus_proto.set_geo_route_length(bus.geo_route_length);
  }

  (*db_proto.mutable_router()) = router_->Serialize();
  (*db_proto.mutable_painter()) = painter_->Serialize();
  (*db_proto.mutable_companies()) = companies_->Serialize();

  db_proto.SerializeToOstream(&os);
}

TransportCatalog::TransportCatalog(istream& is) {
  TCProto::TransportCatalog proto;
  proto.ParseFromIstream(&is);

  for (const TCProto::StopResponse& stop_proto : proto.stops()) {
    Stop& stop = stops_[stop_proto.name()];
    for (const string& bus_name : stop_proto.bus_names()) {
      stop.bus_names.insert(bus_name);
    }
  }

  for (const TCProto::BusResponse& bus_proto : proto.buses()) {
    Bus& bus = buses_[bus_proto.name()];
    bus.stop_count = bus_proto.stop_count();
    bus.unique_stop_count = bus_proto.unique_stop_count();
    bus.road_route_length = bus_proto.road_route_length();
    bus.geo_route_length = bus_proto.geo_route_length();
  }

  router_ = make_unique<TransportRouter>(proto.router());
  painter_ = make_unique<Paint::Painter>(proto.painter());
  companies_ = make_unique<CompaniesCatalog>(move(proto.companies()));
}