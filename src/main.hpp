#pragma once

#include <ostream>
#include <vector>

int execute(const std::vector<std::string>& args);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vect) {
  for (const T& t : vect)
    out << t << ' ';
  return out;
}
