#include "error.h"

using terse::ParseError;
using terse::TokenType;

ParseError::ParseError(int line, int col, std::string message)
  : line_{line}, col_{col}, message_{message} {}

unexpected<ParseError> terse::invalid_map_source_error(int line, int col) {
  return unexpected{ParseError{line, col, "Source must be a single word"}};
}

unexpected<ParseError> terse::unexpected_token_error(int line, int col, TokenType expected, TokenType actual) {
  return unexpected{ParseError{
    line,
    col,
    std::string{"Expected token of type "} + std::string{expected.to_string()}
      + " but got " + std::string{actual.to_string()}
  }};
}

unexpected<ParseError> terse::malformed_statement_error(int line, int col) {
  return unexpected{ParseError{line, col, "Malformed statement"}};
}
