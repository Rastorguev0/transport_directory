#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"
#include "descriptions.h"

#include <map>
#include <vector>
#include <string>
#include <unordered_map>

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
};


class Painter {
public:
  Painter(const Json::Dict& render_settings_json,
    const Descriptions::BusesDict& buses,
    const Descriptions::StopsDict& stops);

  std::string Paint() const;
private:
  RenderSettings settings_;
  const Descriptions::BusesDict& buses_dict_;
  std::map<std::string, Svg::Point> stops_coords_;
  std::unordered_map<std::string, Svg::Color> bus_colors_;

  void PaintBusLines(Svg::Document& svg) const;
  void PaintBusLabels(Svg::Document& svg) const;
  void PaintStopPoints(Svg::Document& svg) const;
  void PaintStopLabels(Svg::Document& svg) const;

  static const std::unordered_map<std::string, void (Painter::*)(Svg::Document&) const> LAYER_ACTIONS;
};