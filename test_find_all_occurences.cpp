#include <ostream>
#include <random>
#include <algorithm>

#include "shafran_test_framework.h"
#include "find_all_occurrences.h"

void TestFindAllOccurencesUnit() {
  {
    SuffixTree tree("a");
    std::vector<int> result({ 0, });
    CheckEq(FindAllOccurrences(tree, "a"), result,
      "TestFindAllOccurencesUnit: wrong result #1");
  }

  {
    SuffixTree tree("aaaaa");
    std::vector<int> result({ 0, 1, 2, 3, 4 });
    std::vector<int> occurences = FindAllOccurrences(tree, "a");
    std::sort(occurences.begin(), occurences.end());
    CheckEq(occurences, result,
      "TestFindAllOccurencesUnit: wrong result #2");
  }

  {
    SuffixTree tree("ababa");
    std::vector<int> result({ 0, 2 });
    std::vector<int> occurences = FindAllOccurrences(tree, "aba");
    std::sort(occurences.begin(), occurences.end());
    CheckEq(occurences, result,
      "TestFindAllOccurencesUnit: wrong result #3");
  }

  {
    SuffixTree tree("a");
    std::vector<int> result;
    std::vector<int> occurences = FindAllOccurrences(tree, "b");
    std::sort(occurences.begin(), occurences.end());
    CheckEq(occurences, result,
      "TestFindAllOccurencesUnit: wrong result #4");
  }

  {
    SuffixTree tree("a");
    std::vector<int> result;
    std::vector<int> occurences = FindAllOccurrences(tree, "ad");
    std::sort(occurences.begin(), occurences.end());
    CheckEq(occurences, result,
      "TestFindAllOccurencesUnit: wrong result #5");
  }
}

std::string GetRandomString(size_t length, const std::string& symbol_set) {
  std::string result;
  for (size_t i = 0; i < length; ++i) {
    result += symbol_set[rand() % symbol_set.size()];
  }

  return result;
}

std::vector<int> FindAllOccurencesByTrivialAlgo(const std::string& text,
  const std::string& pattern) {
  std::vector<int> result;
  for (size_t i = 0; i < text.size(); ++i) {
    if (text.find(pattern, i) == i) {
      result.push_back(i);
    }
  }

  return result;
}

void TestFindAllOccurencesRandomTest() {
  srand(777);
  for (int i = 0; i < 100; ++i) {
    std::string text = GetRandomString(1000, "abcd");
    std::string pattern = GetRandomString(5, "abcd");

    SuffixTree tree(text);
    std::vector<int> occurences_by_suffix_tree = 
      FindAllOccurrences(tree, pattern);
    std::vector<int> occurences_by_trivial_algo = 
      FindAllOccurencesByTrivialAlgo(text, pattern);

    std::sort(occurences_by_suffix_tree.begin(), 
      occurences_by_suffix_tree.end());
    std::sort(occurences_by_trivial_algo.begin(), 
      occurences_by_trivial_algo.end());

    CheckEq(occurences_by_suffix_tree, occurences_by_trivial_algo, 
      "TestFindAllOccurencesRandomTest: error on test : " + '\n' 
      + text + '\n' + pattern);
  }
}

void TestFindAllOccurences(std::ostream& out) {
  TestFindAllOccurencesUnit();
  PrintOK("TestFindAllOccurencesUnit", out);

  TestFindAllOccurencesRandomTest();
  PrintOK("TestFindAllOccurencesRandomTest", out);
}
