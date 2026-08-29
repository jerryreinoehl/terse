#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>

namespace terse {

  class TokenType {
    public:
      static const TokenType NONE;
      static const TokenType ARM;
      static const TokenType BRACE_LEFT;
      static const TokenType BRACE_RIGHT;
      static const TokenType DOUBLE_QUOTE;
      static const TokenType ERROR;
      static const TokenType FUNC;
      static const TokenType NEWLINE;
      static const TokenType PAREN_LEFT;
      static const TokenType PAREN_RIGHT;
      static const TokenType STOP;
      static const TokenType STRING;


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

  template <typename T>
  struct TokenTraits;

  class Token {
    public:
      Token(TokenType type, int line, int col) noexcept;

      const TokenType type() const noexcept;
      int line() const noexcept;
      int col() const noexcept;

      template <typename T>
      bool is() const {
        return type_ == TokenTraits<T>::type();
      }

      template <typename T>
      const T& as() const {
        assert(type() == TokenTraits<T>::type());
        return *static_cast<const T*>(this);
      }

    private:
      TokenType type_;
      int line_;
      int col_;
  };


  class StringToken : public Token {
    public:
      StringToken(const std::string& value, int line, int col);

      std::string value() const noexcept;

    private:
      std::string value_{};
  };

  class ErrorToken : public Token {
    public:
      ErrorToken(const std::string& error, int line, int col);

      std::string error() const noexcept;

    private:
      std::string error_{};
  };

  template <>
  struct TokenTraits<StringToken> {
    static const TokenType &type() noexcept {
      return TokenType::STRING;
    }
  };

  template <>
  struct TokenTraits<ErrorToken> {
    static const TokenType &type() noexcept {
      return TokenType::ERROR;
    }
  };

}
