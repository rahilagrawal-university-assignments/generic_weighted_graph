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
      THEN("size of graph should 0") {
        REQUIRE(nodes.size() == 0);
      }
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
      THEN("result should be true") {
        REQUIRE(result == true);
      }
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
      THEN("result should be false") {
        REQUIRE(result == false);
      }
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
      REQUIRE_THROWS_WITH(g.InsertEdge("d", "b", 5), "Cannot call Graph::InsertEdge when either src or dst node does not exist");
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
      THEN("result should be true") {
        REQUIRE(result == true);
      }
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
      THEN("result should be false") {
        REQUIRE(result == false);
      }
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
      THEN("result should be false") {
        REQUIRE(result == false);
      }
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
      THEN("result should be true") {
        REQUIRE(result == true);
      }
    }
  }
}

// ----------------------- Constructors ----------------------------

// Construct from list of nodes

// Construct from list of edges

// Construct from initialiser list of nodes

// Copy constructor

// Default Move Constructor

// Default Destructor

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
      THEN("start++ should now equal cend") {
        REQUIRE(start == g.cend());
      }
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
