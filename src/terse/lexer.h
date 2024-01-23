#pragma once

#include "token.h"

namespace terse {
  class Lexer {
    public:
      Lexer(const char *buf, size_t size);

      // Return next `Token` in buffer.
      Token next();

    private:
      static const char COMMENT{'#'};

      const char *const buf_;   // Start of buffer.
      const char *cur_;         // Current position in buffer.
      const char *const end_;   // End of buffer.

      bool at_end_of_statement_{false};

      // Move `cur_` to next non-whitespace character.
      void skip_whitespace();

      // Move `cur_` to next non-space character.
      void skip_space();

      // Move `cur_` to end of line.
      void skip_line();

      // Move `cur_` to next character. Skips whitespace and comments.
      void seek();
  };
}
