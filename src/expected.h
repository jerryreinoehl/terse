#pragma once

#include <iostream>

template <typename E>
class Unexpected {
  public:
    Unexpected(const E& error) : error_{error} {}
    Unexpected(E&& error) : error_{std::move(error)} {}

    const E& error() const& noexcept {
      return error_;
    }

    E&& error() && noexcept {
      return std::move(error_);
    }

  private:
    E error_;
};

template <typename E>
Unexpected<E> unexpected(const E& error) {
  return Unexpected<E>{error};
}

template <typename E>
Unexpected<E> unexpected(E&& error) {
  return Unexpected<E>{std::move(error)};
}

template <typename V, typename E>
class Expected;

template <typename T>
struct expected_traits;

template <typename V, typename E>
struct expected_traits<Expected<V, E>> {
  typedef V value_type;
  typedef E error_type;
};

template <typename E>
struct expected_traits<Unexpected<E>> {
  typedef E error_type;
};

template <typename T, typename V, typename E>
struct normalize_expected {
  typedef Expected<typename std::decay<T>::type, E> type;
};

template <typename U, typename F, typename V, typename E>
struct normalize_expected<Expected<U, F>, V, E> {
  typedef Expected<U, F> type;
};

template <typename F, typename V, typename E>
struct normalize_expected<Unexpected<F>, V, E> {
  typedef Expected<V, F> type;
};

template <typename V, typename E>
class Expected {
  public:

    Expected(const V& value) : has_value_{true} {
      new (&storage_.value) V{value};
    }

    Expected(V&& value) : has_value_{true} {
      new (&storage_.value) V{std::move(value)};
    }

    Expected(const Expected<V, E>& expected) : has_value_{expected.has_value_} {
      if (has_value_) {
        new (&storage_.value) V{expected.storage_.value};
      } else {
        new (&storage_.error) E{expected.storage_.error};
      }
    }

    Expected(Expected<V, E>&& expected) : has_value_{expected.has_value_} {
      if (has_value_) {
        new (&storage_.value) V{std::move(expected.storage_.value)};
      } else {
        new (&storage_.error) E{std::move(expected.storage_.error)};
      }
    }

    Expected(const Unexpected<E>& error) : has_value_{false} {
      new (&storage_.error) E{error.error()};
    }

    Expected(Unexpected<E>&& error) : has_value_{false} {
      new (&storage_.error) E{std::move(error.error())};
    }

    ~Expected() {
      destroy();
    }

    V& value() & {
      return storage_.value;
    }

    const V& value() const& {
      return storage_.value;
    }

    V&& value() && {
      return std::move(storage_.value);
    }

    E& error() & {
      return storage_.error;
    }

    const E& error() const& {
      return storage_.error;
    }

    E&& error() && {
      return std::move(storage_.error);
    }

    template <typename F>
    typename normalize_expected<
      typename std::result_of<F(V&)>::type, V, E
    >::type
    and_then(F&& func) & {
      typedef typename std::result_of<F(V&)>::type CallbackResult;
      typedef typename normalize_expected<CallbackResult, V, E>::type Result;

      static_assert(
        std::is_same<typename expected_traits<Result>::error_type, E>::value,
        "and_then callback must return either Expected<V, E> or Expected<E>"
      );

      if (has_value_) {
        return Result{func(storage_.value)};
      }

      return Result{unexpected(storage_.error)};
    }

    template <typename F>
    typename normalize_expected<
      typename std::result_of<F(V&&)>::type, V, E
    >::type
    and_then(F&& func) && {
      typedef typename std::result_of<F(V&&)>::type CallbackResult;
      typedef typename normalize_expected<CallbackResult, V, E>::type Result;

      static_assert(
        std::is_same<typename expected_traits<Result>::error_type, E>::value,
        "and_then callback must return either Expected<V, E> or Expected<E>"
      );

      if (has_value_) {
        return Result{func(std::move(storage_.value))};
      }

      return Result{unexpected(std::move(storage_.error))};
    }

    template <typename F>
    Expected<V, E>
    or_else(F&& func) & {
      typedef typename std::result_of<F(E&)>::type CallbackResult;
      typedef typename normalize_expected<CallbackResult, V, E>::type Result;

      static_assert(
        std::is_same<typename expected_traits<Result>::value_type, V>::value &&
        std::is_same<typename expected_traits<Result>::error_type, E>::value,
        "and_then callback must return either Expected<V, E> or Expected<E>"
      );

      if (has_value_) {
        return *this;
      }

      return Result{func(storage_.error)};
    }

    template <typename F>
    Expected<V, E>
    or_else(F&& func) && {
      typedef typename std::result_of<F(E&&)>::type CallbackResult;
      typedef typename normalize_expected<CallbackResult, V, E>::type Result;

      static_assert(
        std::is_same<typename expected_traits<Result>::value_type, V>::value &&
        std::is_same<typename expected_traits<Result>::error_type, E>::value,
        "and_then callback must return either Expected<V, E> or Expected<E>"
      );

      if (has_value_) {
        return std::move(*this);
      }

      return Result{func(std::move(storage_.error))};
    }

    template <typename U>
    V value_or(U&& fallback) & {
      if (has_value_) {
        return storage_.value;
      }

      return static_cast<V>(std::forward<U>(fallback));
    }

    template <typename U>
    V value_or(U&& fallback) && {
      if (has_value_) {
        return std::move(storage_.value);
      }

      return static_cast<V>(std::forward<U>(fallback));
    }

    Expected<V, E>& operator=(const Expected<V, E>& rhs) noexcept {
      std::cout << "In copy assign\n";
      if (this == &rhs) {
        return *this;
      }

      destroy();

      has_value_ = rhs.has_value_;
      if (has_value_) {
        new (&storage_.value) V{rhs.storage_.value};
      } else {
        new (&storage_.error) E{rhs.storage_.error};
      }

      return *this;
    }

    Expected<V, E>& operator=(Expected<V, E>&& rhs) noexcept {
      std::cout << "In move assign\n";
      if (this == &rhs) {
        return *this;
      }

      destroy();

      has_value_ = rhs.has_value_;
      if (has_value_) {
        new (&storage_.value) V{std::move(rhs.storage_.value)};
      } else {
        new (&storage_.error) E{std::move(rhs.storage_.error)};
      }

      return *this;
    }

    explicit operator bool() const {
      return has_value_;
    }

    const V& operator*() const& {
      return storage_.value;
    }

    V& operator*() & {
      return storage_.value;
    }

    const V* operator->() const& {
      return &storage_.value;
    }

    V* operator->() & {
      return &storage_.value;
    }


  private:
    union Storage {
      V value;
      E error;

      Storage() {}
      ~Storage() {}
    } storage_;

    bool has_value_{false};

    void destroy() noexcept {
      if (has_value_) {
        storage_.value.~V();
      } else {
        storage_.error.~E();
      }
    }
};
