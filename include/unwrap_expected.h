#pragma once

#include "unwrap.h"
#include "expected.h"

template <typename V, typename E>
struct error_propagation_traits<expected<V, E>> {
  static bool has_value(const expected<V, E>& t) {
    return t.has_value();
  }

  static V extract_value(expected<V, E>&& t) {
    return std::forward<expected<V, E>>(t).value();
  }

  static E extract_error(expected<V, E>&& t) {
    return std::forward<expected<V, E>>(t).error();
  }

  static expected<V, E> from_value(V&& v) {
    return std::forward<V>(v);
  }

  static expected<V, E> from_error(E&& e) {
    return unexpected<E>{std::forward<E>(e)};
  }
};
