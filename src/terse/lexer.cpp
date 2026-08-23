#include "lexer.h"

#include <memory>
#include <sstream>

using terse::Lexer;
using terse::Token;

std::unique_ptr<const Token> Lexer::next() {
  char c;
  std::stringstream lexeme{};
  int line, col;

  skip_whitespace();

  while (true) {
    line = line_;
    col = col_;
    c = get();

    if (c == '\n') {
      return std::make_unique<const Token>(TokenType::NEWLINE, line, col);
    } else if (c == EOF) {
      return std::make_unique<const Token>(TokenType::STOP, line, col);
    } else if (c == '#') {
      readline();
      return std::make_unique<const Token>(TokenType::NEWLINE, line, col);
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
          return std::make_unique<const ErrorToken>("Unterminated string", line, col);
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

  TokenType type = TokenType::from_lexeme(lexeme.str());

  if (type == TokenType::NONE) {
    return std::make_unique<const StringToken>(lexeme.str(), line, col);
  } else {
    return std::make_unique<const Token>(type, line, col);
  }
}
