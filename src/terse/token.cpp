#include "token.h"

using terse::Token;

Token::Token(Type type, std::string_view value) : type_{type}, value_{value} {}

Token::Type Token::type() const noexcept {
  return type_;
}

std::string Token::value() const noexcept {
  return value_.value_or("");
}
