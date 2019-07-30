/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "assignments/dg/graph.h"
#include "catch.h"

// ----------------------- Commonly Used Functions -----------------------

// Default Constructor
SCENARIO("Create a Graph using the Default Constructor") {
  GIVEN("A graph with default constructor") {
    gdwg::Graph<std::string, int> g;
    WHEN("size of graph is checked") {
      std::vector<std::string> nodes = g.GetNodes();
      THEN("size of graph should 0") { REQUIRE(nodes.size() == 0); }
    }
  }
}

// IsNode
SCENARIO("Check the membership of a Node using IsNode") {
  GIVEN("A graph with default constructor") {
    gdwg::Graph<std::string, int> g;
    WHEN("nodes are added") {
      g.InsertNode("a");
      g.InsertNode("b");
      THEN("IsNode a and IsNode b but not IsNode c") {
        REQUIRE(g.IsNode("a"));
        REQUIRE(g.IsNode("b"));
        REQUIRE(!g.IsNode("c"));
      }
    }
  }
}

// InsertNode
SCENARIO("Inserting a node into the graph") {
  GIVEN("A graph with default constructor") {
    gdwg::Graph<std::string, int> g;
    WHEN("nodes are added") {
      bool result1 = g.InsertNode("a");
      bool result2 = g.InsertNode("b");
      std::vector<std::string> nodes = g.GetNodes();
      THEN("graph should contain 2 nodes") {
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
        REQUIRE(result1 == true);
        REQUIRE(result2 == true);
      }
    }
    WHEN("duplicate nodes are added") {
      bool result1 = g.InsertNode("a");
      bool result2 = g.InsertNode("b");
      bool result3 = g.InsertNode("a");
      std::vector<std::string> nodes = g.GetNodes();
      THEN("duplicate node should not be added to the graph") {
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
        REQUIRE(result1 == true);
        REQUIRE(result2 == true);
        REQUIRE(result3 == false);
      }
    }
  }
}

// IsEdge
SCENARIO("Checking is an edge exists in the graph") {
  GIVEN("A graph with default constructor and 2 nodes and 1 edge") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertEdge("a", "b", 5);
    WHEN("IisEdge is called") {
      bool result = g.isEdge("a", "b", 5);
      THEN("result should be true") { REQUIRE(result == true); }
    }

    WHEN("IisEdge is called with non existent edge") {
      bool result = g.isEdge("a", "b", 10);
      THEN("result should be false") { REQUIRE(result == false); }
    }
  }
}

// InsertEdge
SCENARIO("Inserting an edge into the graph") {
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

    WHEN("edge with a non existent node is added") {
      REQUIRE_THROWS_WITH(
          g.InsertEdge("d", "b", 5),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
  }
}

// GetNodes
SCENARIO("Creating a vector of nodes in the graph") {
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

    WHEN("more nodes are added") {
      g.InsertNode("d");
      g.InsertNode("c");
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
SCENARIO("Checking if 2 graphs are equal") {
  GIVEN("Create 2 equal graphs and 1 not equal graph") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    h.InsertNode("a");
    h.InsertNode("b");
    gdwg::Graph<std::string, int> i;
    i.InsertNode("a");
    i.InsertNode("d");
    WHEN("equality is tested on 2 equal graphs") {
      bool result = (g == h);
      THEN("result should be true") { REQUIRE(result == true); }
    }

    WHEN("equality is tested on 2 not equal graphs") {
      bool result = (g == i);
      THEN("result should be false") { REQUIRE(result == false); }
    }
  }
}

// Inequality Operator Overload
SCENARIO("Checking if 2 graphs are not equal") {
  GIVEN("Create 2 equal graphs and 1 not equal graph") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    gdwg::Graph<std::string, int> h;
    h.InsertNode("a");
    h.InsertNode("b");
    gdwg::Graph<std::string, int> i;
    i.InsertNode("a");
    i.InsertNode("d");
    WHEN("inequality is tested on 2 equal graphs") {
      bool result = (g != h);
      THEN("result should be false") { REQUIRE(result == false); }
    }

    WHEN("inequality is tested on 2 not equal graphs") {
      bool result = (g != i);
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
SCENARIO("Calling cbegin iterator on a graph") {
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
SCENARIO("Calling cend iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("cend is called") {
      auto end = g.cend();
      end--;
      THEN("cend-- must equal the last edge in sorted order") {
        REQUIRE(std::get<0>(*end) == "b");
        REQUIRE(std::get<1>(*end) == "c");
        REQUIRE(std::get<2>(*end) == 5);
      }
      end--;
      THEN("end-- should equal now equal the cbegin") { REQUIRE(end == g.cbegin()); }
    }
  }
}

// begin
SCENARIO("Calling begin iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("begin is called") {
      auto start = g.begin();
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
      THEN("start++ should now equal end") { REQUIRE(start == g.end()); }
    }
  }
}

// end
SCENARIO("Calling end iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("end is called") {
      auto end = g.end();
      end--;
      THEN("end-- must equal the last node in sorted order") {
        REQUIRE(std::get<0>(*end) == "b");
        REQUIRE(std::get<1>(*end) == "c");
        REQUIRE(std::get<2>(*end) == 5);
      }
      end--;
      THEN("end-- should equal now equal the begin") { REQUIRE(end == g.begin()); }
    }
  }
}

