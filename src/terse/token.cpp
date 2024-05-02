#include "token.h"

using terse::Token;
using terse::TokenType;
using terse::WordToken;

Token::Token(TokenType type) : type_{type} {}

TokenType Token::type() const noexcept {
  return type_;
}

WordToken::WordToken(const std::string& value) : Token{TokenType::WORD}, value_{value} {}

std::string WordToken::value() const noexcept {
  return value_;
}
