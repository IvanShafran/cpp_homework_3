#pragma once

#include <iostream>

#include "test_suffix_tree.h"
#include "test_find_all_occurences.h"

int main() {
  try {
    TestSuffixTreeUnitTests(std::cout);
    TestFindAllOccurences(std::cout);
  }
  catch (std::exception e) {
    std::cout << e.what() << std::endl;
    system("PAUSE");
    return 0;
  }

  std::cout << "ALL OK" << std::endl;
  system("PAUSE");
  return 0;
}
