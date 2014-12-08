#pragma once

#include <string>
#include <vector>

#include "suffix_tree.h"

namespace {

class BuildSuffixArrayVisitor : public SuffixTreeVisitor {
public:
  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {
    if (end_substring_index == suffix_tree_string_->size()) {
      //leaf
      *do_transition = false;
      int string_length = suffix_tree_string_->size();

      suffix_array_.push_back(string_length - (*distance_from_root_)[incidence_vertex]);
    }
    else {
      //not leaf
      *do_transition = true;
    }
  }

  std::vector<int> GetSuffixArray() const {
    std::vector<int> result = suffix_array_;

    result.erase(result.begin());

    return result;
  }
private:
  std::vector<int> suffix_array_;
  int suffix_order;
};

std::vector<int> BuildSuffixArray(const SuffixTree& tree) {
  BuildSuffixArrayVisitor visitor;

  tree.TreeTraversal(&visitor);

  return visitor.GetSuffixArray();
}

}
