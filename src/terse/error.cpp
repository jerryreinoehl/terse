#include "error.hpp"

using terse::ParseError;
using terse::TokenType;

ParseError::ParseError(int line, int col, utl::string_view message)
  : line_{line}, col_{col}, message_{message} {}

  utl::unexpected<ParseError> terse::invalid_map_source_error(int line, int col) {
  return utl::unexpected<ParseError>{line, col, "Source must be a single word"};
}

utl::unexpected<ParseError> terse::unexpected_token_error(int line, int col, TokenType expected, TokenType actual) {
  return utl::unexpected<ParseError>{
    line,
    col,
    std::string{"Expected token of type "} + std::string{expected.to_string()}
      + " but got " + std::string{actual.to_string()}
  };
}

utl::unexpected<ParseError> terse::malformed_statement_error(int line, int col) {
  return utl::unexpected<ParseError>{line, col, "Malformed statement"};
}

utl::unexpected<ParseError> terse::symbol_not_found_error(int line, int col, utl::string_view symbol) {
  return utl::unexpected<ParseError>{line, col, "Symbol `" + symbol.to_string() + "` not found"};
}
