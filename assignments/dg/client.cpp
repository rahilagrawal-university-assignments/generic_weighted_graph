#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;

  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you?", 3);

  // const auto xyz{g};

  std::cout << g << '\n';

  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }

  std::vector<int> vec{1, 2, 3, 4, 5};

  gdwg::Graph<int, int> g2{vec.begin(), vec.end()};

  std::cout << g2 << '\n';

  std::vector<std::tuple<std::string, std::string, int>> vecTuples{
      std::make_tuple("Bar", "Foo", 3.14)};
  gdwg::Graph<std::string, int> g3{vecTuples.begin(), vecTuples.end()};

  std::cout << g3 << '\n';

  gdwg::Graph<char, std::string> b{'a', 'b', 'x', 'y'};

  std::cout << b << '\n';

  gdwg::Graph<std::string, int> aCopy;

  aCopy = g;
  std::cout << (aCopy == g) << '\n';
  g.Replace("you?", "are");
  std::cout << g << '\n';

  gdwg::Graph<char, int> c;
  c.InsertNode('A');
  c.InsertNode('B');
  c.InsertNode('C');
  c.InsertNode('D');
  c.InsertEdge('A', 'B', 1);
  c.InsertEdge('A', 'C', 2);
  c.InsertEdge('A', 'D', 3);
  std::cout << c;
  c.MergeReplace('A', 'B');
  std::cout << "--------------------";
  std::cout << c;

  c.Clear();
  std::cout << "here" << c << "here\n";
  c.InsertNode('A');
  c.InsertNode('B');
  c.InsertNode('C');
  c.InsertNode('D');
  c.InsertEdge('A', 'B', 1);
  c.InsertEdge('B', 'C', 10);
  c.InsertEdge('A', 'C', 2);
  c.InsertEdge('A', 'D', 3);
  std::cout << c;
  std::cout << c.IsConnected('A', 'B');
  std::cout << c.IsConnected('B', 'B') << "\n";

  std::cout << c.GetNodes().size() << "\n";
  std::cout << c.GetConnected('A').size() << "\n";

  // c.erase('A', 'B', 1);
  // std::cout << c << "\n";

  auto it = c.find('A', 'B', 1);
  std::cout << std::get<0>(*it) << " -> " << std::get<1>(*it) << " - " << std::get<2>(*it) << "\n";
  it++;
  std::cout << std::get<0>(*it) << " -> " << std::get<1>(*it) << " - " << std::get<2>(*it) << "\n";
}