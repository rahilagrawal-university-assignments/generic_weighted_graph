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

  // ----------------------- Constructors ----------------------------

  // Default Constructor
  Graph() = default;

  // Construct from list of nodes
  Graph(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);

  // Construct from list of edges
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator,
        typename std::vector<std::tuple<N, N, E>>::const_iterator);

  // Construct from initialiser list of nodes
  Graph(typename std::initializer_list<N>);

  // Copy constructor
  Graph(const Graph&);

  // Default Move Constructor
  Graph(Graph&&) = default;

  // Default Destructor
  ~Graph() = default;

  // ----------------------- Operations ----------------------------

  // Copy Assignment
  Graph& operator=(const Graph&);

  // Default Move Assignment
  Graph& operator=(Graph&&) = default;

  // Helper Functions
  shared_ptr<N> getNode(const N&) const;
  bool isEdge(const N&, const N&, const E&) const;

  // ----------------------- Methods ----------------------------

  bool InsertNode(const N&);
  bool DeleteNode(const N&);
  bool Replace(const N&, const N&);
  void MergeReplace(const N&, const N&);
  bool IsNode(const N&) const;

  bool InsertEdge(const N&, const N&, const E&);

  // ----------------------- Friends ----------------------------

  // Equality Operator Overload
  friend bool operator==(const Graph& g1, const Graph& g2) {
    // Compare if nodes are the same
    if (g1.nodes.size() != g2.nodes.size())
      return false;

    for (auto nodeG1 : g1.nodes) {
      auto present = false;
      for (auto nodeG2 : g2.nodes) {
        if (*nodeG1 == *nodeG2) {
          present = true;
          break;
        }
      }
      if (present == false)
        return false;
    }

    // Compare if edges are the same
    if (g1.edges.size() != g2.edges.size())
      return false;

    for (auto edgeG1 : g1.edges) {
      auto present = false;
      for (auto edgeG2 : g2.edges) {
        shared_ptr<N> source1 = edgeG1->source_.lock();
        shared_ptr<N> destination1 = edgeG1->destination_.lock();
        shared_ptr<N> source2 = edgeG2->source_.lock();
        shared_ptr<N> destination2 = edgeG2->destination_.lock();
        if (*source1 == *source2 && *destination1 == *destination2 &&
            edgeG1->weight_ == edgeG2->weight_) {
          present = true;
          break;
        }
      }
      if (present == false)
        return false;
    }

    return true;
  }

  // Inequality Operator Overload
  friend bool operator!=(const Graph& g1, const Graph& g2) { return !(g1 == g2); }

  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {

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
