#include "painter.h"
#include "aligner.h"

#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_set>
#include <optional>
#include <utility>
#include <fstream>

using namespace std;

namespace Paint {

  Svg::Point ParsePoint(const Json::Node& node) {
    return Svg::Point{
    node.AsArray().at(0).AsDouble(),
    node.AsArray().at(1).AsDouble()
    };
  }

  Svg::Color ParseColor(const Json::Node& node) {
    const auto& variant_color = node.GetBase();
    Svg::Color color;
    if (holds_alternative<string>(variant_color)) {
      color = node.AsString();
    }
    else {
      const auto& color_atts = node.AsArray();
      const Svg::Rgb rgb = {
        static_cast<uint8_t>(color_atts.at(0).AsInt()),
        static_cast<uint8_t>(color_atts.at(1).AsInt()),
        static_cast<uint8_t>(color_atts.at(2).AsInt())
      };
      color = color_atts.size() == 3 ? rgb : Svg::Color(Svg::Rgba{ rgb, color_atts.at(3).AsDouble() });
    }
    return color;
  }

  vector<Svg::Color> ParsePalette(const Json::Node& node) {
    const auto& colors = node.AsArray();
    vector<Svg::Color> palette;
    palette.reserve(colors.size());
    for (const auto& color : colors) {
      palette.push_back(ParseColor(color));
    }
    return palette;
  }

  vector<string> ParseVectorStrings(const Json::Node& node) {
    vector<string> result;
    for (const auto& str : node.AsArray()) {
      result.push_back(str.AsString());
    }
    return result;
  }

  RenderSettings ParseSettings(const Json::Dict& json) {
    return {
      .width = json.at("width").AsDouble(),
      .height = json.at("height").AsDouble(),
      .padding = json.at("padding").AsDouble(),
      .stop_radius = json.at("stop_radius").AsDouble(),
      .line_width = json.at("line_width").AsDouble(),
      .stop_label_font_size = json.at("stop_label_font_size").AsInt(),
      .stop_label_offset = ParsePoint(json.at("stop_label_offset")),
      .underlayer_color = ParseColor(json.at("underlayer_color")),
      .underlayer_width = json.at("underlayer_width").AsDouble(),
      .color_palette = ParsePalette(json.at("color_palette")),
      .bus_label_font_size = json.at("bus_label_font_size").AsInt(),
      .bus_label_offset = ParsePoint(json.at("bus_label_offset")),
      .layers = ParseVectorStrings(json.at("layers")),
      .outer_margin = json.at("outer_margin").AsDouble(),
      .company_radius = json.at("company_radius").AsDouble(),
      .company_line_width = json.at("company_line_width").AsDouble(),
    };
  }

  static map<string, Svg::Point> ComputeStopsCoords(
    const Descriptions::StopsDict& stops,
    const Descriptions::BusesDict& buses_dict,
    const vector<YellowPages::Company>& companies,
    const RenderSettings& render_settings) {

    const double max_width = render_settings.width;
    const double max_height = render_settings.height;
    const double padding = render_settings.padding;
    Aligner aligner(stops, buses_dict, companies, max_width, max_height, padding);

    map<string, Svg::Point> result;
    for (const auto& [name, _] : stops) {
      result[name] = aligner(name);
    }
    for (const auto& company : companies) {
      string name = CompanyMainName(company);
      result[name] = aligner(name);
    }
    return result;
  }

  static unordered_map<string, Svg::Color> ChooseBusColors(
    const Descriptions::BusesDict& buses_dict,
    const RenderSettings& render_settings) {

    const auto& palette = render_settings.color_palette;
    unordered_map<string, Svg::Color> bus_colors;
    int idx = 0;
    for (const auto& [bus_name, _] : buses_dict) {
      bus_colors[bus_name] = palette[idx++ % palette.size()];
    }
    return bus_colors;
  }

  void Painter::PaintBusLines(Svg::Document& svg) const {
    for (const auto& [bus_name, bus] : *buses_dict_) {
      const auto& stops = bus->stops;
      if (stops.empty()) {
        continue;
      }
      auto line = BaseBusLine(bus_name);
      for (const auto& stop_name : stops) {
        line.AddPoint(places_coords_.at(stop_name));
      }
      svg.Add(line);
    }
  }

