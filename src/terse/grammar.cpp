#include "grammar.hpp"

using terse::WordExpression;
using terse::Statements;

void WordExpression::add(utl::string_view word) {
  words_.push_back(std::string{word});
}

size_t WordExpression::size() const noexcept {
  return words_.size();
}

const std::vector<std::string>& WordExpression::words() const noexcept {
  return words_;
}

utl::string_view WordExpression::operator[](size_t index) const {
  return words_.at(index);
}

utl::string_view WordExpression::first() const noexcept {
  return words_[0];
}

void Statements::add(const Statement& statement) {
  statements_.push_back(statement);
}
