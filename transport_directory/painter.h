#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"
#include "descriptions.h"

#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <variant>

namespace Paint {

  struct Link {
    std::string bus_name;
    std::vector<std::string> stops;
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
      std::shared_ptr<Descriptions::BusesOwner> buses,
      std::shared_ptr<Descriptions::StopsDict> stops);

    std::string Paint() const;
    std::string PaintRoute(const RouteChain& route) const;
  private:
    const RenderSettings settings_;
    const std::shared_ptr<Descriptions::BusesOwner> buses_dict_;
    const std::map<std::string, Svg::Point> stops_coords_;
    const std::unordered_map<std::string, Svg::Color> bus_colors_;

    Svg::Document base_map_;
    std::string rendered_map_;

    void PaintBusLines(Svg::Document& svg, const std::variant<std::monostate, RouteChain>&) const;
    void PaintBusLines(Svg::Document& svg, std::monostate) const;
    void PaintBusLines(Svg::Document& svg, const RouteChain&) const;

    void PaintBusLabels(Svg::Document& svg, const std::variant<std::monostate, RouteChain>&) const;
    void PaintBusLabels(Svg::Document& svg, std::monostate) const;
    void PaintBusLabels(Svg::Document& svg, const RouteChain&) const;

    void PaintStopPoints(Svg::Document& svg, const std::variant<std::monostate, RouteChain>&) const;
    void PaintStopPoints(Svg::Document& svg, std::monostate) const;
    void PaintStopPoints(Svg::Document& svg, const RouteChain&) const;

    void PaintStopLabels(Svg::Document& svg, const std::variant<std::monostate, RouteChain>&) const;
    void PaintStopLabels(Svg::Document& svg, std::monostate) const;
    void PaintStopLabels(Svg::Document& svg, const RouteChain&) const;

    void PaintBusLabel(
      Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintStopLabel(
      Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintStopPoint(Svg::Document& svg, Svg::Point pos) const;
    Svg::Polyline BaseBusLine(const std::string& bus_name) const;

    Svg::Document MakeDocument() const;

    static const std::unordered_map<std::string,
      void (Painter::*)(Svg::Document&, const std::variant<std::monostate, RouteChain>&) const> LAYER_ACTIONS;
  };

}