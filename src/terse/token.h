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
      enum Value {
        Arm,
        LeftBrace,
        RightBrace,
        DoubleQuote,
        Error,
        Func,
        Newline,
        LeftParen,
        RightParen,
        Stop,
        String,
        None, // Keep this last as it is used for TokenType element count.
      };

      static constexpr std::size_t value_count{static_cast<std::size_t>(TokenType::None + 1)};
      static std::vector<std::string_view> names;
      static std::map<std::string_view, TokenType> map;

      static void add(TokenType type, std::string_view name);
      static void add(TokenType type, std::string_view name, std::string_view lexeme);

      static TokenType from_lexeme(std::string_view lexeme);

      static TokenType& get_instance() noexcept;

      TokenType() noexcept {}
      constexpr TokenType(Value value) noexcept : value{value} {}
      constexpr operator Value() const noexcept { return value; }

      std::string_view to_string() const;

    private:
      struct token_type_t {};

      Value value;

      TokenType(token_type_t);
  };

  std::ostream& operator<<(std::ostream& out, TokenType::Value type);

  template <typename T>
  struct TokenTraits;

  class Token {
    public:
      Token(TokenType type, int line, int col) noexcept;

      TokenType type() const noexcept;
      int line() const noexcept;
      int col() const noexcept;

      template <typename T>
      bool is() const noexcept {
        return type_ == TokenTraits<T>::type;
      }

      template <typename T>
      const T& as() const {
        assert(type_ == TokenTraits<T>::type);
        return static_cast<const T&>(*this);
      }

    private:
      int line_;
      int col_;
      TokenType type_;
  };


  class StringToken : public Token {
    public:
      StringToken(const std::string& value, int line, int col) noexcept;

      std::string value() const noexcept;

    private:
      std::string value_{};
  };

  class ErrorToken : public Token {
    public:
      ErrorToken(const std::string& error, int line, int col) noexcept;

      std::string error() const noexcept;

    private:
      std::string error_{};
  };

  template <>
  struct TokenTraits<StringToken> {
    static constexpr TokenType type{TokenType::String};
  };

  template <>
  struct TokenTraits<ErrorToken> {
    static constexpr TokenType type{TokenType::Error};
  };

}
