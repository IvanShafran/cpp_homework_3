/*There is the implementation of a suffix tree, which has
**linear time of a construction and linear extramemory.
**
**SuffixTree class has two public methods:
**1)  //returns your string + '$'
**  const std::string& get_string();
**2)
**  void DFS(Visitor* visitor);
**  
**DFS - Depth-first search
**For using DFS you must implement public inheritor of SuffixTreeVisitor.
**
**You get information from dfs in this way:
**
**dfs(int v, Visitor* visitor) {
**  visitor->BeforeVertexProcessing();
**  for (u : incidence_list(v)) {
**    bool do_transition;
**    visitor->ProcessLink(v, u, begin(v, u), end(v, u), &do_transition);
**    if (do_transition) {
**      dfs(u)
**    }
**  }
**  visitor->AfterProcessing(v);
**}
**
**[ begin(v, u); end(v, u) ) is a substring of suffix_tree_string
**
**You can see example in find_all_occurences.h
*/

#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <stack>
#include <map>


const int NULL_VERTEX = -1;
const int INF = std::numeric_limits<int>::max();


class SuffixTreeVisitor {
 public:
  const std::string* suffix_tree_string_;
   
  //calls before vertices proccessing once time
  void set_suffix_tree_string(const std::string* suffix_tree_string) {
    this->suffix_tree_string_ = suffix_tree_string;
  }

  void BeforeVertexProcessing(int vertex) {}

  void ProcessLink(int vertex, int incidence_vertex,
                   int begin_substring_index, int end_substring_index,
                   bool* do_transition) {}

  void AfterVertexProcessing(int vertex) {}
};

class SuffixTree {
public:
  SuffixTree(const std::string& string) {
    string_ = string;
    string_ += '$';

    BuildAlphabet();

    InitTree();

    BuildTree();
  }

  const std::string& get_string() const {
    return string_;
  }

  template <class Visitor>
  void DFS(Visitor* visitor) const {
    visitor->set_suffix_tree_string(&(this->get_string()));

    size_t number_of_vertices = this->tree_.size();
    enum colors { WHITE, GREY, BLACK };
    std::vector<colors> color(number_of_vertices, WHITE);
    std::vector<std::vector<int> > link_lists(number_of_vertices);
    std::vector<int> next_link(number_of_vertices);
    std::stack<size_t> vertex_stack;

    vertex_stack.push(this->root_);
    while (!vertex_stack.empty()) {
      size_t vertex = vertex_stack.top();

      if (color[vertex] == WHITE) {
        visitor->BeforeVertexProcessing(vertex);

        link_lists[vertex] = GetIncidenceList(vertex);
        next_link[vertex] = 0;
        color[vertex] = GREY;
      }

      if (next_link[vertex] == link_lists[vertex].size()) {
        vertex_stack.pop();
        link_lists[vertex].resize(0);
        color[vertex] = BLACK;

        visitor->AfterVertexProcessing(vertex);
      }
      else {
        Link link = tree_[vertex].links[link_lists[vertex][next_link[vertex]]];

        bool do_transition;

        visitor->ProcessLink(vertex,
                             link.incidence_vertex,
                             link.begin_substring, 
                             std::min<int>(link.end_substring, 
                                           this->string_.size()),
                             &do_transition);

        if (color[link.incidence_vertex] == WHITE && do_transition) {
          vertex_stack.push(link.incidence_vertex);
        }

        ++next_link[vertex];
      }
    }
  }

#ifdef SUFFIX_TREE_TESTING
 public:
#else
 private:
#endif

  struct Link {
    int begin_substring, end_substring, incidence_vertex;

    Link() : incidence_vertex(NULL_VERTEX) {}
    Link(int begin, int end, int to) : begin_substring(begin),
      end_substring(end),
      incidence_vertex(to) {}
  };

  struct Vertex {
    std::vector<Link> links;
    int suffix;

    Vertex(int number_of_links) : links(number_of_links, Link()), suffix(NULL_VERTEX) {}
  };

  struct Point {
    int vertex;
    int begin;

    Point() {}
    Point(int _vertex, int _begin) : vertex(_vertex), begin(_begin) {}
  };

  std::vector<Vertex> tree_;
  std::string string_;
  int root_, dummy_;
  Point active_point_;
  std::string alphabet_;
  std::map<int, int> map_of_alphabet_index_;

