#include "token.h"

using terse::Token;
using terse::TokenType;
using terse::WordToken;

TokenType::Value TokenType::next_value{0};
std::map<std::string_view, TokenType> TokenType::map{};
std::vector<std::string_view> TokenType::names{};

const TokenType TokenType::NONE{"NONE"};
const TokenType TokenType::ARM{"=>", "ARM"};
const TokenType TokenType::BRACE_LEFT{"{", "BRACE_LEFT"};
const TokenType TokenType::BRACE_RIGHT{"}", "BRACE_RIGHT"};
const TokenType TokenType::DOUBLE_QUOTE{"\"", "DOUBLE_QUOTE"};
const TokenType TokenType::FUNC{"$", "FUNC"};
const TokenType TokenType::NEWLINE{"\n", "NEWLINE"};
const TokenType TokenType::PAREN_LEFT{"(", "PAREN_LEFT"};
const TokenType TokenType::PAREN_RIGHT{")", "PAREN_RIGHT"};
const TokenType TokenType::STOP{"STOP"};
const TokenType TokenType::WORD{"WORD"};

TokenType TokenType::from_lexeme(const std::string& lexeme) {
  auto it = map.find(lexeme);

  if (it != map.end()) {
    return it->second;
  }

  return NONE;
}

std::ostream& terse::operator<<(std::ostream& out, TokenType type) {
  return out << type.to_string();
}

Token::Token(TokenType type, int line, int col) noexcept : type_{type}, line_{line}, col_{col} {}

const TokenType Token::type() const noexcept {
  return type_;
}

int Token::line() const noexcept {
  return line_;
}

int Token::col() const noexcept {
  return col_;
}

WordToken::WordToken(const std::string& value, int line, int col) : Token{TokenType::WORD, line, col}, value_{value} {}

std::string WordToken::value() const noexcept {
  return value_;
}
