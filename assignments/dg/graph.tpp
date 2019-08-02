// ----------------------- Constructors ----------------------------
// Constructor that takes in vector of nodes
template <typename N, typename E>
Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin,
                   typename std::vector<N>::const_iterator end) {
  while (begin != end) {
    InsertNode(*begin);
    begin++;
  }
}

// Constructor that takes in a vector of tuples
template <typename N, typename E>
Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                   typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  while (begin != end) {
    InsertNode(std::get<0>(*begin));
    InsertNode(std::get<1>(*begin));
    InsertEdge(std::get<0>(*begin), std::get<1>(*begin), std::get<2>(*begin));
    begin++;
  }
}

// Constructor that takes in a initializer list of nodes
template <typename N, typename E>
Graph<N, E>::Graph(typename std::initializer_list<N> nodes) {
  for (auto node : nodes)
    InsertNode(node);
}

// Copy Constructor
template <typename N, typename E>
Graph<N, E>::Graph(const Graph &g) {
  for (auto node : g.nodes_)
    InsertNode(*node);
  for (auto edge : g.edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    InsertEdge(*source, *destination, edge->weight_);
  }
}

// ----------------------- Operations ----------------------------
// Copy Assignment Operator
template <typename N, typename E>
Graph<N, E> &Graph<N, E>::operator=(const Graph &g) noexcept {
  for (auto node : g.nodes_)
    InsertNode(*node);
  for (auto edge : g.edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    InsertEdge(*source, *destination, edge->weight_);
  }

  return *this;
}

// ----------------------- Methods ----------------------------
// Check is a particular node is in the graph
template <typename N, typename E>
bool Graph<N, E>::IsNode(const N &val) const noexcept {
  for (auto node : nodes_) {
    if (*node == val)
      return true;
  }
  return false;
}

// Inserts a node into the graph
template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N &val) noexcept {
  if (IsNode(val)) {
    return false;
  }
  nodes_.push_back(std::make_shared<N>(val));
  return true;
}

// Inserts an edge into the graph
template <typename N, typename E>
bool Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {
  if (isEdge(src, dst, w))
    return false;
  shared_ptr<N> source = getNode(src);
  shared_ptr<N> destination = getNode(dst);
  if (!source || !destination) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  edges_.insert(std::make_shared<Edge>(source, destination, w));

  return true;
}

// Deletes a node from the graph
template <typename N, typename E>
bool Graph<N, E>::DeleteNode(const N &val) noexcept {
  for (auto nodeItr = nodes_.begin(); nodeItr != nodes_.end(); nodeItr++) {
    auto node = *nodeItr;
    if (*node == val) {
      // Remove from nodes
      nodes_.erase(nodeItr);

      // Remove its edges
      for (auto edgeItr = edges_.begin(); edgeItr != edges_.end(); edgeItr++) {
        auto edge = *edgeItr;
        shared_ptr<N> source = edge->source_.lock();
        shared_ptr<N> destination = edge->destination_.lock();
        if (val == *source || val == *destination) {
          edgeItr = edges_.erase(edgeItr);
          edgeItr--;
        }
      }
      return true;
    }
  }
  return false;
}

// Replaces oldData by the newData
template <typename N, typename E>
bool Graph<N, E>::Replace(const N &oldData, const N &newData) {
  // Check if oldData is present
  if (!IsNode(oldData)) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }

  // Check if newData is present
  if (IsNode(newData)) {
    return false;
  }

  // Okay to change
  auto changeNode = getNode(oldData);
  *changeNode = newData;

  return true;
}