  void BuildAlphabet() {
    for (int i = 0; i < string_.size(); ++i) {
      if (std::find(alphabet_.begin(), alphabet_.end(), string_[i]) ==
          alphabet_.end()) {
        map_of_alphabet_index_[string_[i]] = alphabet_.size();
        alphabet_ += string_[i];
      }
    }
  }

  int GetSizeOfAlhpabet() const {
    return alphabet_.size();
  }

  int& suffix_link(int v) {
    return tree_[v].suffix;
  }

  int get_edge_index(int symbol_index) {
    return map_of_alphabet_index_[string_[symbol_index]];
  }

  void LinkVertex(int from, int first_symbol,
    int begin_substring, int end_substring, int to) {
    tree_[from].links[first_symbol] = Link(begin_substring, end_substring, to);
  }

  int NewVertex() {
    int index = tree_.size();
    tree_.push_back(Vertex(this->GetSizeOfAlhpabet()));
    return index;
  }

  void InitTree() {
    dummy_ = NewVertex();
    root_ = NewVertex();
    active_point_ = Point(root_, 0);

    suffix_link(root_) = dummy_;
    for (int i = 0; i < GetSizeOfAlhpabet(); ++i) {
      LinkVertex(dummy_, i, i, i + 1, root_);
    }
  }

  Point Canonicalize(int vertex, int begin, int end) {
    if (end <= begin) {
      return Point(vertex, begin);
    }
    else {
      Link link = tree_[vertex].links[get_edge_index(begin)];
      while (end - begin >= link.end_substring - link.begin_substring) {
        begin += link.end_substring - link.begin_substring;
        vertex = link.incidence_vertex;
        if (end > begin)
          link = tree_[vertex].links[get_edge_index(begin)];
      }
      return Point(vertex, begin);
    }
  }

  bool Split(int vertex, int begin, int end, int link_index,
    int* next_branching_vertex) {
    if (end <= begin) {
      *next_branching_vertex = vertex;
      return tree_[vertex].links[link_index].incidence_vertex == NULL_VERTEX;
    }
    else {
      Link link = tree_[vertex].links[get_edge_index(begin)];
      if (link_index == get_edge_index(link.begin_substring + end - begin)) {
        *next_branching_vertex = vertex;
        return false;
      }

      int middle = NewVertex();
      LinkVertex(vertex, get_edge_index(link.begin_substring),
        link.begin_substring,
        link.begin_substring + end - begin, middle);
      LinkVertex(middle, get_edge_index(link.begin_substring + end - begin),
        link.begin_substring + end - begin,
        link.end_substring, link.incidence_vertex);

      *next_branching_vertex = middle;
      return true;
    }
  }

  Point CreateNewBranches(int vertex, int begin, int end) {
    int next_branching_vertex;
    int previous_branching_vertex = root_;

    while (Split(vertex, begin, end, get_edge_index(end), &next_branching_vertex)) {
      LinkVertex(next_branching_vertex, get_edge_index(end),
        end, INF, NewVertex());

      if (previous_branching_vertex != root_) {
        suffix_link(previous_branching_vertex) = next_branching_vertex;
      }
      previous_branching_vertex = next_branching_vertex;

      Point newPoint = Canonicalize(suffix_link(vertex), begin, end);
      vertex = newPoint.vertex;
      begin = newPoint.begin;
    }

    if (previous_branching_vertex != this->root_) {
      suffix_link(previous_branching_vertex) = next_branching_vertex;
    }

    return Point(vertex, begin);
  }

  void AddSymbol(int index_of_next_symbol) {
    int end = index_of_next_symbol;

    this->active_point_ = CreateNewBranches(active_point_.vertex,
      active_point_.begin, end);
    this->active_point_ = Canonicalize(active_point_.vertex,
      active_point_.begin, end + 1);
  }

  void BuildTree() {
    for (size_t i = 0; i < string_.size(); ++i) {
      AddSymbol(i);
    }
  }

  std::vector<int> GetIncidenceList(int vertex) const {
    std::vector<int> incidence_list;
    for (int index = 0; index < GetSizeOfAlhpabet(); ++index) {
      if (tree_[vertex].links[index].incidence_vertex != NULL_VERTEX) {
        incidence_list.push_back(index);
      }
    }

    return incidence_list;
  }

};
