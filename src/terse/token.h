#pragma once

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace terse {

  class TokenType {
    public:
      static const TokenType NONE;
      static const TokenType ARM;
      static const TokenType BRACE_LEFT;
      static const TokenType BRACE_RIGHT;
      static const TokenType DOUBLE_QUOTE;
      static const TokenType FUNC;
      static const TokenType NEWLINE;
      static const TokenType PAREN_LEFT;
      static const TokenType PAREN_RIGHT;
      static const TokenType STOP;
      static const TokenType WORD;

      static TokenType from_lexeme(const std::string& lexeme);

      TokenType() noexcept {}

      TokenType(const std::string_view name) : value_{next_value++} {
        names.push_back(name);
      }

      TokenType(const std::string_view lexeme, const std::string_view name) : TokenType{name} {
        map[lexeme] = *this;
      }

      std::string to_string() const noexcept {
        return std::string{names[value_]};
      }

      bool operator==(const TokenType other) const noexcept {
        return value_ == other.value_;
      }

      bool operator!=(const TokenType other) const noexcept {
        return value_ != other.value_;
      }

    private:
      using Value = unsigned char;

      static Value next_value;
      static std::map<std::string_view, TokenType> map;
      static std::vector<std::string_view> names;

      Value value_;
  };

  std::ostream& operator<<(std::ostream& out, TokenType type);

  class Token {
    public:
      Token(TokenType type, int line, int col) noexcept;

      const TokenType type() const noexcept;
      int line() const noexcept;
      int col() const noexcept;

    private:
      TokenType type_;
      int line_;
      int col_;
  };

  class WordToken : public Token {
    public:
      WordToken(const std::string& value, int line, int col);

      std::string value() const noexcept;

    private:
      std::string value_{};
  };

}
