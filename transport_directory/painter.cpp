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
		.layers = ParseVectorStrings(json.at("layers"))
	};
}

static map<string, Svg::Point> ComputeStopsCoords(
	const Descriptions::StopsDict& stops_dict,
	const Descriptions::BusesDict& buses_dict,
	const RenderSettings& render_settings) {

	const double max_width = render_settings.width;
	const double max_height = render_settings.height;
	const double padding = render_settings.padding;
	Aligner aligner(stops_dict, buses_dict, max_width, max_height, padding);

	map<string, Svg::Point> result;
	for (const auto& [name, _] : stops_dict) {
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
	for (const auto& [bus_name, bus_ptr] : buses_dict) {
		bus_colors[bus_name] = palette[idx++ % palette.size()];
	}
	return bus_colors;
}


Painter::Painter(const Json::Dict& render_settings_json,
	const Descriptions::BusesDict& buses,
	const Descriptions::StopsDict& stops) 
	: settings_(ParseSettings(render_settings_json)),
	buses_dict_(buses),
	stops_coords_(ComputeStopsCoords(stops, buses, settings_)),
	bus_colors_(ChooseBusColors(buses, settings_)) {};

void Painter::PaintBusLines(Svg::Document& svg) const {
	for (const auto& [bus_name, bus] : buses_dict_) {
		const auto& stops = bus->stops;
		if (stops.empty()) {
			continue;
		}
		Svg::Polyline line;
		line.SetStrokeColor(bus_colors_.at(bus_name))
			.SetStrokeWidth(settings_.line_width)
			.SetStrokeLineCap("round").SetStrokeLineJoin("round");
		for (const auto& stop_name : stops) {
			line.AddPoint(stops_coords_.at(stop_name));
		}
		svg.Add(line);
	}
}

void Painter::PaintBusLabels(Svg::Document& svg) const {
	for (const auto& [bus_name, bus] : buses_dict_) {
		const auto& stops = bus->stops;
		if (!stops.empty()) {
			const auto& color = bus_colors_.at(bus_name);
			for (const string& endpoint : bus->endpoints) {
				const auto point = stops_coords_.at(endpoint);
				const auto base_text =
					Svg::Text{}
					.SetPoint(point)
					.SetOffset(settings_.bus_label_offset)
					.SetFontSize(settings_.bus_label_font_size)
					.SetFontFamily("Verdana")
					.SetFontWeight("bold")
					.SetData(bus_name);
				svg.Add(
					Svg::Text(base_text)
					.SetFillColor(settings_.underlayer_color)
					.SetStrokeColor(settings_.underlayer_color)
					.SetStrokeWidth(settings_.underlayer_width)
					.SetStrokeLineCap("round").SetStrokeLineJoin("round")
				);
				svg.Add(
					Svg::Text(base_text)
					.SetFillColor(color)
				);
			}
		}
	}
}

void Painter::PaintStopPoints(Svg::Document& svg) const {
	for (const auto& [stop_name, stop_point] : stops_coords_) {
		svg.Add(Svg::Circle{}
			.SetCenter(stop_point)
			.SetRadius(settings_.stop_radius)
			.SetFillColor("white"));
	}
}

void Painter::PaintStopLabels(Svg::Document& svg) const {
	for (const auto& [stop_name, stop_point] : stops_coords_) {
		const auto base_text =
			Svg::Text{}
			.SetPoint(stop_point)
			.SetOffset(settings_.stop_label_offset)
			.SetFontSize(settings_.stop_label_font_size)
			.SetFontFamily("Verdana")
			.SetData(stop_name);
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
}


const unordered_map<string, void (Painter::*)(Svg::Document&) const> Painter::LAYER_ACTIONS = {
		{"bus_lines",   &Painter::PaintBusLines},
		{"bus_labels",  &Painter::PaintBusLabels},
		{"stop_points", &Painter::PaintStopPoints},
		{"stop_labels", &Painter::PaintStopLabels},
};

string Painter::Paint() const {
	Svg::Document svg;
	for (const auto& layer : settings_.layers) {
		(this->*LAYER_ACTIONS.at(layer))(svg);
	}
	ostringstream out;
	svg.Render(out);
	//ofstream o("svg1.svg");
	ofstream o("svg2.svg");
	svg.Render(o);
	return out.str();
}