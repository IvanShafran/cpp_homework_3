#include <string>

namespace {

void ThrowException(const std::string& error_text) {
  throw std::runtime_error(error_text);
}

void PrintOK(const std::string& string, std::ostream& out) {
  out << string << " OK" << "\n";
}

template <class T>
void CheckEq(const T& a, const T& b, const std::string& error_text) {
  if (a != b) {
    ThrowException(error_text);
  }
}

}