// crbegin
SCENARIO("Calling crbegin iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("crbegin is called") {
      auto start = g.crbegin();
      THEN("start must equal last edge in sorted order") {
        REQUIRE(std::get<0>(*start) == "b");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 5);
      }
      start++;
      THEN("start++ should equal next edge") {
        REQUIRE(std::get<0>(*start) == "a");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 1);
      }
      start++;
      THEN("start++ should now equal crend") { REQUIRE(start == g.crend()); }
    }
  }
}

// crend
SCENARIO("Calling crend iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("crend is called") {
      auto end = g.crend();
      end--;
      THEN("crend-- must equal the last edge in sorted order") {
        REQUIRE(std::get<0>(*end) == "a");
        REQUIRE(std::get<1>(*end) == "c");
        REQUIRE(std::get<2>(*end) == 1);
      }
      end--;
      THEN("end should equal now equal the crbegin") { REQUIRE(end == g.crbegin()); }
    }
  }
}

// rbegin
SCENARIO("Calling rbegin iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("rbegin is called") {
      auto start = g.rbegin();
      THEN("start must equal last edge in sorted order") {
        REQUIRE(std::get<0>(*start) == "b");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 5);
      }
      start++;
      THEN("start++ should equal next edge") {
        REQUIRE(std::get<0>(*start) == "a");
        REQUIRE(std::get<1>(*start) == "c");
        REQUIRE(std::get<2>(*start) == 1);
      }
      start++;
      THEN("start++ should now equal rend") { REQUIRE(start == g.rend()); }
    }
  }
}

// rend
SCENARIO("Calling rend iterator on a graph") {
  GIVEN("A graph with default constructor and 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("rend is called") {
      auto end = g.rend();
      end--;
      THEN("crend-- must equal the last edge in sorted order") {
        REQUIRE(std::get<0>(*end) == "a");
        REQUIRE(std::get<1>(*end) == "c");
        REQUIRE(std::get<2>(*end) == 1);
      }
      end--;
      THEN("end should equal now equal the rbegin") { REQUIRE(end == g.rbegin()); }
    }
  }
}

// ----------------------- Operations ------------------------------

// Copy Assignment
SCENARIO("Create a Graph by copy assigning another graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("Graph is created using the copy assignment") {
      gdwg::Graph<std::string, int> newG = g;
      THEN("The 2 graphs are equal") { REQUIRE(g == newG); }
    }
  }
}

