#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"
#include "descriptions.h"
#include "transport_catalog.pb.h"

#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <variant>

namespace Paint {

  struct Link {
    std::string bus_name;
    size_t start_stop_idx;
    size_t finish_stop_idx;
  };
  using RouteChain = std::vector<Link>;

  struct RenderSettings {
    double width;
    double height;
    double padding;
    double stop_radius;
    double line_width;
    int stop_label_font_size;
    Svg::Point stop_label_offset;
    Svg::Color underlayer_color;
    double underlayer_width;
    std::vector<Svg::Color> color_palette;
    int bus_label_font_size;
    Svg::Point bus_label_offset;
    std::vector<std::string> layers;
    double outer_margin;
  };


  class Painter {
  public:
    Painter() = default;
    Painter(const Json::Dict& render_settings_json,
      std::shared_ptr<Descriptions::BusesDict> buses,
      std::shared_ptr<Descriptions::StopsDict> stops);

    Serialize::Painter Serialize() const;
    Painter(const Serialize::Painter& painter);

    std::string Paint() const;
    std::string PaintRoute(const RouteChain& route) const;
  private:
    const RenderSettings settings_;
    const std::shared_ptr<Descriptions::BusesDict> buses_dict_;
    const std::map<std::string, Svg::Point> stops_coords_;
    const std::unordered_map<std::string, Svg::Color> bus_colors_;

    const Svg::Document base_map_;

    void PaintBusLines(Svg::Document& svg) const;
    void PaintBusLabels(Svg::Document& svg) const;
    void PaintStopPoints(Svg::Document& svg) const;
    void PaintStopLabels(Svg::Document& svg) const;

    void PaintRouteBusLines(Svg::Document& svg, const RouteChain&) const;
    void PaintRouteBusLabels(Svg::Document& svg, const RouteChain&) const;
    void PaintRouteStopPoints(Svg::Document& svg, const RouteChain&) const;
    void PaintRouteStopLabels(Svg::Document& svg, const RouteChain&) const;

    void PaintBusLabel(Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintStopLabel(Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintStopPoint(Svg::Document& svg, Svg::Point pos) const;
    Svg::Polyline BaseBusLine(const std::string& bus_name) const;
    Svg::Document MakeDocument() const;

    static const std::unordered_map<std::string, void (Painter::*)(Svg::Document&) const> LAYER_ACTIONS;

    static const std::unordered_map<std::string, void (Painter::*)(Svg::Document&, const RouteChain&) const> ROUTE_LAYER_ACTIONS;
  };

}