#pragma once
#include "json.h"

#include <random>
#include <unordered_set>
#include <unordered_map>
using namespace std;
using namespace Json;

class Generator {
public:
	Generator(int stop_amount, int bus_amount, int stops_on_route, int query_amount)
		: stops_amount(stop_amount), buses_amount(bus_amount), stops_on_route(stops_on_route),
		query_amount(query_amount) {}

	double RandomDouble(double left, double right);

	string GenerateBusName();
	string GenerateRandomStopName();
	vector<Node> GenerateRoute();
	Dict GenerateBusStat();
	Dict GenerateStopStat();
	Dict GenerateRouteStat();
	Dict GenerateBaseStop(const string& stop);
	Dict GenerateBaseBus();
	vector<Node> GenerateBase();
	vector<Node> GenerateStats();
	Dict GenerateBigData();
private:
	mt19937 rnd = default_random_engine{};
	const int stops_amount;
	const int buses_amount;
	const int stops_on_route;
	const int query_amount;

	unordered_map<string, unordered_set<string>> stops_;
	unordered_set<string> buses_;
};

double Generator::RandomDouble(double left, double right) {
	uniform_real_distribution<> distr(left, right);
	return distr(rnd);
}

Dict Generator::GenerateRouteStat() {
	Dict dict;
	auto from = stops_.begin();
	auto to = prev(stops_.end());
	dict["type"] = Node("Route"s);
	dict["id"] = Node(static_cast<int>(rnd() % 1'000'000'000));
	dict["from"] = from->first;
	dict["to"] = to->first;
	return dict;
}

Dict Generator::GenerateStopStat() {
	Dict dict;
	auto it = stops_.begin();
	dict["name"] = it->first;
	dict["type"] = Node("Stop"s);
	dict["id"] = Node(static_cast<int>(rnd() % 1'000'000'000));
	return dict;
}

Dict Generator::GenerateBusStat() {
	auto it = buses_.begin();
	string name = *it;
	Dict dict;
	dict["type"] = Node("Bus"s);
	dict["name"] = move(name);
	dict["id"] = Node(static_cast<int>(rnd() % 1'000'000'000));
	return dict;
}

string Generator::GenerateBusName() {
	static unsigned cur = 1'000'000'000u;
	string bus = to_string(cur++);
	buses_.insert(bus);
	return bus;
}

string Generator::GenerateRandomStopName() {
	if (static_cast<int>(stops_.size()) < stops_amount) {
		string base = "   abcdefijlmnoqstuwxyz";
		shuffle(base.begin(), base.end(), default_random_engine{});
		stops_[base];
		return base;
	}
	else {
		const int index = rnd() % stops_amount;
		auto it = stops_.begin();
		for (int i = 0; i < index; i++) {
			it = next(it);
		}
		return it->first;
	}
}

vector<Node> Generator::GenerateRoute() {
	vector<Node> stops;
	const int stops_count = rnd() % (stops_on_route - 10) + 10;
	for (int i = 0; i < stops_count; i++) {
		stops.push_back(Node(string(GenerateRandomStopName())));
	}
	for (int i = 1; i + 1 < static_cast<int>(stops.size()); i++) {
		stops_[stops[i].AsString()].insert(stops[i - 1].AsString());
		stops_[stops[i].AsString()].insert(stops[i + 1].AsString());
	}
	return stops;
}

Dict Generator::GenerateBaseStop(const string& stop) {
	Dict dict;
	dict["type"] = Node("Stop"s);
	dict["name"] = Node(string(stop));
	dict["latitude"] = RandomDouble(42.5, 44.5);
	dict["longitude"] = RandomDouble(38.5, 40.5);
	Dict distances;
	for (const string& neighbour : stops_[stop]) {
		distances[neighbour] = 3000;
	}
	dict["road_distances"] = move(distances);
	return dict;
}

Dict Generator::GenerateBaseBus() {
	Dict dict;
	dict["type"] = Node("Bus"s);
	dict["name"] = Node(string(GenerateBusName()));
	dict["is_roundtrip"] = false;
	dict["stops"] = GenerateRoute();
	return dict;
}

vector<Node> Generator::GenerateStats() {
	vector<Node> stats;
	for (int i = 0; i < query_amount / 3; i++) {
		stats.push_back(GenerateBusStat());
		stats.push_back(GenerateStopStat());
		stats.push_back(GenerateRouteStat());
	}
	return stats;
}

vector<Node> Generator::GenerateBase() {
	vector<Node> base;
	for (int i = 0; i < buses_amount; i++) {
		base.push_back(GenerateBaseBus());
	}
	for (const auto& [stop, _] : stops_) {
		base.push_back(GenerateBaseStop(stop));
	}
	return base;
}

Dict Generator::GenerateBigData() {
	Json::Dict dict;
	dict["routing_settings"] = Node(Dict{ {"bus_wait_time", Node(2)}, {"bus_velocity", Node(30)} });
	dict["render_settings"] = Node(Dict{
		{"width", Node(1200)},
		{"height", Node(1200)},
		{"padding", Node(50)},
		{"stop_radius", Node(5)},
		{"line_width", Node(14)},
		{"bus_label_font_size", Node(14)},
		{"bus_label_offset", Node(vector<Node>{Node(-7), Node(3)})},
		{"stop_label_font_size", Node(20)},
		{"stop_label_offset", Node(vector<Node>{Node(7), Node(-3)})},
		{"underlayer_color", Node(vector<Node>{Node(255), Node(255), Node(255), Node(0.85)})},
		{"underlayer_width", Node(3)},
		{"color_palette", Node(vector<Node>{Node("green"s), Node("orange"s), Node("red"s)})},
		{"layers", Node(vector<Node>{
						Node("bus_lines"s),
						Node("bus_labels"s),
						Node("stop_points"s),
						Node("stop_labels"s)
				})
		}
			});
	dict["base_requests"] = GenerateBase();
	vector<Node> stats = [this] {
		auto result = GenerateStats();
		result.push_back(Dict{ {"id", Node(1111111111)}, {"type", Node("Map"s)} });
		result.push_back(Dict{ {"id", Node(1111111112)}, {"type", Node("Map"s)} });
		return move(result);
	}();
	dict["stat_requests"] = move(stats);
	return dict;
}