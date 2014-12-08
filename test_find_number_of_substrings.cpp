#include <string>
#include <ostream>
#include <unordered_set>

#include "shafran_test_framework.h"
#include "suffix_tree.h"
#include "find_number_of_substrings.h"

void TestFindNumberOfSubstringsUnit() {
  {
    SuffixTree tree("");
    CheckEq(FindNumberOfSubstrings(tree), 0,
      "TestFindNumberOfSubstringsUnit: #1 wrong result");
  }

  {
    SuffixTree tree("a");

    CheckEq(FindNumberOfSubstrings(tree), 1,
      "TestFindNumberOfSubstringsUnit: #2 wrong result");
  }

  {
    SuffixTree tree("aaaaa");

    CheckEq(FindNumberOfSubstrings(tree), 5,
      "TestFindNumberOfSubstringsUnit: #3 wrong result");
  }

  {
    SuffixTree tree("aabb");

    CheckEq(FindNumberOfSubstrings(tree), 8,
      "TestFindNumberOfSubstringsUnit: #4 wrong result");
  }

  {
    SuffixTree tree("abc");

    CheckEq(FindNumberOfSubstrings(tree), 6,
      "TestFindNumberOfSubstringsUnit: #5 wrong result");
  }
}

int FindNumberOfSubstringExactlyRight(const std::string& string) {
  std::unordered_set<std::string > set;
  
  for (size_t length = 1; length <= string.size(); ++length) {
    for (size_t first_position = 0; first_position + length < string.size() + 1; 
         ++first_position) {
      set.insert(string.substr(first_position, length));
    }
  }

  return set.size();
}

void TestFindNumberOfSubstringsRandomTest() {
  srand(777);
  for (size_t i = 0; i < 25; ++i) {
    std::string string = GetRandomString(100, "abcdefgh");
    SuffixTree tree(string);

    CheckEq(FindNumberOfSubstrings(tree), FindNumberOfSubstringExactlyRight(string),
      "TestFindNumberOfSubstringsRandomTest: wrong result");
  }
}

void TestFindNumberOfSubstrings(std::ostream& out) {
  try {
    TestFindNumberOfSubstringsUnit();
    PrintOK("TestFindNumberOfSubstringsUnit", out);

    TestFindNumberOfSubstringsRandomTest();
    PrintOK("TestFindNumberOfSubstringsRandomTest", out);
  }
  catch (std::exception e) {
    out << e.what() << "\n";
  }
}
