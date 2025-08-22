#include "lexer.h"
#include "bufferedreader.h"

#include <sstream>

using terse::Lexer;
using terse::Token;

Token Lexer::next() {
  char c;
  std::stringstream lexeme{};
  int line, col;

  skip_whitespace();

  while (true) {
    line = line_;
    col = col_;
    c = get();

    if (c == '\n') {
      return {TokenType::NEWLINE, line, col};
    } else if (c == EOF) {
      return {TokenType::STOP, line, col};
    } else if (c == '#') {
      readline();
      return {TokenType::NEWLINE, line, col};
    } else {
      break;
    }
  }

  do {
    lexeme << c;
  } while (!is_special(c = get()));

  putback();
  std::cout << "lexeme is " << lexeme.str() << '\n';

  TokenType type = TokenType::from_lexeme(lexeme.str());
  std::cout << "type is " << type << '\n';

  if (type == TokenType::NONE) {
    return WordToken{lexeme.str(), line, col};
  } else {
    return {type, line, col};
  }
}
