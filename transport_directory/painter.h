#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"

#include <algorithm>
#include <map>
#include <variant>
#include <sstream>
#include <optional>
#include <fstream>

struct Borders {
	double min_lat;
	double min_lon;
	double max_lat;
	double max_lon;
};

struct LayerTags {
	const std::string bus_lines = "bus_lines";
	const std::string bus_labels = "bus_labels";
	const std::string stop_points = "stop_points";
	const std::string stop_labels = "stop_labels";
};


class Painter {
public:
	Painter(const Json::Dict& render_settings_json, const Borders& borders);

	Svg::Point ProjectSpherePoint(Sphere::Point p) const;
	void PaintStop(Sphere::Point center);
	void PaintRoute(const std::vector<Sphere::Point>& points);
	void PaintStopName(Sphere::Point coords, const std::string& text);
	void PaintBusName(Sphere::Point coords, const std::string& text, bool preserve_prev_color = false);
	std::string Paint();

private:
	using Layers = std::map<std::string, std::vector<std::variant<Svg::Circle, Svg::Polyline, Svg::Text>>>;
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

	RenderSettings MakeSettings(const Json::Dict& json);
	Svg::Point ParsePoint(const Json::Node& node);
	Svg::Color ParseColor(const Json::Node& node);
	std::vector<Svg::Color> ParsePalette(const Json::Node& node);
	std::vector<std::string> ParseVectorStrings(const Json::Node& node);
	Svg::Text CreateUnderlayer(const Svg::Text& base);

	Svg::Document svg_;
	std::optional<std::string> rendered_map_;
	Layers layers_;
	const LayerTags tags_;
	const RenderSettings render_settings_;
	const Borders borders_;
	double zoom_rate = .0;
};