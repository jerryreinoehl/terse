#include "grammar.h"

#include <iostream> // REMOVE

using terse::WordExpression;
using terse::Statements;

void WordExpression::add(const std::string& word) {
  std::cout << "WordExpression::add( " << word << " )\n";
  words_.push_back(word);
}

size_t WordExpression::size() const noexcept {
  return words_.size();
}

const std::string& WordExpression::operator[](size_t index) const {
  return words_.at(index);
}

const std::string& WordExpression::operator[](idx::last_t) const {
  return words_.at(words_.size()-1);
}

void Statements::add(const Statement& statement) {
  statements_.push_back(statement);
}
