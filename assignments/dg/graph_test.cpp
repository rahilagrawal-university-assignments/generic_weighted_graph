/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/dg/graph.h"
#include "catch.h"

// ----------------------- Commonly Used Functions -----------------------

// Default Constructor
SCENARIO("Testing Default Constructor") {
  GIVEN("A graph with default constructor") {
    gdwg::Graph<std::string, int> g;
    WHEN("size of graph is checked") {
      std::vector<std::string> nodes = g.GetNodes();
      THEN("size of graph should 0") { REQUIRE(nodes.size() == 0); }
    }
  }
}

// InsertNode
SCENARIO("Testing InsertNode") {
  GIVEN("A graph with default constructor") {
    gdwg::Graph<std::string, int> g;
    WHEN("nodes are added") {
      g.InsertNode("a");
      g.InsertNode("b");
      std::vector<std::string> nodes = g.GetNodes();
      THEN("graph should contain 2 nodes") {
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
      }
    }
  }
}

// IsEdge
SCENARIO("Testing isEdge - has the edge") {
  GIVEN("A graph with default constructor and 2 nodes and 1 edge") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertEdge("a", "b", 5);
    WHEN("IisEdge is called") {
      bool result = g.isEdge("a", "b", 5);
      THEN("result should be true") { REQUIRE(result == true); }
    }
  }
}

SCENARIO("Testing isEdge - does not have the edge") {
  GIVEN("A graph with default constructor and 2 nodes and 1 edge") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertEdge("a", "b", 5);
    WHEN("IisEdge is called with different edge") {
      bool result = g.isEdge("a", "b", 10);
      THEN("result should be false") { REQUIRE(result == false); }
    }
  }
}

// InsertEdge
SCENARIO("Testing InsertEdge") {
  GIVEN("A graph with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    WHEN("edge is added") {
      g.InsertEdge("a", "b", 5);
      THEN("graph should contain 1 edge from a to b with weight 5") {
        REQUIRE(g.isEdge("a", "b", 5) == true);
      }
    }
  }
}

SCENARIO("Testing InsertEdge - exception") {
  GIVEN("A graph with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    WHEN("edge is added") {
      REQUIRE_THROWS_WITH(
          g.InsertEdge("d", "b", 5),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
  }
}

// GetNodes
SCENARIO("Testing GetNodes has all nodes correctly entered") {
  GIVEN("A graph with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    WHEN("getNodes is called") {
      std::vector<std::string> nodes = g.GetNodes();
      THEN("nodes vector should have 2 nodes") {
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
      }
    }
  }
}

SCENARIO("Testing that GetNodes returns sorted order") {
  GIVEN("A graph with default constructor and 4 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertNode("a");
    g.InsertNode("d");
    WHEN("getNodes is called") {
      std::vector<std::string> nodes = g.GetNodes();
      THEN("nodes vector should have 4 nodes in sorted order") {
        REQUIRE(nodes.size() == 4);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
        REQUIRE(nodes[2] == "c");
        REQUIRE(nodes[3] == "d");
      }
    }
  }
}

// Equality Operator Overload
SCENARIO("Testing == that is true") {
  GIVEN("2 graphs with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    h.InsertNode("a");
    h.InsertNode("b");
    WHEN("equality is tested") {
      bool result = (g == h);
      THEN("result should be true") { REQUIRE(result == true); }
    }
  }
}

SCENARIO("Testing == that is false") {
  GIVEN("2 graphs with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    g.InsertNode("d");
    g.InsertNode("b");
    WHEN("equality is tested") {
      bool result = (g == h);
      THEN("result should be false") { REQUIRE(result == false); }
    }
  }
}

// Inequality Operator Overload
SCENARIO("Testing != that is false") {
  GIVEN("2 graphs with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    h.InsertNode("a");
    h.InsertNode("b");
    WHEN("inequality is tested") {
      bool result = (g != h);
      THEN("result should be false") { REQUIRE(result == false); }
    }
  }
}

SCENARIO("Testing != that is true") {
  GIVEN("2 graphs with default constructor and 2 nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    g.InsertNode("d");
    g.InsertNode("b");
    WHEN("inequality is tested") {
      bool result = (g != h);
      THEN("result should be true") { REQUIRE(result == true); }
    }
  }
}

// ----------------------- Constructors ----------------------------

