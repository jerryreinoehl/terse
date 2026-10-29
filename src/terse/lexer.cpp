#include "lexer.hpp"

#include <sstream>
#include <utl/char_iterator.hpp>

using terse::Lexer;
using terse::Token;
using terse::TokenResult;

TokenResult Lexer::next() {
  char c;
  std::stringstream lexeme{};
  std::string lexeme_str{};
  utl::const_char_iterator it{};
  int line, col;

  skip_whitespace();

  while (true) {
    line = line_;
    col = col_;
    c = get();

    if (c == '\n' || c == ';') {
      return Token::make_ptr(TokenType::Newline, line, col);
    } else if (c == EOF) {
      return Token::make_ptr(TokenType::Stop, line, col);
    } else if (c == '#') {
      readline();
      return Token::make_ptr(TokenType::Newline, line, col);
    } else {
      break;
    }
  }

  while (true) {
    if (c == '"') {
      // Scan until next '"'.
      while ((c = get()) != '"') {
        if (is_end_of_line(c)) {
          putback();
          return unexpected<ParseError>{line, col, "Unterminated string"};
        }
        lexeme << c;
      }
      c = get();
    } else if (is_word_boundary(c)) {
      break;
    } else {
      lexeme << c;
      c = get();
    }
  }

  putback();

  lexeme_str = lexeme.str();
  it = utl::const_char_iterator{lexeme_str.c_str()};

  if (*it == '$') {
    ++it; // Skip "$"
    std::cout << "Symbol is " << it.view() << '\n';
    return SymbolToken::make_ptr(it.view(), line, col);
  }

  TokenType type = TokenType::from_lexeme(it.view());

  if (type == TokenType::None) {
    return StringToken::make_ptr(it.view(), line, col);
  } else {
    return Token::make_ptr(type, line, col);
  }
}
