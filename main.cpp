#pragma once

#include <iostream>

#include "test_suffix_tree.h"
#include "test_find_all_occurences.h"

int main() {
  TestSuffixTreeUnitTests(std::cout);
  TestFindAllOccurences(std::cout);
  system("PAUSE");

  return 0;
}
