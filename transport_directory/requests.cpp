#include "requests.h"
#include "transport_router.h"
#include "companies_catalog.h"

#include <vector>

using namespace std;

namespace Requests {

  Json::Dict Stop::Process(const TransportCatalog& db) const {
    const auto* stop = db.GetStop(name);
    Json::Dict dict;
    if (!stop) {
      dict["error_message"] = Json::Node("not found"s);
    }
    else {
      vector<Json::Node> bus_nodes;
      bus_nodes.reserve(stop->bus_names.size());
      for (const auto& bus_name : stop->bus_names) {
        bus_nodes.emplace_back(bus_name);
      }
      dict["buses"] = Json::Node(move(bus_nodes));
    }
    return dict;
  }

  Json::Dict Bus::Process(const TransportCatalog& db) const {
    const auto* bus = db.GetBus(name);
    Json::Dict dict;
    if (!bus) {
      dict["error_message"] = Json::Node("not found"s);
    }
    else {
      dict = {
          {"stop_count", Json::Node(static_cast<int>(bus->stop_count))},
          {"unique_stop_count", Json::Node(static_cast<int>(bus->unique_stop_count))},
          {"route_length", Json::Node(bus->road_route_length)},
          {"curvature", Json::Node(bus->road_route_length / bus->geo_route_length)},
      };
    }
    return dict;
  }

  struct RouteItemResponseBuilder {
    Json::Dict operator()(const TransportRouter::RouteInfo::BusItem& bus_item) const {
      return Json::Dict{
          {"type", Json::Node("RideBus"s)},
          {"bus", Json::Node(bus_item.bus_name)},
          {"time", Json::Node(bus_item.time)},
          {"span_count", Json::Node(static_cast<int>(bus_item.span_count))}
      };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WaitItem& wait_item) const {
      return Json::Dict{
          {"type", Json::Node("WaitBus"s)},
          {"stop_name", Json::Node(wait_item.stop_name)},
          {"time", Json::Node(wait_item.time)},
      };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WalkToCompany& walk_item) const {
      return Json::Dict{
        {"type", Json::Node("WalkToCompany"s)},
        {"time", Json::Node(walk_item.time)},
        {"stop_name", Json::Node(walk_item.stop_from)},
        {"company", Json::Node(walk_item.company_name)},
      };
    }
  };

  struct RouteForPainterBuilder {
    Paint::RouteInfo::Item operator()(const TransportRouter::RouteInfo::BusItem& bus_item) {
      return Paint::RouteInfo::BusItem{
        bus_item.bus_name, bus_item.start_stop_idx, bus_item.finish_stop_idx
      };
    }
    Paint::RouteInfo::Item operator()(const TransportRouter::RouteInfo::WaitItem& wait_item) {
      return {};
    }
    Paint::RouteInfo::Item operator()(const TransportRouter::RouteInfo::WalkToCompany& walk_item) const {
      return Paint::RouteInfo::WalkItem{
        .stop_from = walk_item.stop_from,
        .company_name = walk_item.company_name,
        .rubric_name = walk_item.rubric
      };
    }
  };

  Json::Dict Route::Process(const TransportCatalog& db) const {
    Json::Dict dict;
    const auto route = db.FindRoute(stop_from, stop_to);
    if (!route) {
      dict["error_message"] = Json::Node("not found"s);
    }
    else {
      dict["total_time"] = Json::Node(route->total_time);
      vector<Json::Node> items;
      items.reserve(route->items.size());
      Paint::RouteInfo paint_info;

      for (const auto& item : route->items) {
        items.push_back(visit(RouteItemResponseBuilder{}, item));
        if (!holds_alternative<TransportRouter::RouteInfo::WaitItem>(item)) {
          paint_info.items.push_back(visit(RouteForPainterBuilder{}, item));
        }
      }

      dict["map"] = Json::Node{ db.RenderRoute(move(paint_info)) };
      dict["items"] = move(items);
    }

    return dict;
  }

  Json::Dict Map::Process(const TransportCatalog& db) const {
    return Json::Dict{ {"map", db.RenderMap()} };
  }

  Json::Dict FindCompanies::Process(const TransportCatalog& db) const {
    vector<Json::Node> companies;
    auto found = db.FindCompanies(model);
    companies.reserve(found.size());
    for (const auto& company : found) {
      companies.push_back(Json::Node{ company });
    }
    Json::Dict dict;
    dict["companies"] = move(companies);
    return dict;
  }

  Json::Dict RouteToCompany::Process(const TransportCatalog& db) const {
    const auto route = db.RouteToCompany(from, model);
    Json::Dict dict;
    if (!route) {
      dict["error_message"] = Json::Node("not found"s);
    }
    else {
      dict["total_time"] = Json::Node(route->total_time);
      vector<Json::Node> items;
      items.reserve(route->items.size());
      Paint::RouteInfo paint_info;

      for (const auto& item : route->items) {
        items.push_back(visit(RouteItemResponseBuilder{}, item));
        if (!holds_alternative<TransportRouter::RouteInfo::WaitItem>(item)) {
          paint_info.items.push_back(visit(RouteForPainterBuilder{}, item));
        }
      }

      dict["map"] = Json::Node{ db.RenderRoute(move(paint_info)) };
      dict["items"] = move(items);
    }
    return dict;
  }

  variant<Stop, Bus, Route, Map, FindCompanies, RouteToCompany> Read(const Json::Dict& attrs) {
    const string& type = attrs.at("type").AsString();
    if (type == "Bus") {
      return Bus{ attrs.at("name").AsString() };
    }
    else if (type == "Stop") {
      return Stop{ attrs.at("name").AsString() };
    }
    else if (type == "Route") {
      return Route{ attrs.at("from").AsString(), attrs.at("to").AsString() };
    }
    else if (type == "Map") {
      return Map{};
    }
    else if (type == "FindCompanies") {
      return FindCompanies{ .model = CompanyQuery::ReadCompany(attrs) };
    }
    else return RouteToCompany{
      .from = attrs.at("from").AsString(),
      .model = CompanyQuery::ReadCompany(attrs),
    };
  }

  vector<Json::Node> ProcessAll(const TransportCatalog& db, const vector<Json::Node>& requests) {
    vector<Json::Node> responses;
    responses.reserve(requests.size());
    for (const Json::Node& request_node : requests) {
      Json::Dict dict = visit([&db](const auto& request) {
        return request.Process(db);
        },
        Requests::Read(request_node.AsMap()));
      dict["request_id"] = Json::Node(request_node.AsMap().at("id").AsInt());
      responses.push_back(Json::Node(dict));
    }
    return responses;
  }

}
