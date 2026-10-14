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
      Lexer(std::istream& stream);

      // Return next `Token` in buffer.
      TokenResult next();

    private:
      BufferedReader reader_;
      int line_{1};
      int col_{1};
      int prev_col_{1};

      void skip_whitespace() noexcept;

      bool is_space(int c) const noexcept;

      bool is_word_boundary(int c) const noexcept;

      bool is_end_of_line(int c) const noexcept;

      void readline() noexcept;

      char get() noexcept;

      void putback() noexcept;
  };
}

//*****************************************************************************
// class Lexer
//*****************************************************************************

inline terse::Lexer::Lexer(std::istream& stream) : reader_{stream, 4096} {}

inline void terse::Lexer::skip_whitespace() noexcept {
  char c;
  do {
    c = get();
  } while (c == ' ' or c == '\t');
  putback();
}

inline bool terse::Lexer::is_space(int c) const noexcept {
  return (c == ' ' || c == '\t');
}

inline bool terse::Lexer::is_word_boundary(int c) const noexcept {
  const char* word_boundaries = " \n;#\t\"";

  for (; *word_boundaries; ++word_boundaries) {
    if (c == *word_boundaries) {
      return true;
    }
  }

  return false;
}

inline bool terse::Lexer::is_end_of_line(int c) const noexcept {
  return (c == '\n' || c == EOF);
}

inline void terse::Lexer::readline() noexcept {
  char c;
  while ((c = get()) != '\n');
}

inline char terse::Lexer::get() noexcept {
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

inline void terse::Lexer::putback() noexcept {
  reader_.putback();

  --col_;
  if (col_ < 1) {
    --line_;
    col_ = prev_col_;
  }
}
