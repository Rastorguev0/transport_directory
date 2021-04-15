#include "painter.h"

using namespace std;

Serialize::SvgPoint SerializePoint(const Svg::Point& p) {
  Serialize::SvgPoint result;
  result.set_x(p.x);
  result.set_y(p.y);
  return result;
}

Svg::Point DeserializePoint(const Serialize::SvgPoint& p) {
  return { p.x(), p.y() };
}

Serialize::SvgColor SerializeColor(const Svg::Color& c) {
  Serialize::SvgColor result;
  if (holds_alternative<string>(c)) {
    Serialize::StrColor color;
    color.set_color(get<string>(c));
    *(result.mutable_str()) = color;
  }
  else if (holds_alternative<Svg::Rgb>(c)) {
    Serialize::Rgb s_c;
    s_c.set_red(get<Svg::Rgb>(c).red);
    s_c.set_green(get<Svg::Rgb>(c).green);
    s_c.set_blue(get<Svg::Rgb>(c).blue);
    *(result.mutable_rgb()) = s_c;
  }
  else if (holds_alternative<Svg::Rgba>(c)) {
    Serialize::Rgb _c;
    _c.set_red(get<Svg::Rgba>(c).color.red);
    _c.set_green(get<Svg::Rgba>(c).color.green);
    _c.set_blue(get<Svg::Rgba>(c).color.blue);
    Serialize::Rgba s_c;
    *(s_c.mutable_color()) = _c;
    s_c.set_alpha(get<Svg::Rgba>(c).alpha);
    *(result.mutable_rgba()) = s_c;
  }
  return result;
}

Svg::Color DeserializeColor(const Serialize::SvgColor& c) {
  Svg::Color result;
  if (c.has_str()) {
    result = c.str().color();
  }
  else if (c.has_rgb()) {
    Svg::Rgb rgb;
    rgb.red = c.rgb().red();
    rgb.green= c.rgb().green();
    rgb.blue = c.rgb().blue();
    result = rgb;
  }
  else if (c.has_rgba()) {
    Svg::Rgba rgba;
    rgba.color.red = c.rgba().color().red();
    rgba.color.green = c.rgba().color().green();
    rgba.color.blue = c.rgba().color().blue();
    rgba.alpha = c.rgba().alpha();
    result = rgba;
  }
  return result;
}

Serialize::Painter Paint::Painter::Serialize() const {
  Serialize::Painter result;

  Serialize::RenderSettings settings;
  settings.set_width(settings_.width);
  settings.set_height(settings_.height);
  settings.set_padding(settings_.padding);
  settings.set_stop_radius(settings_.stop_radius);
  settings.set_line_width(settings_.line_width);
  settings.set_stop_label_font_size(settings_.stop_label_font_size);
  *(settings.mutable_stop_label_offset()) = SerializePoint(settings_.stop_label_offset);
  *(settings.mutable_underlayer_color()) = SerializeColor(settings_.underlayer_color);
  settings.set_underlayer_width(settings_.underlayer_width);
  for (const auto& color : settings_.color_palette) {
    *(settings.add_color_palette()) = SerializeColor(color);
  }
  settings.set_bus_label_font_size(settings_.bus_label_font_size);
  *(settings.mutable_bus_label_offset()) = SerializePoint(settings_.bus_label_offset);
  for (const auto& l : settings_.layers) {
    settings.add_layers(l);
  }
  settings.set_outer_margin(settings_.outer_margin);
  *(result.mutable_settings_()) = move(settings);

  for (const auto& [name, bus] : *buses_dict_) {
    Serialize::BusesDict dict;
    dict.set_name(name);
    for (const auto& stop : bus->stops) {
      dict.add_stops(stop);
    }
    for (const auto& end : bus->endpoints) {
      dict.add_endpoints(end);
    }
    dict.set_is_roundtrip(bus->is_roundtrip);
    *(result.add_buses_dict_()) = move(dict);
  }

  for (const auto& [name, stop] : stops_coords_) {
    Serialize::StopCoord sc;
    sc.set_name(name);
    *(sc.mutable_point()) = SerializePoint(stop);
    *(result.add_stops_coords_()) = move(sc);
  }

  for (const auto& [bus, color] : bus_colors_) {
    Serialize::BusColor bc;
    bc.set_name(bus);
    *(bc.mutable_color()) = SerializeColor(color);
    *(result.add_bus_colors_()) = move(bc);
  }
  return result;
}

Paint::RenderSettings DeserializeSettings(const Serialize::Painter& p) {
  Paint::RenderSettings result;
  result.width = p.settings_().width();
  result.height = p.settings_().height();
  result.padding = p.settings_().padding();
  result.stop_radius = p.settings_().stop_radius();
  result.line_width = p.settings_().line_width();
  result.stop_label_font_size = p.settings_().stop_label_font_size();
  result.stop_label_offset = DeserializePoint(p.settings_().stop_label_offset());
  result.underlayer_color = DeserializeColor(p.settings_().underlayer_color());
  result.underlayer_width = p.settings_().underlayer_width();
  for (const auto& color : p.settings_().color_palette()) {
    result.color_palette.push_back(DeserializeColor(color));
  }
  result.bus_label_font_size = p.settings_().bus_label_font_size();
  result.bus_label_offset = DeserializePoint(p.settings_().bus_label_offset());
  for (const auto& l : p.settings_().layers()) {
    result.layers.push_back(l);
  }
  result.outer_margin = p.settings_().outer_margin();
  return result;
}

Descriptions::BusesDict DeserializeBusesDict(const Serialize::Painter& p) {
  Descriptions::BusesDict result;
  for (const auto& item : p.buses_dict_()) {
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

std::map<std::string, Svg::Point> DeserialzieStopsCoords(const Serialize::Painter& p) {
  std::map<std::string, Svg::Point> result;
  for (const auto& item : p.stops_coords_()) {
    result[item.name()] = DeserializePoint(item.point());
  }
  return result;
}

std::unordered_map<std::string, Svg::Color> DeserializeBusColors(const Serialize::Painter& p) {
  std::unordered_map<std::string, Svg::Color> result;
  for (const auto& item : p.bus_colors_()) {
    result[item.name()] = DeserializeColor(item.color());
  }
  return result;
}


Paint::Painter::Painter(const Serialize::Painter& painter)
  : settings_(DeserializeSettings(painter)),
  buses_dict_(make_shared<Descriptions::BusesDict>(DeserializeBusesDict(painter))),
  stops_coords_(DeserialzieStopsCoords(painter)),
  bus_colors_(DeserializeBusColors(painter)),
  base_map_(MakeDocument())
{}