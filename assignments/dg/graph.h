#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace gdwg {
using std::shared_ptr;
using std::weak_ptr;

template <typename N, typename E>
class Graph {
 private:
  // Private struct for internal representation of an Edge
  struct Edge {
    // Edges have weak pointers to look at nodes and a weight of type E
    Edge(shared_ptr<N> source, shared_ptr<N> destination, const E& weight)
      : source_{source}, destination_{destination}, weight_{weight} {}
    weak_ptr<N> source_;
    weak_ptr<N> destination_;
    E weight_;
  };

  // Sort Comparator for the set of edges
  struct sortEdges {
    bool operator()(const shared_ptr<Edge>& edge1, const shared_ptr<Edge>& edge2) const {
      shared_ptr<N> source1 = edge1->source_.lock();
      shared_ptr<N> source2 = edge2->source_.lock();
      shared_ptr<N> destination1 = edge1->destination_.lock();
      shared_ptr<N> destination2 = edge2->destination_.lock();

      if (*source1 == *source2) {
        if (*destination1 == *destination2)
          return edge1->weight_ < edge2->weight_;
        return *destination1 < *destination2;
      }
      return *source1 < *source2;
    }
  };

  // Function to get Outgoing Edges from a Node
  std::set<shared_ptr<Edge>, sortEdges> getOutEdges(const N& node) const {
    std::set<shared_ptr<Edge>, sortEdges> nodeEdges;
    for (auto edge : edges_) {
      shared_ptr<N> source = edge->source_.lock();
      if (*source == node) {
        nodeEdges.insert(edge);
      }
    }

    return nodeEdges;
  }

  // Internal representation of the nodes and edges of a graph
  std::vector<shared_ptr<N>> nodes_;
  std::set<shared_ptr<Edge>, sortEdges> edges_;

 public:
  // ----------------------- Iterators ---------------------------

  // Const Iterator
  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<N, N, E>*;
    using difference_type = int;

    // Pointer deferencing for iterator
    reference operator*() const {
      shared_ptr<Edge> edge = *edge_itr_;
      shared_ptr<N> source = edge->source_.lock();
      shared_ptr<N> destination = edge->destination_.lock();
      return {*source, *destination, edge->weight_};
    }

    // Pre-incremement operator for iterator
    const_iterator operator++() {
      if (edge_itr_ != end_sentinel_) {
        ++edge_itr_;
      }
      return *this;
    }

