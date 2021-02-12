#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"

#include <algorithm>
#include <sstream>
#include <optional>

struct Borders {
	double min_lat;
	double min_lon;
	double max_lat;
	double max_lon;
};


class Painter {
public:
	Painter(const Json::Dict& render_settings_json, const Borders& borders);

	Svg::Point ProjectSpherePoint(Sphere::Point p) const;
	void PaintCircle(Sphere::Point center);
	void PaintPolyline(const std::vector<Sphere::Point>& points);
	void PaintText(Sphere::Point coords, const std::string& text);
	std::string Paint() const;
private:
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
	};

	RenderSettings MakeSettings(const Json::Dict& json);
	Svg::Point ParsePoint(const Json::Node& node);
	Svg::Color ParseColor(const Json::Node& node);
	std::vector<Svg::Color> ParsePalette(const Json::Node& node);

	Svg::Document svg;
	const RenderSettings render_settings_;
	const Borders borders_;
	double zoom_rate = .0;
};