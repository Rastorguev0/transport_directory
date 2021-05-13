#include "transport_catalog.h"
#include "transport_catalog.pb.h"
#include <memory>
#include <algorithm>
#include <cmath>

using namespace std;

TransportCatalog::TransportCatalog(vector<Descriptions::InputQuery> data,
  const Json::Dict& routing_settings_json,
  const Json::Dict& render_settings_json,
  const Json::Dict& yellow_pages_json) {

  auto stops_end = partition(begin(data), end(data), [](const auto& item) {
    return holds_alternative<Descriptions::Stop>(item);
    });

  auto stops_dict = make_shared<Descriptions::StopsDict>();
  for (const auto& item : Range{ begin(data), stops_end }) {
    const auto& stop = get<Descriptions::Stop>(item);
    (*stops_dict)[stop.name] = make_unique<Descriptions::Stop>(stop);
    stops_.insert({ stop.name, {} });
  }

  auto buses_dict = make_shared<Descriptions::BusesDict>();
  for (const auto& item : Range{ stops_end, end(data) }) {
    const auto& bus = get<Descriptions::Bus>(item);

    (*buses_dict)[bus.name] = make_unique<Descriptions::Bus>(bus);
    buses_[bus.name] = Bus{
      bus.stops.size(),
      ComputeUniqueItemsCount(AsRange(bus.stops)),
      ComputeRoadRouteLength(bus.stops, *stops_dict),
      ComputeGeoRouteDistance(bus.stops, *stops_dict)
    };

    for (const string& stop_name : bus.stops) {
      stops_.at(stop_name).bus_names.insert(bus.name);
    }
  }

  companies_ = make_unique<CompaniesCatalog>(
    yellow_pages_json.at("rubrics").AsMap(), yellow_pages_json.at("companies").AsArray()
    );
  router_ = make_unique<TransportRouter>(*stops_dict, *buses_dict, routing_settings_json);
  painter_ = make_unique<Paint::Painter>(render_settings_json, buses_dict, stops_dict, companies_->GetCompanies());
}


const TransportCatalog::Stop* TransportCatalog::GetStop(const string& name) const {
  return GetValuePointer(stops_, name);
}

const TransportCatalog::Bus* TransportCatalog::GetBus(const string& name) const {
  return GetValuePointer(buses_, name);
}

optional<TransportRouter::RouteInfo> TransportCatalog::FindRoute(
  const string& stop_from, const string& stop_to) const {
  return router_->FindRoute(stop_from, stop_to);
}

vector<string> TransportCatalog::FindCompanies(const CompanyQuery::Company& model) const {
  vector<string> result;
  auto companies = companies_->FindCompanies(model);
  result.reserve(companies.size());
  for (auto company : companies) {
    result.push_back(CompanyMainName(*company));
  }
  return result;
}

optional<TransportRouter::RouteInfo> TransportCatalog::RouteToCompany(
  const string& from, const double datetime, const CompanyQuery::Company& model) const
{
  optional<TransportRouter::RouteInfo> result;
  for (auto company : companies_->FindCompanies(model)) {
    for (const auto& stop : company->nearby_stops()) {
      auto route = router_->FindRoute(from, stop.name());
      if (route) {
        string company_name = CompanyMainName(*company);

        double walk_time = stop.meters() / (1000 * router_->GetWalkVelocity()) * 60; //in minutes
        route->total_time += walk_time;

        double cur_time = datetime + route->total_time;
        /* если попали на следующую неделю */
        cur_time = fmod(cur_time, 7.0 * 24.0 * 60.0);
        double wait_time = companies_->WaitingForOpen(cur_time, company_name);
        route->total_time += wait_time;

        TransportRouter::RouteInfo::WalkToCompany walk_item {
          .time = walk_time, .stop_from = stop.name(), .company_name = company_name
          };
        if (!company->rubrics().empty()) {
          walk_item.rubric = companies_->GetRubric(company->rubrics()[0]);
        }
        route->items.push_back(walk_item);
        if (wait_time != 0) {
          route->items.push_back(TransportRouter::RouteInfo::WaitCompany{wait_time, company_name});
        }
        if (!result.has_value() || route->total_time < result->total_time) {
          result = move(route);
        }
      }
    }
  }
  return result;
}

int TransportCatalog::ComputeRoadRouteLength(
  const vector<string>& stops,
  const Descriptions::StopsDict& stops_dict
) {
  int result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
  }
  return result;
}

double TransportCatalog::ComputeGeoRouteDistance(
  const vector<string>& stops,
  const Descriptions::StopsDict& stops_dict
) {
  double result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Sphere::Distance(
      stops_dict.at(stops[i - 1])->position, stops_dict.at(stops[i])->position
    );
  }
  return result;
}

std::string TransportCatalog::RenderMap() const {
  return painter_->Paint();
}

std::string TransportCatalog::RenderRoute(const Paint::Route& items) const {
  return painter_->Paint(items);
}