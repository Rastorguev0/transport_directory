#pragma once

#include "descriptions.h"
#include "json.h"
#include "transport_router.h"
#include "painter.h"
#include "utils.h"
#include "sphere.h"
#include "companies_catalog.h"

#include <optional>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Responses {
	struct Stop {
		std::set<std::string> bus_names;
	};

	struct Bus {
		size_t stop_count = 0;
		size_t unique_stop_count = 0;
		int road_route_length = 0;
		double geo_route_length = 0.0;
	};
}

class TransportCatalog {
private:
	using Bus = Responses::Bus;
	using Stop = Responses::Stop;

public:
	TransportCatalog(std::vector<Descriptions::InputQuery> data,
		const Json::Dict& routing_settings_json,
		const Json::Dict& render_settings_json,
		const Json::Dict& yellow_pages_json);

	TransportCatalog(std::istream& is);
	void Serialize(std::ostream& os) const;

	const Stop* GetStop(const std::string& name) const;
	const Bus* GetBus(const std::string& name) const;

	std::optional<TransportRouter::RouteInfo> FindRoute(
		const std::string& stop_from, const std::string& stop_to) const;

	std::vector<std::string> FindCompanies(const CompanyQuery::Company& model) const;
	std::optional<TransportRouter::RouteInfo> RouteToCompany(
		const std::string& stop_from, const CompanyQuery::Company& model) const;

	std::string RenderMap() const;
	std::string RenderRoute(const Paint::Route& items) const;

private:
	static int ComputeRoadRouteLength(
		const std::vector<std::string>& stops,
		const Descriptions::StopsDict& stops_dict
	);

	static double ComputeGeoRouteDistance(
		const std::vector<std::string>& stops,
		const Descriptions::StopsDict& stops_dict
	);

	std::unordered_map<std::string, Stop> stops_;
	std::unordered_map<std::string, Bus> buses_;

	std::unique_ptr<TransportRouter> router_;
	std::unique_ptr<Paint::Painter> painter_;
	std::unique_ptr<CompaniesCatalog> companies_;
};
