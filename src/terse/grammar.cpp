#include "grammar.hpp"

using terse::WordExpression;
using terse::Statements;

WordExpression& WordExpression::operator=(WordExpression&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  words_ = std::move(other.words_);
}

void WordExpression::add(TokenPtr token) {
  words_.push_back(std::move(token));
}

size_t WordExpression::size() const noexcept {
  return words_.size();
}

const std::vector<terse::TokenPtr>& WordExpression::words() const& noexcept {
  return words_;
}

std::vector<terse::TokenPtr> WordExpression::words() && noexcept {
  return std::move(words_);
}

const terse::Token& WordExpression::operator[](size_t index) const {
  return *words_.at(index);
}

const terse::Token& WordExpression::first() const& noexcept {
  return *words_[0];
}

terse::TokenPtr WordExpression::first() && noexcept {
  return std::move(words_[0]);
}

void Statements::add(const Statement& statement) {
  statements_.push_back(statement);
}
