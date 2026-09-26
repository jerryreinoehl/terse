#pragma once

#include <iostream>
#include <utility>

namespace terse {
  template <typename T, typename = void>
  struct has_to_string : std::false_type {};

  template <typename T>
  struct has_to_string<T, decltype(std::declval<T>().to_string(), void())> : std::true_type {};

  template <typename T, typename = void>
  struct is_printable : std::false_type {};

  template <typename T>
  struct is_printable<T, decltype(std::declval<std::ostream&>() << std::declval<T>(), void())> : std::true_type {};

  template <typename T, typename std::enable_if<has_to_string<T>::value && !is_printable<T>::value, int>::type = 0>
  std::ostream&
  operator<<(std::ostream& out, const T& t) {
    return out << std::string{t.to_string()};
  }
}
