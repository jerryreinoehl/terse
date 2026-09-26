#pragma once

#include "expected.h"
#include "token.h"

#include <memory>
#include <string>

namespace terse {
  class ParseError {
    public:
      ParseError(int line, int col, std::string message);

      std::string to_string() const noexcept {
        return std::string{} + std::to_string(line_) + ":" + std::to_string(col_) + " " + message_;
      }

    private:
      int line_;
      int col_;
      std::string message_;
  };

  template <typename T>
  using ParseResult = expected<T, ParseError>;
  using TokenResult = ParseResult<std::unique_ptr<const Token>>;
}
