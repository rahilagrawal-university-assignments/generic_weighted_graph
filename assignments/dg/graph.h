#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace gdwg {
using std::shared_ptr;
using std::weak_ptr;

template <typename N, typename E>
class Graph {
 public:
  class const_iterator {};

  Graph() = default;
  Graph(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator,
        typename std::vector<std::tuple<N, N, E>>::const_iterator);
  Graph(typename std::initializer_list<N>);
  Graph(const Graph&);
  Graph(Graph&&) = default;

  ~Graph() = default;

  Graph& operator=(const Graph&);
  Graph& operator=(Graph&&) = default;

  bool InsertNode(const N&);
  bool isNode(const N&) const;
  bool InsertEdge(const N&, const N&, const E&);
  shared_ptr<N> getNode(const N&) const;
  bool isEdge(const N&, const N&, const E&) const;

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g) {

    std::vector<shared_ptr<N>> nodes = g.nodes;
    std::vector<shared_ptr<Edge>> edges = g.edges;

    std::sort(nodes.begin(), nodes.end(),
              [](const shared_ptr<N>& a, const shared_ptr<N>& b) -> bool { return *a < *b; });

    for (auto node : nodes) {
      os << *node << " (\n";

      std::vector<shared_ptr<Edge>> edges = g.getOutEdges(*node);

      std::sort(edges.begin(), edges.end(),
                [](const shared_ptr<Edge>& edge1, const shared_ptr<Edge>& edge2) -> bool {
                  shared_ptr<N> destination1 = edge1->destination_.lock();
                  shared_ptr<N> destination2 = edge2->destination_.lock();

                  if (*destination1 == *destination2) {
                    return edge1->weight_ < edge2->weight_;
                  }
                  return *destination1 < *destination2;
                });
      for (auto edge : edges) {
        shared_ptr<N> destination1 = edge->destination_.lock();
        os << "\t" << *destination1 << " | " << edge->weight_ << std::endl;
      }

      os << ")\n";
    }

    return os;
  }

 private:
  struct Edge {
    Edge(shared_ptr<N> source, shared_ptr<N> destination, const E& weight)
      : source_{source}, destination_{destination}, weight_{weight} {}
    weak_ptr<N> source_;
    weak_ptr<N> destination_;
    E weight_;
  };

  std::vector<shared_ptr<Edge>> getOutEdges(const N& node) const {
    std::vector<shared_ptr<Edge>> nodeEdges;
    for (auto edge : edges) {
      shared_ptr<N> source = edge->source_.lock();
      if (*source == node) {
        nodeEdges.push_back(edge);
      }
    }

    return nodeEdges;
  }

  std::vector<shared_ptr<Edge>> getInEdges(const N& node) const {
    std::vector<shared_ptr<Edge>> nodeEdges;
    for (auto edge : edges) {
      shared_ptr<N> destination = edge->destination_.lock();
      if (*destination == node) {
        nodeEdges.push_back(edge);
      }
    }

    return nodeEdges;
  }

  std::vector<shared_ptr<N>> nodes;
  std::vector<shared_ptr<Edge>> edges;
};
#include "assignments/dg/graph.tpp"

}  // namespace gdwg

#endif  // ASSIGNMENTS_DG_GRAPH_H_
