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

  static map<string, Svg::Point> ComputePlacesCoords(
    const Descriptions::StopsDict& stops_dict,
    const Descriptions::BusesDict& buses_dict,
    const vector<YellowPages::Company>& companies,
    const RenderSettings& render_settings) {

    const double max_width = render_settings.width;
    const double max_height = render_settings.height;
    const double padding = render_settings.padding;
    Aligner aligner(stops_dict, buses_dict, companies, max_width, max_height, padding);

    map<string, Svg::Point> result;
    for (const auto& [name, _] : stops_dict) {
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

  void Painter::PaintMoveLines(Svg::Document& svg) const {
    for (const auto& [bus_name, bus] : *buses_dict_) {
      const auto& stops = bus->stops;
      if (stops.empty()) {
        continue;
      }
      auto line = PaintBaseLine(bus_name);
      for (const auto& stop_name : stops) {
        line.AddPoint(places_coords_.at(stop_name));
      }
      svg.Add(line);
    }
  }

  void Painter::PaintMoveLines(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const {
    for (const auto& [bus, start, finish] : buses) {
      auto line = PaintBaseLine(bus);
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        line.AddPoint(places_coords_.at(stops[stop_idx]));
      }
      svg.Add(line);
    }
  }

  void Painter::PaintMoveLines(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const {
    for (const auto& [stop, name, _] : walks) {
      svg.Add(Svg::Polyline{}
        .AddPoint(places_coords_.at(stop))
        .AddPoint(places_coords_.at(name))
        .SetStrokeColor("black")
        .SetStrokeWidth(settings_.company_line_width)
        .SetStrokeLineCap("round")
        .SetStrokeLineJoin("round"));
    }
  }


  void Painter::PaintMoveLabels(Svg::Document& svg) const {
    for (const auto& [bus_name, bus] : *buses_dict_) {
      const auto& stops = bus->stops;
      if (!stops.empty()) {
        for (const string& endpoint : bus->endpoints) {
          PaintMoveLabel(svg, places_coords_.at(endpoint), bus_name);
        }
      }
    }
  }

  void Painter::PaintMoveLabels(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const {
    for (const auto& [bus, start, finish] : buses) {
      const auto& endpoints = buses_dict_->at(bus)->endpoints;
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        if (find(begin(endpoints), end(endpoints), stops[stop_idx]) != end(endpoints)) {
          PaintMoveLabel(svg, places_coords_.at(stops[stop_idx]), bus);
        }
      }
    }
  }


  void Painter::PaintPlacePoints(Svg::Document& svg) const {
    for (const auto& [stop_name, _] : *stops_dict_) {
      PaintPlacePoint(svg, places_coords_.at(stop_name));
    }
  }

  void Painter::PaintPlacePoints(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const {
    for (const auto& [bus, start, finish] : buses) {
      const auto& stops = buses_dict_->at(bus)->stops;
      for (size_t stop_idx = start; stop_idx <= finish; stop_idx++) {
        PaintPlacePoint(svg, places_coords_.at(stops[stop_idx]));
      }
    }
  }

  void Painter::PaintPlacePoints(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const {
    for (const auto& [_, name, __] : walks) {
      svg.Add(Svg::Circle{}
      .SetCenter(places_coords_.at(name))
      .SetRadius(settings_.company_radius)
      .SetFillColor("black"));
    }
  }


  void Painter::PaintPlaceLabels(Svg::Document& svg) const {
    for (const auto& [stop_name, stop] : *stops_dict_) {
      PaintPlaceLabel(svg, places_coords_.at(stop_name), stop_name);
    }
  }

  void Painter::PaintPlaceLabels(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const {
    if (buses.empty()) return;
    const auto& bus = buses.front().bus_name;
    const auto& start = buses_dict_->at(bus)->stops[buses.front().start_stop_idx];
    PaintPlaceLabel(svg, places_coords_.at(start), start);
    for (const auto& [bus, start, finish] : buses) {
      const auto& transfer = buses_dict_->at(bus)->stops[finish];
      PaintPlaceLabel(svg, places_coords_.at(transfer), transfer);
    }
  }

  void Painter::PaintPlaceLabels(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const {
    for (const auto& [_, name, rubric] : walks) {
      PaintPlaceLabel(svg, places_coords_.at(name),
        rubric + string(min((size_t)1, rubric.size()), ' ') + name);
    }
  }


  void Painter::PaintMoveLabel(
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

  void Painter::PaintPlaceLabel(
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

  void Painter::PaintPlacePoint(Svg::Document& svg, Svg::Point pos) const {
    svg.Add(Svg::Circle{}
      .SetCenter(pos)
      .SetRadius(settings_.stop_radius)
      .SetFillColor("white"));
  }

  Svg::Polyline Painter::PaintBaseLine(const std::string& bus_name) const {
    return Svg::Polyline{}.SetStrokeColor(bus_colors_.at(bus_name))
      .SetStrokeWidth(settings_.line_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round");
  }


  Svg::Document Painter::MakeDocument() const {
    Svg::Document doc;
    for (const auto& layer : settings_.layers) {
      if (LAYER_ACTIONS.count(layer))
        (this->*LAYER_ACTIONS.at(layer))(doc);
    }
    return doc;
  }

  const unordered_map<string, void (Painter::*)(Svg::Document&) const> Painter::LAYER_ACTIONS = {
      {"bus_lines",   static_cast<void (Painter::*)(Svg::Document&) const>(&Painter::PaintMoveLines)},
      {"bus_labels",  static_cast<void (Painter::*)(Svg::Document&) const>(&Painter::PaintMoveLabels)},
      {"stop_points", static_cast<void (Painter::*)(Svg::Document&) const>(&Painter::PaintPlacePoints)},
      {"stop_labels", static_cast<void (Painter::*)(Svg::Document&) const>(&Painter::PaintPlaceLabels)},
  };

  const unordered_map<
    string, void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const> Painter::MOVE_LAYER_ACTIONS = {
      {"bus_lines",   static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const>(&Painter::PaintMoveLines)},
      {"bus_labels",  static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const>(&Painter::PaintMoveLabels)},
      {"stop_points", static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const>(&Painter::PaintPlacePoints)},
      {"stop_labels", static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const>(&Painter::PaintPlaceLabels)},
  };

  const unordered_map<
    string, void (Painter::*)(Svg::Document&, const Route::Items<Route::Walk>&) const> Painter::WALK_LAYER_ACTIONS = {
      {"company_lines",  static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Walk>&) const>(&Painter::PaintMoveLines)},
      {"company_points", static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Walk>&) const>(&Painter::PaintPlacePoints)},
      {"company_labels", static_cast<void (Painter::*)(Svg::Document&, const Route::Items<Route::Walk>&) const>(&Painter::PaintPlaceLabels)},
  };


  Painter::Painter(const Json::Dict& render_settings_json,
    shared_ptr<Descriptions::BusesDict> buses,
    shared_ptr<Descriptions::StopsDict> stops,
    const vector<YellowPages::Company>& companies)
    : settings_(ParseSettings(render_settings_json)),
    buses_dict_(buses),
    stops_dict_(stops),
    places_coords_(ComputePlacesCoords(*stops, *buses, companies, settings_)),
    bus_colors_(ChooseBusColors(*buses, settings_)),
    base_map_(MakeDocument())
  {
  };

  string Painter::Paint(std::optional<Route> route) const {
    Svg::Document route_map = base_map_;
    if (route.has_value()) {
      route_map.Add(
        Svg::Rectangle{}
        .SetCorner({ -settings_.outer_margin, -settings_.outer_margin })
        .SetWidth(settings_.width + 2 * settings_.outer_margin)
        .SetHeight(settings_.height + 2 * settings_.outer_margin)
        .SetFillColor(settings_.underlayer_color)
      );

      string stop_from;
      if (route->buses.empty() && !route->walks.empty())
        stop_from = route->walks.front().stop_from;

      for (const auto& layer : settings_.layers) {
        if (!stop_from.empty() && layer == "stop_labels") {
          PaintPlaceLabel(route_map, places_coords_.at(stop_from), stop_from);
        }
        else if (MOVE_LAYER_ACTIONS.count(layer))
          (this->*MOVE_LAYER_ACTIONS.at(layer))(route_map, route->buses);
        else
          (this->*WALK_LAYER_ACTIONS.at(layer))(route_map, route->walks);
      }
    }
    ostringstream result;
    route_map.Render(result);
    return result.str();
  }

}