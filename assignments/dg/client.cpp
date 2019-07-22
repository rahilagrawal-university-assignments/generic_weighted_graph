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

  std::cout << g << '\n';

  // gdwg::Graph<std::string, int> g2{g};

  // std::cout << g2 << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  // for (const auto& [from, to, weight] : g) {
  //   std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  // }

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
  std::cout << c;
}