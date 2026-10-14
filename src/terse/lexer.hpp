#pragma once

#include "bufferedreader.hpp"
#include "error.hpp"
#include "expected.hpp"
#include "token.hpp"

#include <iostream>
#include <memory>

namespace terse {

  class Lexer {
    public:
      Lexer(std::istream& stream) : reader_{stream, 4096} {}

      // Return next `Token` in buffer.
      TokenResult next();

    private:
      BufferedReader reader_;
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
        const char* word_boundaries = " \n;#\t\"";

        for (; *word_boundaries; ++word_boundaries) {
          if (c == *word_boundaries) {
            return true;
          }
        }

        return false;
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
