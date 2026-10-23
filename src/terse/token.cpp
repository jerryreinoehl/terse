#include "token.hpp"
#include <utl/string_view.hpp>

using terse::TokenType;

std::vector<utl::string_view> TokenType::names{TokenType::value_count};
std::map<utl::string_view, terse::TokenType> TokenType::map{};
TokenType TokenType::token_type{TokenType::token_type_t{}};

TokenType::TokenType(TokenType::token_type_t) {
  TokenType::add(TokenType::None, "None");
  TokenType::add(TokenType::Arm, "Arm", "=>");
  TokenType::add(TokenType::LeftBrace, "LeftBrace", "{");
  TokenType::add(TokenType::RightBrace, "RightBrace", "}");
  TokenType::add(TokenType::DoubleQuote, "DoubleQuote", "\"");
  TokenType::add(TokenType::Error, "Error");
  TokenType::add(TokenType::Func, "Func", "$");
  TokenType::add(TokenType::Newline, "Newline", {"\n", ";"});
  TokenType::add(TokenType::LeftParen, "LeftParen", "(");
  TokenType::add(TokenType::RightParen, "RightParen", ")");
  TokenType::add(TokenType::Stop, "Stop");
  TokenType::add(TokenType::String, "String");
  TokenType::add(TokenType::Symbol, "Symbol");
}

std::ostream& terse::operator<<(std::ostream& out, TokenType::Value type) {
  return out << static_cast<TokenType>(type).to_string();
}
