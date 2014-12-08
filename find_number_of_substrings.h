#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "suffix_tree.h"

namespace {

class FindNumberOfSubstringsVisitor : public SuffixTreeVisitor {
 public:
  void InitVisitor() {
    number_of_substrings_ = 0;
  }

  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {
    *do_transition = true;

    number_of_substrings_ += end_substring_index - begin_substring_index;
  }

  int GetNumberOfSubstrings() const {
    //ignore symbol $
    return number_of_substrings_ - static_cast<int>(suffix_tree_string_->size());
  }

 private:
  int number_of_substrings_;
};

int FindNumberOfSubstrings(const SuffixTree& tree) {
  FindNumberOfSubstringsVisitor visitor;
  tree.TreeTraversal(&visitor);

  return visitor.GetNumberOfSubstrings();
}

}
