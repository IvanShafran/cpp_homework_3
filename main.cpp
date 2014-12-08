#pragma once

#include <iostream>

#include "test_suffix_tree.h"
#include "test_find_all_occurences.h"
#include "test_find_number_of_substrings.h"
#include "test_build_suffix_array.h"
#include "test_length_of_max_substring.h"

int main() {
  TestSuffixTreeUnitTests(std::cout);
  TestFindAllOccurences(std::cout);
  TestFindNumberOfSubstrings(std::cout);
  TestBuildSuffixArray(std::cout);
  TestGetLengthOfMaxSubstring(std::cout);
  system("PAUSE");

  return 0;
}
