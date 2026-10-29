#pragma once

#include <ostream>
#include <vector>

#include <utl/string_view.hpp>

int execute(const std::vector<utl::string_view>& args);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vect) {
  for (const T& t : vect)
    out << t << ' ';
  return out;
}