// Move Assignment
SCENARIO("Create a Graph by move assigning another graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "c", 1);
    WHEN("Graph is created using the move assignment") {
      gdwg::Graph<std::string, int> newG = std::move(g);
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

// ----------------------- Methods ---------------------------------

// DeleteNode
SCENARIO("Deleting a Node from a Graph") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("D", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("Node A is Deleted") {
      bool result = g.DeleteNode("A");
      THEN("A is actually deleted") { REQUIRE(result); }
      THEN("There are only 3 nodes now") { REQUIRE(g.GetNodes().size() == 3); }
      THEN("A is not a node") { REQUIRE(!g.IsNode("A")); }
      THEN("There are no edges that start or end in A") {
        for (auto edge = g.cbegin(); edge != g.cend(); edge++) {
          REQUIRE(std::get<0>(*edge) != "A");
          REQUIRE(std::get<1>(*edge) != "A");
        }
      }
    }
    WHEN("Node E is Deleted") {
      bool result = g.DeleteNode("E");
      THEN("E is NOT actually deleted") { REQUIRE(!result); }
      THEN("There are only 4 nodes still") { REQUIRE(g.GetNodes().size() == 4); }
    }
  }
}

// Replace
SCENARIO("Replacing a Node in a Graph") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("D", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("Node A is replaced with E") {
      bool replaced = g.Replace("A", "E");
      THEN("The node is actually replaced") { REQUIRE(replaced); }
      THEN("There are only 4 nodes still") { REQUIRE(g.GetNodes().size() == 4); }
      THEN("A is not a node") { REQUIRE(!g.IsNode("A")); }
      THEN("There are no edges that start or end in A") {
        for (auto edge = g.cbegin(); edge != g.cend(); edge++) {
          REQUIRE(std::get<0>(*edge) != "A");
          REQUIRE(std::get<1>(*edge) != "A");
        }
      }
      THEN("E is a node") { REQUIRE(g.IsNode("E")); }
      THEN("There is an edge from E->B(1) and D->E(4)") {
        REQUIRE(g.isEdge("E", "B", 1));
        REQUIRE(g.isEdge("D", "E", 4));
      }
    }
    WHEN("Node A is replaced with B") {
      bool replaced = g.Replace("A", "B");
      THEN("The node is not actually replaced") { REQUIRE(!replaced); }
      THEN("There are still 4 nodes") { REQUIRE(g.GetNodes().size() == 4); }
      THEN("B is a node") { REQUIRE(g.IsNode("B")); }
      THEN("A is a node") { REQUIRE(g.IsNode("A")); }
      THEN("There is an edge from A->B(1) and D->A(4)") {
        REQUIRE(g.isEdge("A", "B", 1));
        REQUIRE(g.isEdge("D", "A", 4));
      }
    }
    WHEN("Node F is rpelaced with B") {
      REQUIRE_THROWS_WITH(g.Replace("F", "B"),
                          "Cannot call Graph::Replace on a node that doesn't exist");
    }
  }
}

