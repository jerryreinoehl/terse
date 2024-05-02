#pragma once

#include <string>

namespace terse {

  enum class TokenType {
    ARM,
    BRACE_LEFT,
    BRACE_RIGHT,
    DOUBLE_QUOTE,
    FUNC,
    NEWLINE,
    PAREN_LEFT,
    PAREN_RIGHT,
    STOP,
    WORD,
  };

  class Token {
    public:
      Token(TokenType type);

      TokenType type() const noexcept;

    private:
      TokenType type_;
  };

  class WordToken : public Token {
    public:
      WordToken(const std::string& value);

      std::string value() const noexcept;

    private:
      std::string value_{};
  };

}