  void Painter::PaintRouteBusLines(Svg::Document& svg, const RouteInfo& route) const {
    for (const auto& [bus, start, finish] : route) {
      auto line = BaseBusLine(bus);
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        line.AddPoint(places_coords_.at(stops[stop_idx]));
      }
      svg.Add(line);
    }
  }

  void Painter::PaintBusLabels(Svg::Document& svg) const {
    for (const auto& [bus_name, bus] : *buses_dict_) {
      const auto& stops = bus->stops;
      if (!stops.empty()) {
        for (const string& endpoint : bus->endpoints) {
          PaintBusLabel(svg, places_coords_.at(endpoint), bus_name);
        }
      }
    }
  }

  void Painter::PaintRouteBusLabels(Svg::Document& svg, const RouteInfo& route) const {
    for (const auto& [bus, start, finish] : route) {
      const auto& endpoints = buses_dict_->at(bus)->endpoints;
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        if (find(begin(endpoints), end(endpoints), stops[stop_idx]) != end(endpoints)) {
          PaintBusLabel(svg, places_coords_.at(stops[stop_idx]), bus);
        }
      }
    }
  }

  void Painter::PaintStopPoints(Svg::Document& svg) const {
    for (const auto& [stop_name, stop_point] : places_coords_) {
      PaintStopPoint(svg, stop_point);
    }
  }

  void Painter::PaintRouteStopPoints(Svg::Document& svg, const RouteInfo& route) const {
    for (const auto& [bus, start, finish] : route) {
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        PaintStopPoint(svg, places_coords_.at(stops[stop_idx]));
      }
    }
  }

  void Painter::PaintStopLabels(Svg::Document& svg) const {
    for (const auto& [stop_name, stop_point] : places_coords_) {
      PaintStopLabel(svg, stop_point, stop_name);
    }
  }

  void Painter::PaintRouteStopLabels(Svg::Document& svg, const RouteInfo& route) const {
    if (route.items.empty()) return;
    const auto& bus = route.items.front().bus_name;
    const auto& start = buses_dict_->at(bus)->stops[route.items.front().start_stop_idx];
    PaintStopLabel(svg, places_coords_.at(start), start);
    for (const auto& [bus, start, finish] : route.items) {
      const auto& transfer = buses_dict_->at(bus)->stops[finish];
      PaintStopLabel(svg, places_coords_.at(transfer), transfer);
    }
  }


  void Painter::PaintBusLabel(
    Svg::Document& svg, Svg::Point pos, const string& name) const {
    const auto base_text =
      Svg::Text{}
      .SetPoint(pos)
      .SetOffset(settings_.bus_label_offset)
      .SetFontSize(settings_.bus_label_font_size)
      .SetFontFamily("Verdana")
      .SetFontWeight("bold")
      .SetData(name);
    svg.Add(
      Svg::Text(base_text)
      .SetFillColor(settings_.underlayer_color)
      .SetStrokeColor(settings_.underlayer_color)
      .SetStrokeWidth(settings_.underlayer_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round")
    );
    svg.Add(
      Svg::Text(base_text)
      .SetFillColor(bus_colors_.at(name))
    );
  }

  void Painter::PaintStopLabel(
    Svg::Document& svg, Svg::Point pos, const string& name) const {
    const auto base_text =
      Svg::Text{}
      .SetPoint(pos)
      .SetOffset(settings_.stop_label_offset)
      .SetFontSize(settings_.stop_label_font_size)
      .SetFontFamily("Verdana")
      .SetData(name);
    svg.Add(
      Svg::Text(base_text)
      .SetFillColor(settings_.underlayer_color)
      .SetStrokeColor(settings_.underlayer_color)
      .SetStrokeWidth(settings_.underlayer_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round")
    );
    svg.Add(
      Svg::Text(base_text)
      .SetFillColor("black")
    );
  }

  void Painter::PaintStopPoint(Svg::Document& svg, Svg::Point pos) const {
    svg.Add(Svg::Circle{}
      .SetCenter(pos)
      .SetRadius(settings_.stop_radius)
      .SetFillColor("white"));
  }

  Svg::Polyline Painter::BaseBusLine(const std::string& bus_name) const {
    return Svg::Polyline{}.SetStrokeColor(bus_colors_.at(bus_name))
      .SetStrokeWidth(settings_.line_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round");
  }


  Svg::Document Painter::MakeDocument() const {
    Svg::Document doc;
    for (const auto& layer : settings_.layers) {
      (this->*LAYER_ACTIONS.at(layer))(doc);
    }
    return doc;
  }


  const unordered_map<string, void (Painter::*)(Svg::Document&) const> Painter::LAYER_ACTIONS = {
      {"bus_lines",   &Painter::PaintBusLines},
      {"bus_labels",  &Painter::PaintBusLabels},
      {"stop_points", &Painter::PaintStopPoints},
      {"stop_labels", &Painter::PaintStopLabels},
  };
  const unordered_map<string, void (Painter::*)(Svg::Document&, const RouteInfo&) const> Painter::ROUTE_LAYER_ACTIONS = {
      {"bus_lines",   &Painter::PaintRouteBusLines},
      {"bus_labels",  &Painter::PaintRouteBusLabels},
      {"stop_points", &Painter::PaintRouteStopPoints},
      {"stop_labels", &Painter::PaintRouteStopLabels},
  };


  Painter::Painter(const Json::Dict& render_settings_json,
    shared_ptr<Descriptions::BusesDict> buses,
    shared_ptr<Descriptions::StopsDict> stops,
    const vector<YellowPages::Company>& companies)
    : settings_(ParseSettings(render_settings_json)),
    buses_dict_(buses),
    places_coords_(ComputeStopsCoords(*stops, *buses, companies, settings_)),
    bus_colors_(ChooseBusColors(*buses, settings_)),
    base_map_(MakeDocument())
  {
  };

  string Painter::Paint() const {
    ostringstream out;
    base_map_.Render(out);
    return out.str();
  }

  std::string Painter::PaintRoute(const RouteInfo& info) const {
    Svg::Document route_map = base_map_;
    route_map.Add(
      Svg::Rectangle{}
      .SetCorner({ -settings_.outer_margin, -settings_.outer_margin })
      .SetWidth(settings_.width + 2 * settings_.outer_margin)
      .SetHeight(settings_.height + 2 * settings_.outer_margin)
      .SetFillColor(settings_.underlayer_color)
    );

    for (const auto& layer : settings_.layers) {
      (this->*ROUTE_LAYER_ACTIONS.at(layer))(route_map, info);
    }

    ostringstream out;
    route_map.Render(out);
    return out.str();
  }

}