// MergeReplace
SCENARIO("Merge Replacing a Node in a Graph") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("D", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("Node A is replaced with B") {
      g.MergeReplace("A", "B");
      THEN("There are only 3 nodes now") { REQUIRE(g.GetNodes().size() == 3); }
      THEN("A is not a node") { REQUIRE(!g.IsNode("A")); }
      THEN("There are no edges that start or end in A") {
        for (auto edge = g.cbegin(); edge != g.cend(); edge++) {
          REQUIRE(std::get<0>(*edge) != "A");
          REQUIRE(std::get<1>(*edge) != "A");
        }
      }
      THEN("There is an edge from B->B(1) and D->A(4)") {
        REQUIRE(g.isEdge("B", "B", 1));
        REQUIRE(g.isEdge("D", "B", 4));
      }
    }
    WHEN("Node A is replaced with B") {
      g.MergeReplace("A", "B");
      THEN("There are only 3 nodes now") { REQUIRE(g.GetNodes().size() == 3); }
      THEN("A is not a node") { REQUIRE(!g.IsNode("A")); }
      THEN("There are no edges that start or end in A") {
        for (auto edge = g.cbegin(); edge != g.cend(); edge++) {
          REQUIRE(std::get<0>(*edge) != "A");
          REQUIRE(std::get<1>(*edge) != "A");
        }
      }
      THEN("There is an edge from B->B(1) and D->A(4)") {
        REQUIRE(g.isEdge("B", "B", 1));
        REQUIRE(g.isEdge("D", "B", 4));
      }
    }
    WHEN("There is an edge from A->A(1) and A->B(1)") {
      g.InsertEdge("A", "A", 1);
      AND_WHEN("Node A is replaced by Node B") {
        g.MergeReplace("A", "B");
        THEN("There are only 3 nodes now") { REQUIRE(g.GetNodes().size() == 3); }
        THEN("A is not a node") { REQUIRE(!g.IsNode("A")); }
        THEN("There are no edges that start or end in A") {
          for (auto edge = g.cbegin(); edge != g.cend(); edge++) {
            REQUIRE(std::get<0>(*edge) != "A");
            REQUIRE(std::get<1>(*edge) != "A");
          }
        }
        THEN("There is an edge from B->B(1) and D->A(4)") {
          REQUIRE(g.isEdge("B", "B", 1));
          REQUIRE(g.isEdge("D", "B", 4));
        }
        THEN("There is only one edge from B->B(1) (duplicate not added)") {
          REQUIRE(g.GetWeights("B", "B").size() == 1);
        }
      }
    }
    WHEN("Node E is rpelaced with B") {
      REQUIRE_THROWS_WITH(
          g.MergeReplace("E", "B"),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
    WHEN("Node B is rpelaced with E") {
      REQUIRE_THROWS_WITH(
          g.MergeReplace("B", "E"),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
  }
}

// Clear
SCENARIO("Clearing a Graph") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("D", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("The graph is cleared") {
      g.Clear();
      THEN("The graph is same as a new graph with no edges and no nodes") {
        gdwg::Graph<std::string, int> clearG;
        REQUIRE(g == clearG);
      }

      THEN("New nodes should be able to be inserted") {
        g.InsertNode("a");
        g.InsertNode("b");
        auto nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == "a");
        REQUIRE(nodes[1] == "b");
      }
    }
  }
}

// IsConnected
SCENARIO("Checking connectivity of 2 nodes in a Graph") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("D", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("The connectivity from A to B is Tested") {
      bool connected = g.IsConnected("A", "B");
      THEN("A and B are connected from A to B") { REQUIRE(connected); }
    }
    WHEN("The connectivity from B to A is Tested") {
      bool connected = g.IsConnected("B", "A");
      THEN("A and B are not connected from B to A") { REQUIRE(!connected); }
    }
    WHEN("The connectivity from E to A is Tested") {
      REQUIRE_THROWS_WITH(
          g.IsConnected("E", "A"),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
    WHEN("The connectivity from A to E is Tested") {
      REQUIRE_THROWS_WITH(
          g.IsConnected("A", "E"),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
  }
}

// GetConnected
SCENARIO("Getting nodes connected to a given node") {
  GIVEN("A graph with 4 edges and 4 nodes, 1 Node from A and 0 nodes from D") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("B", "C", 2), std::make_tuple("C", "D", 3),
        std::make_tuple("B", "A", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("The connected nodes of A are received") {
      auto connectedNodes = g.GetConnected("A");
      THEN("There is one node in the list and it is B") {
        REQUIRE(connectedNodes.size() == 1);
        REQUIRE(connectedNodes[0] == "B");
      }
    }
    WHEN("The connected nodes of D are received") {
      auto connectedNodes = g.GetConnected("D");
      THEN("There is no nodes in the list") { REQUIRE(connectedNodes.size() == 0); }
    }
    WHEN("The connected nodes of A are received") {
      REQUIRE_THROWS_WITH(g.GetConnected("E"),
                          "Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }
  }
}

// GetWeights
SCENARIO("Getting weights for edges between 2 given nodes") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("A", "B", -1), std::make_tuple("A", "B", 5),
        std::make_tuple("A", "B", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("The weights of edges from A->B are received") {
      auto weights = g.GetWeights("A", "B");
      THEN("There are 4 weights") { REQUIRE(weights.size() == 4); }
      THEN("The weights are sorted") {
        auto sortedWeights = weights;
        std::sort(sortedWeights.begin(), sortedWeights.end());
        REQUIRE(weights == sortedWeights);
      }
    }
    WHEN("The weights from A to E are received") {
      REQUIRE_THROWS_WITH(
          g.GetWeights("A", "E"),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
    WHEN("The weights from E to A are received") {
      REQUIRE_THROWS_WITH(
          g.GetWeights("E", "A"),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
  }
}

// erase
SCENARIO("Erase an edge from the internal representation") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("A", "B", 1), std::make_tuple("A", "B", -1), std::make_tuple("A", "B", 5),
        std::make_tuple("A", "B", 4)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("The edge A->B(1) is erased") {
      auto erased = g.erase("A", "B", 1);
      THEN("A->B(1) actually got deleted") { REQUIRE(erased); }
      THEN("A->B(1) does not exist anymore") { REQUIRE(!g.isEdge("A", "B", 1)); }
      THEN("A->B(-1) still exists") { REQUIRE(g.isEdge("A", "B", -1)); }
      THEN("A->B(5) still exists") { REQUIRE(g.isEdge("A", "B", 5)); }
      THEN("A->B(4) still exists") { REQUIRE(g.isEdge("A", "B", 4)); }
    }
    WHEN("The edge B->B(1) is erased") {
      auto erased = g.erase("B", "B", 1);
      THEN("B->B(1) did not delete because it does not exist") { REQUIRE(!erased); }
    }
  }
}

// Iterator find
SCENARIO("Finding a particular edge in the graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "b", 1);
    WHEN("find is called") {
      auto it = g.find("a", "b", 1);
      THEN("iterator should point to the edge from a to b with weight 1") {
        REQUIRE(std::get<0>(*it) == "a");
        REQUIRE(std::get<1>(*it) == "b");
        REQUIRE(std::get<2>(*it) == 1);
      }
    }

    WHEN("find is called for a edge that does not exist") {
      auto it = g.find("a", "c", 2);
      THEN("iterator should equal cend") { REQUIRE(it == g.cend()); }
    }
  }
}

// Iterator erase
SCENARIO("Erasing an edge using an iterator in the graph") {
  GIVEN("A graph with 3 nodes and 2 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertNode("c");
    g.InsertEdge("b", "c", 5);
    g.InsertEdge("a", "b", 1);
    auto it = g.find("a", "b", 1);
    WHEN("erase is called") {
      it = g.erase(it);
      THEN("edge should be erased and iterator should point to next edge") {
        REQUIRE(g.isEdge("a", "b", 1) == false);
        REQUIRE(std::get<0>(*it) == "b");
        REQUIRE(std::get<1>(*it) == "c");
        REQUIRE(std::get<2>(*it) == 5);
      }
    }

    WHEN("erase is called for a edge that does not exist") {
      auto it = g.find("a", "b", 2);
      it = g.erase(it);
      THEN("iterator should equal cend") { REQUIRE(it == g.cend()); }
    }
  }
}

// ----------------------- Friends ---------------------------------

// Outstream Operator Overload
SCENARIO("Print a graph using the output stream") {
  GIVEN("A graph with 4 edges and 4 nodes") {
    std::vector<std::tuple<std::string, std::string, int>> vecTuples{
        std::make_tuple("B", "D", 5), std::make_tuple("B", "C", -1), std::make_tuple("C", "D", 4),
        std::make_tuple("A", "B", 1)};
    gdwg::Graph<std::string, int> g{vecTuples.begin(), vecTuples.end()};
    WHEN("It is printed using the output stream") {
      std::stringstream v;
      v << g;
      THEN("Then it is printed in the following format:\n[node1][1 space](\n[2 "
           "spaces][node1_connected_node1][1 space][pipe][1 space][weight]\n[2 "
           "spaces][node1_connected_node2][1 space][pipe][1 space][weight]\n[etc "
           "etc]\n)\n[node2][1 space](\n[2 spaces][node2_connected_node1][1 space][pipe][1 "
           "space][weight]\n[2 spaces][node2_connected_node2][1 space][pipe][1 "
           "space][weight]\n[etc etc]\n)\n[node3][1 space](\n[2 spaces][node3_connected_node1][1 "
           "space][pipe][1 space][weight]\n[2 spaces][node3_connected_node2][1 space][pipe][1 "
           "space][weight]\n[etc etc]\n)\n[etc etc]\n"
           "sorted first by node, then by connected node and then by weight") {
        REQUIRE(v.str() == "A (\n  B | 1\n)\nB (\n  C | -1\n  D | 5\n)\nC (\n  D | 4\n)\nD (\n)\n");
      }
    }
  }
  GIVEN("An empty graph is created") {
    gdwg::Graph<std::string, int> g;
    WHEN("It is printed using the output stream") {
      std::stringstream v;
      v << g;
      THEN("Then the output is empty") { REQUIRE(v.str() == ""); }
    }
  }
}
