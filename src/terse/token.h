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

      TokenType() noexcept {}
      constexpr TokenType(Value value) noexcept : value{value} {}
      constexpr operator Value() const noexcept { return value; }

      std::string_view to_string() const;

    private:
      struct token_type_t {};

      static TokenType token_type;

      Value value;

      TokenType(token_type_t);
  };

  std::ostream& operator<<(std::ostream& out, TokenType::Value type);

  class Token {
    public:
      Token(TokenType type, int line, int col) noexcept;

      // This token's type.
      TokenType type() const noexcept;

      // The line and column numbers where this token was found. (1-based).
      int line() const noexcept;
      int col() const noexcept;

      // Returns `true` if this token is of token class `T`, and `false`
      // otherwise.
      //   e.g.
      //   Token token = StringToken{...};
      //   assert(token.is<StringToken>());
      //   assert(!token.is<ErrorToken>());
      template <typename T>
      bool is() const noexcept;

      // Casts this token to token class `T`.
      // Asserts that this token's `type()` matches the `type()` of `T`.
      //   e.g.
      //   Token token = StringToken{...};
      //   std::cout << token.as<StringToken>().value() << "\n";
      template <typename T>
      const T& as() const;

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

  template <typename T>
  struct token_traits;

  template <>
  struct token_traits<StringToken> {
    static constexpr TokenType type{TokenType::String};
  };

  template <>
  struct token_traits<ErrorToken> {
    static constexpr TokenType type{TokenType::Error};
  };

}

//*****************************************************************************
// class Token
//*****************************************************************************

inline terse::TokenType terse::Token::type() const noexcept {
  return type_;
}

inline int terse::Token::line() const noexcept {
  return line_;
}

inline int terse::Token::col() const noexcept {
  return col_;
}

template <typename T>
bool terse::Token::is() const noexcept {
  return type_ == token_traits<T>::type;
}

template <typename T>
const T& terse::Token::as() const {
  assert(type_ == token_traits<T>::type);
  return static_cast<const T&>(*this);
}
