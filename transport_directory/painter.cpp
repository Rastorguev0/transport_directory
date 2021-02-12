#include "painter.h"

Painter::Painter(const Json::Dict& render_settings_json, const Borders& borders)
	: render_settings_(MakeSettings(render_settings_json)), borders_(borders) {

	double x_rate = borders.max_lon - borders.min_lon == 0 ?
		0 : (render_settings_.width - 2 * render_settings_.padding) / (borders.max_lon - borders.min_lon);
	double y_rate = borders.max_lat - borders.min_lat == 0 ?
		0 : (render_settings_.height - 2 * render_settings_.padding) / (borders.max_lat - borders.min_lat);

	zoom_rate = std::min(x_rate, y_rate);
	if (zoom_rate == 0) zoom_rate = std::max(x_rate, y_rate);

	for (const auto& layer : render_settings_.layers) {
		layers_[layer];
	}
};

Svg::Point Painter::ProjectSpherePoint(Sphere::Point p) const {
	return {
		(p.longitude - borders_.min_lon) * zoom_rate + render_settings_.padding,
		(borders_.max_lat - p.latitude) * zoom_rate + render_settings_.padding
	};
}

void Painter::PaintStop(Sphere::Point center) {
	if (layers_.count(tags_.stop_points)) {
		layers_[tags_.stop_points].push_back(
			Svg::Circle{}
			.SetCenter(ProjectSpherePoint(center))
			.SetFillColor("white")
			.SetRadius(render_settings_.stop_radius)
			);
	}
}

void Painter::PaintRoute(const std::vector<Sphere::Point>& points) {
	if (layers_.count(tags_.bus_lines)) {
		static int color_idx = 0;
		Svg::Polyline polyline =
			Svg::Polyline{}
			.SetStrokeColor(render_settings_.color_palette[color_idx++ % render_settings_.color_palette.size()])
			.SetStrokeWidth(render_settings_.line_width)
			.SetStrokeLineCap("round")
			.SetStrokeLineJoin("round");
		for (const auto& p : points) {
			polyline.AddPoint(ProjectSpherePoint(p));
		}
		layers_[tags_.bus_lines].push_back(std::move(polyline));
	}
}

void Painter::PaintBusName(Sphere::Point coords, const std::string& text, bool use_prev_color) {
	if (layers_.count(tags_.bus_labels)) {
		static int color_idx = 0;
		Svg::Text base =
			Svg::Text{}
			.SetData(text)
			.SetPoint(ProjectSpherePoint(coords))
			.SetOffset(render_settings_.bus_label_offset)
			.SetFontSize(render_settings_.bus_label_font_size)
			.SetFontWeight("bold")
			.SetFontFamily("Verdana");

		layers_[tags_.bus_labels].push_back(CreateUnderlayer(base));
		if (use_prev_color && color_idx > 0) color_idx--;
		layers_[tags_.bus_labels].push_back(Svg::Text(base).SetFillColor(
			render_settings_.color_palette[color_idx++ % render_settings_.color_palette.size()]
		));
	}
}

void Painter::PaintStopName(Sphere::Point coords, const std::string& text) {
	if (layers_.count(tags_.stop_labels)) {
		Svg::Text base =
			Svg::Text{}
			.SetData(text)
			.SetPoint(ProjectSpherePoint(coords))
			.SetOffset(render_settings_.stop_label_offset)
			.SetFontSize(render_settings_.stop_label_font_size)
			.SetFontFamily("Verdana");

		layers_[tags_.stop_labels].push_back(CreateUnderlayer(base));
		layers_[tags_.stop_labels].push_back(Svg::Text(base).SetFillColor("black"));
	}
}

Svg::Text Painter::CreateUnderlayer(const Svg::Text& base) {
	return
		Svg::Text(base)
		.SetFillColor(render_settings_.underlayer_color)
		.SetStrokeColor(render_settings_.underlayer_color)
		.SetStrokeWidth(render_settings_.underlayer_width)
		.SetStrokeLineCap("round")
		.SetStrokeLineJoin("round");
}

std::string Painter::Paint() {
	if (!rendered_map_) {
		for (const std::string& layer : render_settings_.layers) {
			for (auto&& obj : layers_.at(layer)) {
				std::visit([this](auto&& o) {
					this->svg_.Add(std::move(o)); }, obj);
			}
		}
		std::stringstream out;
		svg_.Render(out);
		rendered_map_ = out.str();
		//std::ofstream svg_out("svg.svg");
		//svg_.Render(svg_out);
	}
	return rendered_map_.value();
}

Painter::RenderSettings Painter::MakeSettings(const Json::Dict& json) {
	return {
		json.at("width").AsDouble(),
		json.at("height").AsDouble(),
		json.at("padding").AsDouble(),
		json.at("stop_radius").AsDouble(),
		json.at("line_width").AsDouble(),
		json.at("stop_label_font_size").AsInt(),
		ParsePoint(json.at("stop_label_offset")),
		ParseColor(json.at("underlayer_color")),
		json.at("underlayer_width").AsDouble(),
		ParsePalette(json.at("color_palette")),
		json.at("bus_label_font_size").AsInt(),
		ParsePoint(json.at("bus_label_offset")),
		ParseVectorStrings(json.at("layers"))
	};
}

Svg::Point Painter::ParsePoint(const Json::Node& node) {
	return Svg::Point{
	node.AsArray().at(0).AsDouble(),
	node.AsArray().at(1).AsDouble()
	};
}

Svg::Color Painter::ParseColor(const Json::Node& node) {
	const auto& variant_color = node.GetBase();
	Svg::Color color;
	if (std::holds_alternative<std::string>(variant_color)) {
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

std::vector<Svg::Color> Painter::ParsePalette(const Json::Node& node) {
	const auto& colors = node.AsArray();
	std::vector<Svg::Color> palette;
	palette.reserve(colors.size());
	for (const auto& color : colors) {
		palette.push_back(ParseColor(color));
	}
	return palette;
}

std::vector<std::string> Painter::ParseVectorStrings(const Json::Node& node) {
	std::vector<std::string> result;
	for (const auto& str : node.AsArray()) {
		result.push_back(str.AsString());
	}
	return result;
}