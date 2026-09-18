#include "lexer.h"

#include <memory>
#include <sstream>

using terse::Lexer;
using terse::LexerError;
using terse::LexerResult;
using terse::Token;


LexerError::LexerError(int line, int col, std::string message) : line_{line}, col_{col}, message_{message} {}


LexerResult<std::unique_ptr<const Token>> Lexer::next() {
  char c;
  std::stringstream lexeme{};
  int line, col;

  skip_whitespace();

  while (true) {
    line = line_;
    col = col_;
    c = get();

    if (c == '\n') {
      return {std::make_unique<const Token>(TokenType::Newline, line, col)};
    } else if (c == EOF) {
      return {std::make_unique<const Token>(TokenType::Stop, line, col)};
    } else if (c == '#') {
      readline();
      return {std::make_unique<const Token>(TokenType::Newline, line, col)};
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
          return unexpected<LexerError>{line, col, "Unterminated string"};
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

  if (type == TokenType::None) {
    return {std::make_unique<const StringToken>(lexeme.str(), line, col)};
  } else {
    return {std::make_unique<const Token>(type, line, col)};
  }
}
