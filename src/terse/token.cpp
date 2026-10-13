#include "token.hpp"

using terse::TokenType;

std::vector<std::string_view> TokenType::names{TokenType::value_count};
std::map<std::string_view, terse::TokenType> TokenType::map{};
TokenType TokenType::token_type{TokenType::token_type_t{}};

void TokenType::add(TokenType type, std::string_view name) {
  TokenType::names[type] = name;
}

void TokenType::add(TokenType type, std::string_view name, std::string_view lexeme) {
  TokenType::names[type] = name;
  TokenType::map[lexeme] = type;
}

TokenType TokenType::from_lexeme(std::string_view lexeme) {
  if (TokenType::map.count(lexeme) > 0) {
    return TokenType::map[lexeme];
  }
  return TokenType::None;
}

std::string_view TokenType::to_string() const {
  return TokenType::names[value];
}

TokenType::TokenType(TokenType::token_type_t) {
  TokenType::add(TokenType::None, "None");
  TokenType::add(TokenType::Arm, "Arm", "=>");
  TokenType::add(TokenType::LeftBrace, "LeftBrace", "{");
  TokenType::add(TokenType::RightBrace, "RightBrace", "}");
  TokenType::add(TokenType::DoubleQuote, "DoubleQuote", "\"");
  TokenType::add(TokenType::Error, "Error");
  TokenType::add(TokenType::Func, "Func", "$");
  TokenType::add(TokenType::Newline, "Newline", "\n");
  TokenType::add(TokenType::LeftParen, "LeftParen", "(");
  TokenType::add(TokenType::RightParen, "RightParen", ")");
  TokenType::add(TokenType::Stop, "Stop");
  TokenType::add(TokenType::String, "String");
}

std::ostream& terse::operator<<(std::ostream& out, TokenType::Value type) {
  return out << static_cast<TokenType>(type).to_string();
}
