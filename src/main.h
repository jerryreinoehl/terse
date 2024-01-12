#pragma once

#include "terse/emission.h"

#include <ostream>
#include <string>
#include <vector>

std::vector<std::string> translate(const std::vector<std::string>& args, const terse::TokenMap& map);

int execute(const std::vector<std::string>& args);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vect) {
  for (const T& t : vect)
    out << t << ' ';
  return out;
}
