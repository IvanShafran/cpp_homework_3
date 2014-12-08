#include <string>
#include <vector>
#include <algorithm>

#include "suffix_tree.h"
#include "shafran_test_framework.h"
#include "length_of_max_substring.h"

void TestGetLengthOfMaxSubstringUnit() {
  {
    CheckEq<std::map<int, int>>(GetLengthOfMaxSubstring({ "ab", "ac" }), 
      { { 2, 1 } },
      "TestGetLengthOfMaxSubstringUnit: wrong result #1");
  }

  {
    CheckEq<std::map<int, int>>(GetLengthOfMaxSubstring({ "abc", "a", "bc" }),
      { { 3, 0 }, { 2, 2} },
      "TestGetLengthOfMaxSubstringUnit: wrong result #2");
  }

  {
    CheckEq<std::map<int, int>>(GetLengthOfMaxSubstring({ "abcd", "bcd", "cd", "d" }),
      { { 2, 3 }, { 3, 2 }, {4, 1} },
      "TestGetLengthOfMaxSubstringUnit: wrong result #3");
  }
}

int GetNumberOfOccurences(std::string substring,
                          const std::vector<std::string>& strings, 
                          int len,
                          const std::string& alphabet) {
  int result = 0;

  if (substring.size() == len) {
    for (size_t i = 0; i < strings.size(); ++i) {
      if (strings[i].find(substring) != std::string::npos) {
        ++result;
      }
    }
    return result;
  }

  for (size_t i = 0; i < alphabet.size(); ++i) {
    result = std::max(result, GetNumberOfOccurences(substring + alphabet[i], 
                                                    strings, 
                                                    len, 
                                                    alphabet));
  }

  return result;
}

std::map<int, int> GetLengthOfMaxSubstringTrivial(const std::vector<std::string>& strings,
                                                  const std::string& alphabet) {
  std::map<int, int> result;
  for (size_t len = 1; len <= strings.size(); ++len) {
    int number_of_occurences = GetNumberOfOccurences("", strings, len, alphabet);

    if (number_of_occurences > 1) {
      result[number_of_occurences] = len;
    }
  }

  for (size_t i = strings.size() - 1; i >= 2; --i) {
    result[i] = std::max(result[i + 1], result[i]);
  }

  if (strings.size() == 2 && result.size() == 0) {
    result.insert({ 2, 0 });
  }

  return result;
}

void TestGetLengthOfMaxSubstringRandom() {
  srand(777);
  std::string alphabet = "abcde";
  size_t number_of_strings = 8;
  size_t number_of_symbols = 5;
  for (size_t i = 0; i < 100; ++i) {
    std::vector<std::string> strings;
    for (size_t j = 0; j < number_of_strings; ++j) {
      strings.push_back(GetRandomString(number_of_symbols, alphabet));
    }

    CheckEq(GetLengthOfMaxSubstring(strings), GetLengthOfMaxSubstringTrivial(strings, alphabet),
      "TestGetLengthOfMaxSubstringRandom: wrong result");

  }
}

void TestGetLengthOfMaxSubstring(std::ostream& out) {
  try {
    TestGetLengthOfMaxSubstringUnit();
    PrintOK("TestGetLengthOfMaxSubstringUnit", out);

    TestGetLengthOfMaxSubstringRandom();
    PrintOK("TestGetLengthOfMaxSubstringRandom", out);
  }
  catch (std::exception e) {
    out << e.what() << "\n";
  }
}
