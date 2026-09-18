#pragma once

#include "bufferedreader.h"
#include "error.h"
#include "expected.h"
#include "token.h"

#include <iostream>
#include <memory>

namespace terse {

  class Lexer {
    public:
      Lexer(std::istream& stream) : stream_{stream}, reader_{stream, 4096} {}
      Lexer(std::istream&& stream) : Lexer{stream} {}

      // Return next `Token` in buffer.
      TokenResult next();

    private:
      std::istream& stream_; // Do we really need to keep a ref of this.

      BufferedReader reader_;
      char c_;
      int line_{1};
      int col_{1};
      int prev_col_{1};

      void skip_whitespace() noexcept {
        char c;
        do {
          c = get();
        } while (c == ' ' or c == '\t');
        putback();
      }

      bool is_space(int c) const noexcept {
        return (c == ' ' || c == '\t');
      }

      bool is_word_boundary(int c) const noexcept {
        return (c == ' ' || c == '\n' || c == '\t' || c == '#' || c == '"');
      }

      bool is_end_of_line(int c) const noexcept {
        return (c == '\n' || c == EOF);
      }

      void readline() noexcept {
        char c;
        while ((c = get()) != '\n');
      }

      char get() noexcept {
        char c = reader_.get();

        if (c == '\n') {
          ++line_;
          prev_col_ = col_;
          col_ = 1;
        } else {
          ++col_;
        }

        return c;
      }

      void putback() noexcept {
        reader_.putback();

        --col_;
        if (col_ < 1) {
          --line_;
          col_ = prev_col_;
        }
      }
  };
}