// Construct from vector of nodes
SCENARIO("Create a Graph from the given vector of nodes") {
  GIVEN("A vector of 5 int nodes") {
    std::vector<int> vec{1, 2, 3, 4, 5};
    WHEN("Graph is created using the begin and end iterators") {
      gdwg::Graph<int, int> g{vec.begin(), vec.end()};
      THEN("The nodes are the same as given by the original vector") {
        REQUIRE(g.GetNodes() == vec);
      }
      THEN("There are no edges in the graph") { REQUIRE(g.cbegin() == g.cend()); }
    }
  }
}

// Construct from list of edges
SCENARIO("Create a Graph from the given vector of edges") {
  GIVEN("A vector of 5 int nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "C", 1), std::make_tuple("A", "B", 0), std::make_tuple("B", "C", 0),
        std::make_tuple("B", "A", 4)};
    WHEN("Graph is created using the begin and end iterators") {
      gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
      THEN("The edges are the sorted and  as given by the original vector") {
        auto edge = g.cbegin();
        REQUIRE(std::get<0>(*edge) == "A");
        REQUIRE(std::get<1>(*edge) == "B");
        REQUIRE(std::get<2>(*edge) == 0);
        edge++;
        REQUIRE(std::get<0>(*edge) == "A");
        REQUIRE(std::get<1>(*edge) == "C");
        REQUIRE(std::get<2>(*edge) == 1);
        edge++;
        REQUIRE(std::get<0>(*edge) == "B");
        REQUIRE(std::get<1>(*edge) == "A");
        REQUIRE(std::get<2>(*edge) == 4);
        edge++;
        REQUIRE(std::get<0>(*edge) == "B");
        REQUIRE(std::get<1>(*edge) == "C");
        REQUIRE(std::get<2>(*edge) == 0);
        edge++;
        REQUIRE(edge == g.cend());
      }
    }
  }
}

// Construct from initialiser list of nodes
SCENARIO("Create a Graph from the given initiliaser list of nodes") {
  GIVEN("An initiliaser list of 5 int nodes") {
    auto initialiser_list = {1, 2, 3, 4, 5};
    WHEN("Graph is created using the list") {
      gdwg::Graph<int, int> g{initialiser_list};
      THEN("The nodes are the same as given by the initialiser list") {
        REQUIRE(g.GetNodes() == std::vector<int>{1, 2, 3, 4, 5});
      }
      THEN("There are no edges in the graph") { REQUIRE(g.cbegin() == g.cend()); }
    }
  }
}

// Copy constructor
SCENARIO("Create a Graph by copy constructing another graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("Graph is created using the copy constructor") {
      gdwg::Graph<std::string, int> newG{g};
      THEN("The 2 graphs are equal") { REQUIRE(g == newG); }
    }
  }
}

// Default Move Constructor
SCENARIO("Create a Graph by move constructing another graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("Graph is created using the move constructor") {
      gdwg::Graph<std::string, int> newG{std::move(g)};
      THEN("The new graph has 3 nodes and 2 edges") {
        REQUIRE(newG.IsNode("a"));
        REQUIRE(newG.IsNode("b"));
        REQUIRE(newG.IsNode("c"));
        REQUIRE(newG.isEdge("b", "c", 5));
        REQUIRE(newG.isEdge("a", "c", 1));
      }
      THEN("The old graph has 0 nodes and 0 edges") {
        REQUIRE(g.GetNodes().size() == 0);
        REQUIRE(g.cbegin() == g.cend());
      }
    }
  }
}

// ----------------------- Iterator Constructors -----------------------

// cbegin
SCENARIO("Testing cbegin") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("cbegin is called") {
      auto start = g.cbegin();
      THEN("start must equal first edge in sorted order") {
        REQUIRE(std::get<0>(*start) == "a");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 1);
      }
      start++;
      THEN("start++ should equal next edge") {
        REQUIRE(std::get<0>(*start) == "b");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 5);
      }
      start++;
      THEN("start++ should now equal cend") { REQUIRE(start == g.cend()); }
    }
  }
}

// cend

// begin

// end

// crbegin

// crend

// rbegin

// rend

// ----------------------- Operations ------------------------------

// Copy Assignment

// Default Move Assignment

// ----------------------- Methods ---------------------------------

// DeleteNode

// Replace

// MergeReplace

// Clear

// IsNode

// IsConnected

// GetConnected

// GetWeights

// erase

// Iterator find

// Iterator erase

// ----------------------- Friends ---------------------------------

// Outstream Operator Overload