// All instances of oldData is replaced by newData
// Every incoming and outgoing edge of oldData becomes an incoming/ougoing edge of newData
template <typename N, typename E>
void Graph<N, E>::MergeReplace(const N &oldData, const N &newData) {
  // Check if both nodes are present
  if (!IsNode(oldData) || !IsNode(newData)) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }

  // Change the edges
  for (auto edgeItr = edges_.begin(); edgeItr != edges_.end();) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();

    if (oldData == *source && oldData == *destination) {
      // Delete the edge
      edgeItr = edges_.erase(edgeItr);
      // Add Edge if not already there
      InsertEdge(newData, newData, edge->weight_);
    } else if (oldData == *source) {
      edgeItr = edges_.erase(edgeItr);
      InsertEdge(newData, *destination, edge->weight_);
    } else if (oldData == *destination) {
      edgeItr = edges_.erase(edgeItr);
      InsertEdge(*source, newData, edge->weight_);
    } else {
      edgeItr++;
    }
  }

  // Remove the oldData Node
  for (auto nodeItr = nodes_.begin(); nodeItr != nodes_.end(); nodeItr++) {
    auto node = *nodeItr;
    if (*node == oldData) {
      // Remove from nodes
      nodes_.erase(nodeItr);
      break;
    }
  }
}

// Clears the entire graph
template <typename N, typename E>
void Graph<N, E>::Clear() noexcept {
  nodes_.clear();
  edges_.clear();
}

// Checks if 2 nodes are connected by an edge
template <typename N, typename E>
bool Graph<N, E>::IsConnected(const N &src, const N &dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }

  for (auto edge : edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination)
      return true;
  }
  return false;
}

// Gets all nodes of the graph
template <typename N, typename E>
std::vector<N> Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> results;
  for (auto node : nodes_) {
    results.push_back(*node);
  }
  std::sort(results.begin(), results.end());
  return results;
}

// Gets all edges of a particular node
template <typename N, typename E>
std::vector<N> Graph<N, E>::GetConnected(const N &src) const {
  if (!IsNode(src))
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");

  std::vector<N> results;
  for (auto edge : edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source)
      results.push_back(*destination);
  }
  std::sort(results.begin(), results.end());
  return results;
}

// Get the weights of all edges connecting src and dst
template <typename N, typename E>
std::vector<E> Graph<N, E>::GetWeights(const N &src, const N &dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  std::vector<E> results;
  for (auto edge : edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination)
      results.push_back(edge->weight_);
  }
  std::sort(results.begin(), results.end());
  return results;
}

// Erases a edge from the graph
template <typename N, typename E>
bool Graph<N, E>::erase(const N &src, const N &dst, const E &w) noexcept {
  for (auto edgeItr = edges_.begin(); edgeItr != edges_.end(); edgeItr++) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_) {
      edges_.erase(edgeItr);
      return true;
    }
  }
  return false;
}

// Finds a particular edge in the graph and returns it as an iterator
template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::find(const N &src, const N &dst, const E &w) const
    noexcept {
  for (auto edgeItr = edges_.begin(); edgeItr != edges_.end(); edgeItr++) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_) {
      return {edgeItr, edges_.cbegin(), edges_.cend()};
    }
  }
  return {edges_.cend(), edges_.cbegin(), edges_.cend()};
}

// Erases an edge from the graph and returns an iterator to the next edge
template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::erase(const_iterator it) noexcept {
  for (auto edgeItr = edges_.begin(); edgeItr != edges_.end(); edgeItr++) {
    if (edgeItr == it.edge_itr_) {
      edgeItr = edges_.erase(edgeItr);
      return {edgeItr, edges_.cbegin(), edges_.cend()};
    }
  }
  return {edges_.cend(), edges_.cbegin(), edges_.cend()};
}

// ----------------------- Helper Functions ----------------------------
// Returns a shared_ptr to a particular node
template <typename N, typename E>
shared_ptr<N> Graph<N, E>::getNode(const N &val) const noexcept {
  for (auto node : nodes_) {
    if (*node == val)
      return node;
  }
  return nullptr;
}

// Returhs true if there is an edge from src to dst with weight w
template <typename N, typename E>
bool Graph<N, E>::isEdge(const N &src, const N &dst, const E &w) const noexcept {
  for (auto edge : edges_) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_)
      return true;
  }
  return false;
}
