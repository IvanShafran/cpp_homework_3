#include <ostream>
#include <string>

#include "suffix_tree.h"
#include "build_suffix_array.h"
#include "shafran_test_framework.h"

void TestBuildSuffixArrayUnit() {
  {
    SuffixTree tree("a");

    CheckEq<std::vector<int>>(BuildSuffixArray(tree), { 0, },
      "TestBuildSuffixArrayUnit: wrong result #1");
  }

  {
    SuffixTree tree("ba");

    CheckEq<std::vector<int>>(BuildSuffixArray(tree), { 1, 0 },
      "TestBuildSuffixArrayUnit: wrong result #2");
  }

  {
    SuffixTree tree("abacaba");

    CheckEq<std::vector<int>>(BuildSuffixArray(tree), { 6, 4, 0, 2, 5, 1, 3 },
      "TestBuildSuffixArrayUnit: wrong result #3");
  }
}

void CheckSuffixArray(const std::string& string, const std::vector<int>& suffix_array) {
  int string_length = string.size();
  for (size_t i = 1; i < suffix_array.size(); ++i) {
    if (string.substr(suffix_array[i - 1]) > string.substr(suffix_array[i])) {
      ThrowException("TestBuildSuffixArrayRandom: wrong result");
    }
  }
}

void TestBuildSuffixArrayRandom() {
  srand(777);
  for (size_t i = 0; i < 100; ++i) {
    std::string string = GetRandomString(1000, "abcdefgh");
    SuffixTree tree(string);

    CheckSuffixArray(string, BuildSuffixArray(tree));
  }
}

void TestBuildSuffixArray(std::ostream& out) {
  try {
    TestBuildSuffixArrayUnit();
    PrintOK("TestBuildSuffixArray", out);

    TestBuildSuffixArrayRandom();
    PrintOK("TestBuildSuffixArrayRandom", out);
  }
  catch (std::exception e) {
    out << e.what() << "\n";
  }
}