    // Post increment for iterator
    const_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }

    // Pre decrement for iterator
    const_iterator operator--() {
      if (edge_itr_ != begin_sentinel_) {
        --edge_itr_;
      }
      return *this;
    }

    // Post Decrement for iterator
    const_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }

    // Equality operator for iterator
    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return lhs.edge_itr_ == rhs.edge_itr_;
    }

    // Inequality operator for iterator
    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::set<shared_ptr<Edge>>::const_iterator edge_itr_;
    typename std::set<shared_ptr<Edge>>::const_iterator begin_sentinel_;
    typename std::set<shared_ptr<Edge>>::const_iterator end_sentinel_;
    const_iterator(const decltype(edge_itr_)& edge_itr,
                   const decltype(begin_sentinel_)& begin_sentinel,
                   const decltype(end_sentinel_)& end_sentinel)
      : edge_itr_{edge_itr}, begin_sentinel_{begin_sentinel}, end_sentinel_{end_sentinel} {}

    friend class Graph;
  };

  // Const Reverse Iterator
  class const_reverse_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<N, N, E>*;
    using difference_type = int;

    reference operator*() const {
      shared_ptr<Edge> edge = *edge_itr_;
      shared_ptr<N> source = edge->source_.lock();
      shared_ptr<N> destination = edge->destination_.lock();
      return {*source, *destination, edge->weight_};
    }
    const_reverse_iterator operator++() {
      if (edge_itr_ != end_sentinel_) {
        ++edge_itr_;
      }
      return *this;
    }
    const_reverse_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }
    const_reverse_iterator operator--() {
      if (edge_itr_ != begin_sentinel_) {
        --edge_itr_;
      }
      return *this;
    }
    const_reverse_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }

    friend bool operator==(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs) {
      return lhs.edge_itr_ == rhs.edge_itr_;
    }

    friend bool operator!=(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::set<shared_ptr<Edge>>::const_reverse_iterator edge_itr_;
    typename std::set<shared_ptr<Edge>>::const_reverse_iterator begin_sentinel_;
    typename std::set<shared_ptr<Edge>>::const_reverse_iterator end_sentinel_;
    const_reverse_iterator(const decltype(edge_itr_)& edge_itr,
                           const decltype(begin_sentinel_)& begin_sentinel,
                           const decltype(end_sentinel_)& end_sentinel)
      : edge_itr_{edge_itr}, begin_sentinel_{begin_sentinel}, end_sentinel_{end_sentinel} {}

    friend class Graph;
  };

  // -------------------- Iterator Constructors ----------------------

  // Constructors for const_iterator
  const_iterator cbegin() const { return {edges_.cbegin(), edges_.cbegin(), edges_.cend()}; }
  const_iterator cend() const { return {edges_.cend(), edges_.cbegin(), edges_.cend()}; }
  const_iterator begin() { return {edges_.cbegin(), edges_.cbegin(), edges_.cend()}; }
  const_iterator end() { return {edges_.cend(), edges_.cbegin(), edges_.cend()}; }

  // Constructors for const__reverse_iterator
  const_reverse_iterator crbegin() const {
    return {edges_.crbegin(), edges_.crbegin(), edges_.crend()};
  }
  const_reverse_iterator crend() const {
    return {edges_.crend(), edges_.crbegin(), edges_.crend()};
  }
  const_reverse_iterator rbegin() { return {edges_.crbegin(), edges_.crbegin(), edges_.crend()}; }
  const_reverse_iterator rend() { return {edges_.crend(), edges_.crbegin(), edges_.crend()}; }

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
  Graph& operator=(const Graph&) noexcept;

  // Default Move Assignment
  Graph& operator=(Graph&&) = default;

  // Helper Functions
  shared_ptr<N> getNode(const N&) const noexcept;
  bool isEdge(const N&, const N&, const E&) const noexcept;

  // ----------------------- Methods ----------------------------

  // Method for inserting a new node into the graph if it doesnt already exist
  bool InsertNode(const N&) noexcept;

  // Method for deleting a node if it exists
  bool DeleteNode(const N&) noexcept;

  // Method for replacing a node with another if the old node exists and the new does not
  // Exception is thrown otherwise
  bool Replace(const N&, const N&);

  // Method for replacing the edges of a node with another node - both in the graph
  // Old node is deleted
  // New node should also be present
  // Exception is thrown otherwise
  void MergeReplace(const N&, const N&);

  // Method for clearing a graph
  // Deletes all nodes and edges
  void Clear() noexcept;

  // Method for checking if a node exists in the graph
  bool IsNode(const N&) const noexcept;

  // Method for checking if there is at least one edge from src to dest
  bool IsConnected(const N&, const N&) const;

  // Method for getting all the nodes of the graph
  std::vector<N> GetNodes() const noexcept;

  // Method for getting the nodes that have an edge from the given node
  // Throws exception if the node is not present in the graph
  std::vector<N> GetConnected(const N&) const;

  // Method for getting the weights of all the edges from src to dest in ascending order of weight
  // Throws exception if either node is not present in the graph
  std::vector<E> GetWeights(const N&, const N&) const;

  // Method for deleting a given edge if it exists
  bool erase(const N&, const N&, const E&) noexcept;

  // Method for inserting an edge if it does not already exist
  bool InsertEdge(const N&, const N&, const E&);

  // Method for returning an iterator to a given edge
  // Return end iterator if not found
  const_iterator find(const N&, const N&, const E&) const noexcept;

  // Method for erasing the edge at an iterator
  // return iterator to next element if successful
  // return end iterator otherwise
  const_iterator erase(const_iterator it) noexcept;

  // ----------------------- Friends ----------------------------

  // Equality Operator Overload
  friend bool operator==(const Graph& g1, const Graph& g2) {
    // Compare if nodes are the same
    if (g1.nodes_.size() != g2.nodes_.size())
      return false;

    for (auto nodeG1 : g1.nodes_) {
      auto present = false;
      for (auto nodeG2 : g2.nodes_) {
        if (*nodeG1 == *nodeG2) {
          present = true;
          break;
        }
      }
      if (present == false)
        return false;
    }

    // Compare if edges are the same
    if (g1.edges_.size() != g2.edges_.size())
      return false;

    for (auto edgeG1 : g1.edges_) {
      auto present = false;
      for (auto edgeG2 : g2.edges_) {
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

  // OutStream Operator Overload
  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    std::vector<shared_ptr<N>> nodes = g.nodes_;

    std::sort(nodes.begin(), nodes.end(),
              [](const shared_ptr<N>& a, const shared_ptr<N>& b) -> bool { return *a < *b; });

    for (auto node : nodes) {
      os << *node << " (\n";

      std::set<shared_ptr<Edge>, sortEdges> edges = g.getOutEdges(*node);

      for (auto edge : edges) {
        shared_ptr<N> destination1 = edge->destination_.lock();
        os << "  " << *destination1 << " | " << edge->weight_ << std::endl;
      }

      os << ")\n";
    }

    return os;
  }
};
#include "assignments/dg/graph.tpp"

}  // namespace gdwg

#endif  // ASSIGNMENTS_DG_GRAPH_H_
