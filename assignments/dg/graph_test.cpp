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

// InsertNode

// InsertEdge

// GetNodes

// Equality Operator Overload

// Inequality Operator Overload

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

// Default Move Constructor

// Default Destructor

// ----------------------- Iterator Constructors -----------------------

// cbegin

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
