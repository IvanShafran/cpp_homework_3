#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "suffix_tree.h"

class GetLengthOfMaxSubstringVisitor : public SuffixTreeVisitor {
 public:
  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {
    if (end_substring_index == suffix_tree_string_->size()) {
      //incidence_vertex - leaf
      achievable_strings[vertex].insert((*string_index)[begin_substring_index]);
      *do_transition = false;
    }
    else {
      *do_transition = true;
    }
  }
  
  void AfterVertexProcessing(int vertex) {
    int k = achievable_strings[vertex].size();
    if (k >= 2) {
      k_times_occurences[k] = std::max(k_times_occurences[k], (*distance_from_root_)[vertex]);
    }
    //push up to parent
    achievable_strings[(*parent_)[vertex]].insert(achievable_strings[vertex].begin(), 
                                                  achievable_strings[vertex].end());
  }

  std::map<int, int> GetKTimesOccurences(size_t number_of_strings) {
    for (size_t i = number_of_strings - 1; i >= 2; --i) {
      k_times_occurences[i] = std::max(k_times_occurences[i + 1], k_times_occurences[i]);
    }

    if (number_of_strings == 2 && k_times_occurences.size() == 0) {
      k_times_occurences.insert({2, 0});
    }

    return k_times_occurences;
  }

  void set_string_index(const std::vector<int>* _string_index) {
    string_index = _string_index;
  }

 private:
   const std::vector<int>* string_index;
   std::unordered_map<int, std::unordered_set<int>> achievable_strings;
   std::map<int, int> k_times_occurences;
};

std::string GetSeparators(const std::vector<std::string>& strings) {
  std::string separators;
  for (char symbol = -127; symbol < 128; ++symbol) {
    
    bool is_contain = false;
    for (size_t i = 0; i < strings.size(); ++i) {
      if (strings[i].find(symbol) != std::string::npos) {
        is_contain = true;
        break;
      }
    }

    if (!is_contain) {
      separators += symbol;
      if (separators.size() == strings.size()) {
        break;
      }
    }
  }

  return separators;
}

size_t GetLengthOfStrings(const std::vector<std::string>& strings) {
  size_t length = 0;
  std::for_each(strings.begin(), strings.end(), 
    [&length](const std::string& s){ length += s.size(); });

  return length;
}

std::string ConstrucString(const std::vector<std::string>& strings, 
                           std::vector<int>* string_index) {
  std::string separators = GetSeparators(strings);

  std::string string;
  size_t size_of_string = GetLengthOfStrings(strings) + strings.size();
  string.reserve(size_of_string);
  string_index->resize(size_of_string);
  int new_string_position = 0;
  for (size_t i = 0; i < strings.size(); ++i) {
    string += strings[i];
    string += separators[i];

    std::fill(string_index->begin() + new_string_position,
      string_index->begin() + new_string_position + strings[i].size() + 1,
      i);
    new_string_position += strings[i].size() + 1;
  }

  return string;
}

std::map<int, int> GetLengthOfMaxSubstring(const std::vector<std::string>& strings) {
  std::vector<int> string_index;
  std::string string = ConstrucString(strings, &string_index);

  SuffixTree tree(string, "");
  
  GetLengthOfMaxSubstringVisitor visitor;
  visitor.set_string_index(&string_index);

  tree.TreeTraversal(&visitor);

  return visitor.GetKTimesOccurences(strings.size());
}
