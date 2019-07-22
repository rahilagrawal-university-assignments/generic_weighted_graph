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

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {

  for (auto node : g.nodes)
    InsertNode(*node);
  for (auto edge : g.edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    InsertEdge(*source, *destination, edge->weight_);
  }

  return *this;
}

// template <typename N, typename E>
// Graph<N, E>& Graph<N, E>::operator=(Graph&& g) {
//   nodes.clear();
//   edges.clear();
//   nodes = std::move(g.nodes);
//   edges = std::move(g.edges);
//   return *this;
// }
template <typename N, typename E>
bool Graph<N, E>::isNode(const N& val) const {
  for (auto node : nodes) {
    if (*node == val)
      return true;
  }
  return false;
}

template <typename N, typename E>
shared_ptr<N> Graph<N, E>::getNode(const N& val) const {
  for (auto node : nodes) {
    if (*node == val)
      return node;
  }
  return nullptr;
}

template <typename N, typename E>
bool Graph<N, E>::isEdge(const N& src, const N& dst, const E& w) const {
  for (auto edge : edges) {
    shared_ptr<N> source = edge->source_.lock();
    shared_ptr<N> destination = edge->destination_.lock();
    if (src == *source && dst == *destination && w == edge->weight_)
      return true;
  }
  return false;
}

template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N& val) {
  if (isNode(val)) {
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
  edges.push_back(std::make_shared<Edge>(source, destination, w));

  return true;
}