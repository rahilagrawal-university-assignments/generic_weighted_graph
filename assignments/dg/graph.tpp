// ----------------------- Constructors ----------------------------

template <typename N, typename E>
Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin,
                   typename std::vector<N>::const_iterator end) {
  while (begin != end) {
    InsertNode(*begin);
    begin++;
  }
}

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

template <typename N, typename E>
Graph<N, E>::Graph(typename std::initializer_list<N> nodes) {
  for (auto node : nodes)
    InsertNode(node);
}

template <typename N, typename E>
Graph<N, E>::Graph(const Graph& g) {
  for (auto node : g.nodes)
    InsertNode(*node);
  for (auto edge : g.edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    InsertEdge(*source, *destination, edge->weight_);
  }
}

// ----------------------- Operations ----------------------------

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(const Graph& g) noexcept {

  for (auto node : g.nodes)
    InsertNode(*node);
  for (auto edge : g.edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    InsertEdge(*source, *destination, edge->weight_);
  }

  return *this;
}

// ----------------------- Methods ----------------------------

template <typename N, typename E>
bool Graph<N, E>::IsNode(const N& val) const noexcept {
  for (auto node : nodes) {
    if (*node == val)
      return true;
  }
  return false;
}

template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N& val) noexcept {
  if (IsNode(val)) {
    return false;
  }
  nodes.push_back(std::make_shared<N>(val));
  return true;
}

template <typename N, typename E>
bool Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  if (isEdge(src, dst, w))
    return false;
  shared_ptr<N> source = getNode(src);
  shared_ptr<N> destination = getNode(dst);
  if (!source || !destination) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  edges.insert(std::make_shared<Edge>(source, destination, w));

  return true;
}

template <typename N, typename E>
bool Graph<N, E>::DeleteNode(const N& val) noexcept {
  for (auto nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++) {
    auto node = *nodeItr;
    if (*node == val) {
      // Remove from nodes
      nodes.erase(nodeItr);

      // Remove its edges
      for (auto edgeItr = edges.begin(); edgeItr != edges.end(); edgeItr++) {
        auto edge = *edgeItr;
        shared_ptr<N> source = edge->source_.lock();
        shared_ptr<N> destination = edge->destination_.lock();
        if (val == *source || val == *destination) {
          edgeItr = edges.erase(edgeItr);
          edgeItr--;
        }
      }
      return true;
    }
  }
  return false;
}

template <typename N, typename E>
bool Graph<N, E>::Replace(const N& oldData, const N& newData) {
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

template <typename N, typename E>
void Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  // Check if both nodes are present
  if (!IsNode(oldData) || !IsNode(newData)) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }

  // Change the edges
  for (auto edgeItr = edges.begin(); edgeItr != edges.end();) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();

    if (oldData == *source && oldData == *destination) {
      // Delete the edge
      edgeItr = edges.erase(edgeItr);
      // Add Edge if not already there
      InsertEdge(newData, newData, edge->weight_);
    } else if (oldData == *source) {
      edgeItr = edges.erase(edgeItr);
      InsertEdge(newData, *destination, edge->weight_);
    } else if (oldData == *destination) {
      edgeItr = edges.erase(edgeItr);
      InsertEdge(*source, newData, edge->weight_);
    } else {
      edgeItr++;
    }
  }

  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    std::cout << *source << *destination << edge->weight_ << "\n";
  }
  // Remove the oldData Node
  for (auto nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++) {
    auto node = *nodeItr;
    if (*node == oldData) {
      // Remove from nodes
      nodes.erase(nodeItr);
      break;
    }
  }
}

template <typename N, typename E>
void Graph<N, E>::Clear() noexcept {
  nodes.clear();
  edges.clear();
}

template <typename N, typename E>
bool Graph<N, E>::IsConnected(const N& src, const N& dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }

  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination)
      return true;
  }
  return false;
}

template <typename N, typename E>
std::vector<N> Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> results;
  for (auto node : nodes) {
    results.push_back(*node);
  }
  std::sort(results.begin(), results.end());
  return results;
}

template <typename N, typename E>
std::vector<N> Graph<N, E>::GetConnected(const N& src) const {
  if (!IsNode(src))
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");

  std::vector<N> results;
  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source)
      results.push_back(*destination);
  }
  std::sort(results.begin(), results.end());
  return results;
}

template <typename N, typename E>
std::vector<E> Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }

  std::vector<E> results;
  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination)
      results.push_back(edge->weight_);
  }
  std::sort(results.begin(), results.end());
  return results;
}

template <typename N, typename E>
bool Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {
  for (auto edgeItr = edges.begin(); edgeItr != edges.end(); edgeItr++) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_) {
      edges.erase(edgeItr);
      return true;
    }
  }
  return false;
}

template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::find(const N& src, const N& dst, const E& w) const
    noexcept {
  for (auto edgeItr = edges.begin(); edgeItr != edges.end(); edgeItr++) {
    auto edge = *edgeItr;
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_) {
      return {edgeItr, edges.cbegin(), edges.cend()};
    }
  }
  return {edges.cend(), edges.cbegin(), edges.cend()};
}

template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::erase(const_iterator it) noexcept {
  for (auto edgeItr = edges.begin(); edgeItr != edges.end(); edgeItr++) {
    if (edgeItr == it.edge_itr_) {
      edgeItr = edges.erase(edgeItr);
      return {edgeItr, edges.cbegin(), edges.cend()};
    }
  }
  return {edges.cend(), edges.cbegin(), edges.cend()};
}

// ----------------------- Helper Functions ----------------------------

template <typename N, typename E>
shared_ptr<N> Graph<N, E>::getNode(const N& val) const noexcept {
  for (auto node : nodes) {
    if (*node == val)
      return node;
  }
  return nullptr;
}

template <typename N, typename E>
bool Graph<N, E>::isEdge(const N& src, const N& dst, const E& w) const noexcept {
  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_)
      return true;
  }
  return false;
}