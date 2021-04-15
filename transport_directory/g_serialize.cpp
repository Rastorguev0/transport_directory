#include "graph.h"

using namespace std;

template<>
Serialize::BusGraph Graph::DirectedWeightedGraph<double>::Serialize() const {
  Serialize::BusGraph result;
  for (const auto& edge : edges_) {
    Serialize::Edge e;
    e.set_from(edge.from);
    e.set_to(edge.to);
    e.set_weight(edge.weight);
    *(result.add_edges_()) = e;
  }
  for (const auto& list : incidence_lists_) {
    Serialize::IncidenceList l;
    for (auto id : list) {
      l.add_edge_ids(id);
    }
    *(result.add_incidence_lists_()) = l;
  }
  return result;
}

template<>
Graph::DirectedWeightedGraph<double>::DirectedWeightedGraph(const Serialize::BusGraph& graph) {
  edges_.reserve(graph.edges__size());
  for (const auto& edge : graph.edges_()) {
    edges_.push_back({ edge.from(), edge.to(), edge.weight() });
  }
  incidence_lists_.reserve(graph.incidence_lists__size());
  for (const auto& list : graph.incidence_lists_()) {
    IncidenceList l;
    l.reserve(list.edge_ids_size());
    for (auto id : list.edge_ids()) {
      l.push_back(id);
    }
    incidence_lists_.push_back(move(l));
  }
}