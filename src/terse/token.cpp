#include "token.h"

using terse::Token;
using terse::TokenType;
using terse::StringToken;
using terse::ErrorToken;

std::vector<std::string_view> TokenType::names{TokenType::value_count};
std::map<std::string_view, terse::TokenType> map{};

void TokenType::add(TokenType type, std::string_view name) {
  TokenType::names[type] = name;
}

void TokenType::add(TokenType type, std::string_view name, std::string_view lexeme) {
  TokenType::names[type] = name;
  TokenType::map[lexeme] = type;
}

TokenType from_lexeme(std::string_view lexeme) {
  return TokenType::map[lexeme];
}

TokenType& TokenType::get_instance() noexcept {
  static TokenType token_type{token_type_t{}};
  return token_type;
}

std::string_view TokenType::to_string() const {
  return TokenType::names[value];
}

TokenType::TokenType(TokenType::token_type_t) {
  std::cout << "Init TokenType_\n";
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

Token::Token(TokenType type, int line, int col) noexcept : line_{line}, col_{col}, type_{type} {}

int Token::line() const noexcept {
  return line_;
}

int Token::col() const noexcept {
  return col_;
}

StringToken::StringToken(const std::string& value, int line, int col) noexcept
  : Token{TokenType::String, line, col}, value_{value} {}

std::string StringToken::value() const noexcept {
  return value_;
}

ErrorToken::ErrorToken(const std::string& error, int line, int col) noexcept
  : Token{TokenType::Error, line, col}, error_{error} {}

std::string ErrorToken::error() const noexcept {
  return error_;
}
