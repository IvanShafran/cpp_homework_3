#include <string>
#include <vector>

#include "suffix_tree.h"
#include "shafran_test_framework.h"
#include "length_of_max_substring.h"

void TestGetLengthOfMaxSubstringUnit() {
  {
    CheckEq<std::vector<int>>(GetLengthOfMaxSubstring({ "abcd", "abc", "abe" }), {3, 2},
      "TestGetLengthOfMaxSubstringUnit: wrong result #1");
  }

  {
    CheckEq<std::vector<int>>(GetLengthOfMaxSubstring({ "abcd", "bcd", "cd", "d" }), 
      { 3, 2, 1 },
      "TestGetLengthOfMaxSubstringUnit: wrong result #2");
  }
}

void TestGetLengthOfMaxSubstring(std::ostream& out) {
  try {
    TestGetLengthOfMaxSubstringUnit();
    PrintOK("TestGetLengthOfMaxSubstringUnit", out);
  }
  catch (std::exception e) {
    out << e.what() << "\n";
  }
}