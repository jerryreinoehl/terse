#pragma once

#include "token.h"

class Lexer {
  public:
    Lexer(const char *buf, size_t size);
    Token next();

  private:
    const char *const buf_; // Start of buffer.
    const char *cur_;       // Current position in buffer.
    const char *end_;       // End of buffer.

    const char *tok_begin_; // Beginning position of token in buffer.
    const char *tok_end_;   // End position of token in buffer.

    bool eos_ = false;      // Indicates end of statement.

    // Move `cur_` to next non-space character.
    void seek_space();
};
