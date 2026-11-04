#pragma once

#include "token.hpp"

#include <string>
#include <utl/expected.hpp>
#include <utl/string_view.hpp>

namespace terse {
  class ParseError {
    public:
      ParseError(int line, int col, utl::string_view message);

      std::string to_string() const noexcept {
        return std::to_string(line_) + ":" + std::to_string(col_) + " " + message_;
      }

    private:
      int line_;
      int col_;
      std::string message_;
  };

  template <typename T>
  using ParseResult = utl::expected<T, ParseError>;
  using TokenResult = ParseResult<TokenPtr>;

  utl::unexpected<ParseError> invalid_map_source_error(int line, int col);
  utl::unexpected<ParseError> unexpected_token_error(int line, int col, TokenType expected, TokenType actual);
  utl::unexpected<ParseError> malformed_statement_error(int line, int col);

  utl::unexpected<ParseError> symbol_not_found_error(int line, int col, utl::string_view symbol);
}
