#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "suffix_tree.h"

class GetLengthOfMaxSubstringFirstStageVisitor : public SuffixTreeVisitor {
 public:
  void set_pattern(const std::string* string) {
    pattern = string;
  }

  void InitVisitor() {
    if (!already_init) {
      already_init = true;
      number_of_occurrences.resize(this->number_of_vertices_);
    }
  }

  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {
    number_of_occurrences[vertex] += 1;

    if (vertex_substrings.find(vertex) == vertex_substrings.end()) {
      vertex_substrings[vertex] = std::map<int, std::vector<int>>();
    }

    *do_transition = IsThereTransition(vertex,
                                       incidence_vertex,
                                       begin_substring_index, 
                                       end_substring_index);
  }

  bool IsThereTransition(int vertex,
                         int incidence_vertex,
                         int begin_substring_index,
                         int end_substring_index) {
    if (static_cast<size_t>((*distance_from_root_)[vertex]) >= pattern->size()) {
      return false;
    }

    int checking_length =
      std::min<int>(end_substring_index - begin_substring_index,
      pattern->size() - (*distance_from_root_)[vertex]);

    std::string::const_iterator pattern_begin = pattern->cbegin() +
      (*distance_from_root_)[vertex];
    std::string::const_iterator string_begin = suffix_tree_string_->cbegin() +
      begin_substring_index;

    int equal_symbols = 0;
    while (equal_symbols < checking_length && *pattern_begin == *string_begin) {
      ++equal_symbols;
      ++pattern_begin;
      ++string_begin;
    }

    if (checking_length != equal_symbols) {
      vertex_substrings[vertex][begin_substring_index].push_back(begin_substring_index
                                                                 + equal_symbols);
      return false;
    }
    else {
      return true;
    }
  }

 private:
  const std::string* pattern;
  std::vector<int> number_of_occurrences;
  //vertex -> begin_edge -> (end_edge1, end_edge2, end_edge3)
  std::map<int, std::map<int, std::vector<int>>> vertex_substrings;
  bool already_init;
};

class GetLengthOfMaxSubstringSecondStageVisitor : public SuffixTreeVisitor {
 public:
  void set_number_of_strings(int number) {
    number_of_strings = number;
  }

  void InitVisitor() {
    number_of_k_times_occurences.resize(number_of_strings - 2);
  }

  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {
    if ((*number_of_occurrences)[incidence_vertex] >= 2) {
      std::map<int, std::vector<int>>& map_of_substrings = (*vertex_substrings)[incidence_vertex];

      for (auto edge_end_vector_it = map_of_substrings.begin();
        edge_end_vector_it != map_of_substrings.end(); ++edge_end_vector_it) {
        for (int edge_end_index = 0; edge_end_index < (*edge_end_vector_it).second.size(); ++edge_end_index) {
          UpdateKTimesOccurences();
        }
      }

      *do_transition = true;
    }
    else {
      *do_transition = false;
    }
  }

 private:
  const std::vector<int>* number_of_occurrences;
  //begin_edge -> (end_edge1, end_edge2, end_edge3)
  std::map<int, std::map<int, std::vector<int>>>* vertex_substrings;
  //0 index - 2 times occurence
  std::vector<int> number_of_k_times_occurences;
  int number_of_strings;

  void UpdateKTimesOccurences(int k, int length) {
    number_of_k_times_occurences[k - 2] = std::max(number_of_k_times_occurences[k - 2], length);
  }
};

std::vector<int> GetLengthOfMaxSubstring(const std::vector<std::string>& strings) {
  std::string string;
  for (size_t i = 0; i < strings.size(); ++i) {
    string += strings[i];
    string += "$";
  }

  SuffixTree tree(string, "");
  
  GetLengthOfMaxSubstringFirstStageVisitor first_visitor;

  for (size_t i = 0; i < strings.size(); ++i) {
    first_visitor.set_pattern(&strings[i]);

    tree.TreeTraversal(&first_visitor);
  }

  GetLengthOfMaxSubstringSecondStageVisitor second_visitor;
  second_visitor.set_number_of_strings(strings.size());


}
