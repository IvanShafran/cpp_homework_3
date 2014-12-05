#define SUFFIX_TREE_TESTING

#include "suffix_tree.h"
#include "shafran_test_framework.h"

#include <ostream>
#include <algorithm>


void TestSuffixTreeOnEmptyString() {
  try {
    SuffixTree tree("");
  }
  catch (std::exception e) {
    ThrowException("TestSuffixTreeSimpleEmptyString: didn't construct");
  }
}

void TestSuffixTreeGetString() {
  {
    SuffixTree tree("a");
    CheckEq<std::string>(tree.get_string(), "a$", 
      "TestSuffixTreeGetString: wrong result #1");
  }

  {
    std::string string;
    for (int i = -127; i < 128; ++i) {
      char c = i;
      string += c;
    }

    SuffixTree tree(string, "");
    CheckEq<std::string>(tree.get_string(), string, 
      "TestSuffixTreeGetString: wrong result #2");
  }
}

void TestSuffixTreeInitTree() {
  SuffixTree tree("");

  CheckEq(tree.tree_[tree.root_].suffix, tree.dummy_,
    "TestSuffixTreeInitTree: wrong suffix link from root");

  CheckEq<size_t>(tree.tree_[tree.dummy_].links.size(), 
    tree.GetSizeOfAlhpabet(), 
    "TestSuffixTreeInitTree: wrong number of links from dummy");

  for (size_t i = 0; i < tree.tree_[tree.dummy_].links.size(); ++i) {
    CheckEq(tree.tree_[tree.dummy_].links[i].incidence_vertex, tree.root_, 
      "TestSuffixTreeInitTree: wrong links from dummy");
  }
}

bool operator!=(const SuffixTree::Point& first, 
                const SuffixTree::Point& second) {
  return first.begin != second.begin || first.vertex != second.vertex;
}

void TestSuffixTreeCanonize() {
  SuffixTree tree("abac");

  {
    CheckEq(tree.Canonicalize(tree.root_, 1, 1), SuffixTree::Point(tree.root_, 1),
      "TestSuffixTreeCanonize: wrong result #1");
  }

  {
    CheckEq(tree.Canonicalize(tree.root_, 1, 3), SuffixTree::Point(tree.root_, 1),
      "TestSuffixTreeCanonize: wrong result #2");
  }

  {
    int start = 0;
    int end = 3;
    int vertex = tree.tree_[tree.root_].links[tree.get_edge_index(start)].incidence_vertex;
    CheckEq(tree.Canonicalize(tree.root_, start, end), SuffixTree::Point(vertex, 1),
      "TestSuffixTreeCanonize: wrong result #3");
  }
}

void TestSuffixTreeSplit() {
  {
    SuffixTree tree("");
    tree.tree_;

    int next_bracnhing_vertex;
    CheckEq(tree.Split(1, 1, 1, 0, &next_bracnhing_vertex), false,
      "TestSuffixTreeSplit: wrong returned bool value");

    CheckEq(next_bracnhing_vertex, 1,
      "TestSuffixTreeSplit: wrong next_bracnhing_vertex");
  }

  {
    SuffixTree tree("a");
    tree.tree_;

    int next_bracnhing_vertex;
    CheckEq(tree.Split(1, 1, 2, 0, &next_bracnhing_vertex), false,
     "TestSuffixTreeSplit: wrong returned bool value");

    CheckEq(next_bracnhing_vertex, 1,
      "TestSuffixTreeSplit: wrong next_bracnhing_vertex");
  }

  {
    SuffixTree tree("a");
    tree.tree_;

    int next_bracnhing_vertex;
    int vertex = 4;
    CheckEq(tree.Split(1, 1, 2, 1, &next_bracnhing_vertex), true,
      "TestSuffixTreeSplit: wrong returned bool value");


    CheckEq(next_bracnhing_vertex, vertex,
      "TestSuffixTreeSplit: wrong next_bracnhing_vertex");
  }
}

void TestSuffixTreeNewVertex() {
  SuffixTree tree("ab", "");
  
  CheckEq(tree.NewVertex(), 4, "TestSuffixTreeNewVertex: wrong returned value");
}

void TestSuffixTreeBuildAlphabet() {
  SuffixTree tree("abcd");

  CheckEq<std::string>(tree.alphabet_, "$abcd", "TestSuffixTreeBuildAlphabet: wrong alphabet");
}

void TestSuffixTreeGetSizeOfAlphabet() {
  SuffixTree tree("abcd");

  CheckEq<int>(tree.GetSizeOfAlhpabet(), 5, "TestSuffixTreeGetSizeOfAlphabet: wrong result size of alphabet");
}

void TestTreeTraversalLexicographicalOrder() {
  SuffixTree tree("cadb");

  class Visitor : public SuffixTreeVisitor {
   public:
    std::map<int, char> previous_edge_symbol;

    void BeforeVertexProcessing(int vertex) {
      previous_edge_symbol[vertex] = '\0';
    }

    void ProcessLink(int vertex, int incidence_vertex,
                     int begin_substring_index, int end_substring_index,
                     bool* do_transition) {
      char egde_first_symbol = (*suffix_tree_string_)[begin_substring_index];

      if (egde_first_symbol < previous_edge_symbol[vertex]) {
        ThrowException("TestTreeTraversalLexicographicalOrder: wrong order");
      }
      else {
        previous_edge_symbol[vertex] = egde_first_symbol;
      }

      *do_transition = true;
    }
  };

  Visitor visitor;

  tree.TreeTraversal(&visitor);
}

void TestSuffixTreeUnitTests(std::ostream& out) {
  try {
    TestSuffixTreeGetString();
    PrintOK("TestSuffixTreeGetString", out);

    TestSuffixTreeNewVertex();
    PrintOK("TestSuffixTreeNewVertex", out);

    TestSuffixTreeBuildAlphabet();
    PrintOK("TestSuffixTreeBuildAlphabet", out);

    TestSuffixTreeGetSizeOfAlphabet();
    PrintOK("TestSuffixTreeGetSizeOfAlphabet", out);

    TestSuffixTreeInitTree();
    PrintOK("TestSuffixTreeInitTree", out);

    TestSuffixTreeCanonize();
    PrintOK("TestSuffixTreeCanonize", out);

    TestSuffixTreeSplit();
    PrintOK("TestSuffixTreeSplit", out);

    TestSuffixTreeOnEmptyString();
    PrintOK("TestSuffixTreeOnEmptyString", out);

    TestTreeTraversalLexicographicalOrder();
    PrintOK("TestTreeTraversalLexicographicalOrder", out);
  }
  catch (std::exception e) {
    out << e.what() << "\n";
  }
}
