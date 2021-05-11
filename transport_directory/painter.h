#pragma once
#include "json.h"
#include "svg.h"
#include "sphere.h"
#include "descriptions.h"
#include "painter.pb.h"
#include "company.pb.h"
#include "utils.h"

#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <variant>

namespace Paint {

  struct Route {

    struct Bus {
      std::string bus_name;
      size_t start_stop_idx;
      size_t finish_stop_idx;
    };
    struct Walk {
      std::string stop_from;
      std::string company_name;
      std::string rubric_name;
    };
    template<typename ItemType>
    using Items = std::vector<ItemType>;

    Items<Bus> buses;
    Items<Walk> walks;
  };

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
    double company_radius;
    double company_line_width;
  };


  class Painter {
  public:
    Painter() = default;
    Painter(const Json::Dict& render_settings_json,
      std::shared_ptr<Descriptions::BusesDict> buses,
      std::shared_ptr<Descriptions::StopsDict> stops,
      const std::vector<YellowPages::Company>& companies);

    TCProto::Painter Serialize() const;
    Painter(const TCProto::Painter& painter);

    std::string Paint(std::optional<Route> = std::nullopt) const;
  private:
    const RenderSettings settings_;
    const std::shared_ptr<Descriptions::BusesDict> buses_dict_;
    const std::shared_ptr<Descriptions::StopsDict> stops_dict_;
    const std::map<std::string, Svg::Point> places_coords_;
    const std::unordered_map<std::string, Svg::Color> bus_colors_;

    const Svg::Document base_map_;

    void PaintMoveLines(Svg::Document& svg) const;
    void PaintMoveLines(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const;
    void PaintMoveLines(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const;

    void PaintMoveLabels(Svg::Document& svg) const;
    void PaintMoveLabels(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const;

    void PaintPlacePoints(Svg::Document& svg) const;
    void PaintPlacePoints(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const;
    void PaintPlacePoints(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const;

    void PaintPlaceLabels(Svg::Document& svg) const;
    void PaintPlaceLabels(Svg::Document& svg, const Route::Items<Route::Bus>& buses) const;
    void PaintPlaceLabels(Svg::Document& svg, const Route::Items<Route::Walk>& walks) const;

    void PaintMoveLabel(Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintPlaceLabel(Svg::Document& svg, Svg::Point pos, const std::string& name) const;
    void PaintPlacePoint(Svg::Document& svg, Svg::Point pos) const;
    Svg::Polyline PaintBaseLine(const std::string& bus_name) const;

    Svg::Document MakeDocument() const;

    static const std::unordered_map<
      std::string, void (Painter::*)(Svg::Document&) const
    > LAYER_ACTIONS;

    static const std::unordered_map<
      std::string, void (Painter::*)(Svg::Document&, const Route::Items<Route::Bus>&) const
    > MOVE_LAYER_ACTIONS;

    static const std::unordered_map<
      std::string, void (Painter::*)(Svg::Document&, const Route::Items<Route::Walk>&) const
    > WALK_LAYER_ACTIONS;
  };

}