#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <utl/type_traits.hpp>
#include <utl/string_view.hpp>

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
        Symbol,
        None, // Keep this last as it is used for TokenType element count.
      };

      // The number of `TokenType` values.
      static constexpr std::size_t value_count{static_cast<std::size_t>(TokenType::None + 1)};

      // Maps `TokenType`s to their printable names.
      static std::vector<utl::string_view> names;

      // Maps a string lexeme to a `TokenType`.
      static std::map<utl::string_view, TokenType> map;

      // Update `names` and `map` with a new `TokenType`.
      static void add(TokenType type, utl::string_view name);
      static void add(TokenType type, utl::string_view name, utl::string_view lexeme);
      static void add(TokenType type, utl::string_view name, std::initializer_list<utl::string_view> lexemes);

      // Gets the `TokenType` corresponding to the lexeme `lexeme`.
      // Returns `TokenType::None` if the lexeme doesn't match another
      // `TokenType`.
      static TokenType from_lexeme(utl::string_view lexeme);

      TokenType() noexcept {}
      constexpr TokenType(Value value) noexcept : value{value} {}
      constexpr operator Value() const noexcept { return value; }

      utl::string_view to_string() const;

    private:
      struct token_type_t {};

      static TokenType token_type;

      Value value{TokenType::None};

      TokenType(token_type_t);
  };

  std::ostream& operator<<(std::ostream& out, TokenType::Value type);

  class Token;
  class TokenDeleter;

  using TokenPtr = std::unique_ptr<Token, TokenDeleter>;
  using ConstTokenPtr = std::unique_ptr<const Token, TokenDeleter>;

  class Token {
    public:

      template <typename... Args>
      utl::enable_if_t<utl::is_nothrow_constructible_v<Token, Args...>(), TokenPtr>
      static make_ptr(Args... args) noexcept;

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
      utl::enable_if_t<!utl::is_pointer_v<T>(), const T&>
      as() const;

      template <typename T>
      utl::enable_if_t<!utl::is_pointer_v<T>(), T&>
      as();

      // Casts this token to a pointer to token class `T`.
      // Asserts that this token's `type()` matches the `type()` of `T`.
      //   e.g.
      //   const Token *token = new StringToken{...};
      //   std::cout << token->as<const StringToken*>()->value() << '\n';
      //   delete token->as<const StringToken*>();
      template <typename T>
      utl::enable_if_t<utl::is_pointer_v<T>(), T>
      as() const;

      template <typename T>
      utl::enable_if_t<utl::is_pointer_v<T>(), T>
      as();

    private:
      int line_;
      int col_;
      TokenType type_;
  };


  class StringToken : public Token {
    public:

      template <typename... Args>
      utl::enable_if_t<utl::is_constructible_v<StringToken, Args...>(), TokenPtr>
      static make_ptr(Args... args) noexcept;

      StringToken(utl::string_view value, int line, int col) noexcept;

      utl::string_view value() const noexcept;

    private:
      std::string value_{};
  };

  class ErrorToken : public Token {
    public:
      ErrorToken(utl::string_view error, int line, int col) noexcept;

      utl::string_view error() const noexcept;

    private:
      std::string error_{};
  };

  class SymbolToken : public Token {
    public:
      template <typename... Args>
      utl::enable_if_t<utl::is_constructible_v<SymbolToken, Args...>(), TokenPtr>
      static make_ptr(Args... args) noexcept;

      SymbolToken(utl::string_view symbol, int line, int col) noexcept;

      utl::string_view symbol() const noexcept;

    private:
      std::string symbol_{};
  };

  // Deleter to probably delete a `Token` based on its `TokenType`.
  struct TokenDeleter {
    void operator()(const Token *p);
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
// class TokenType
//*****************************************************************************

inline void terse::TokenType::add(TokenType type, utl::string_view name) {
  TokenType::names[type] = name;
}

inline void terse::TokenType::add(TokenType type, utl::string_view name, utl::string_view lexeme) {
  TokenType::names[type] = name;
  TokenType::map[lexeme] = type;
}

inline void terse::TokenType::add(TokenType type, utl::string_view name, std::initializer_list<utl::string_view> lexemes) {
  TokenType::names[type] = name;
  for (const auto& lexeme : lexemes) {
    TokenType::map[lexeme] = type;
  }
}

inline terse::TokenType terse::TokenType::from_lexeme(utl::string_view lexeme) {
  if (TokenType::map.count(lexeme) > 0) {
    return TokenType::map[lexeme];
  }
  return TokenType::None;
}

inline utl::string_view terse::TokenType::to_string() const {
  return TokenType::names[value];
}

//*****************************************************************************
// class Token
//*****************************************************************************

template <typename... Args>
utl::enable_if_t<utl::is_nothrow_constructible_v<terse::Token, Args...>(), terse::TokenPtr>
inline terse::Token::make_ptr(Args... args) noexcept {
  return TokenPtr{new Token{std::forward<Args>(args)...}};
}

inline terse::Token::Token(TokenType type, int line, int col) noexcept
  : line_{line}, col_{col}, type_{type} {}

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
inline bool terse::Token::is() const noexcept {
  return type_ == token_traits<T>::type;
}

template <typename T>
utl::enable_if_t<!utl::is_pointer_v<T>(), const T&>
inline terse::Token::as() const {
  assert(type_ == token_traits<utl::base_type_t<T>>::type);
  return static_cast<const T&>(*this);
}

template <typename T>
utl::enable_if_t<!utl::is_pointer_v<T>(), T&>
inline terse::Token::as() {
  assert(type_ == token_traits<utl::base_type_t<T>>::type);
  return static_cast<T&>(*this);
}

template <typename T>
utl::enable_if_t<utl::is_pointer_v<T>(), T>
inline terse::Token::as() const {
  assert(type_ == token_traits<utl::base_type_t<T>>::type);
  return static_cast<const T>(this);
}

template <typename T>
utl::enable_if_t<utl::is_pointer_v<T>(), T>
inline terse::Token::as() {
  assert(type_ == token_traits<utl::base_type_t<T>>::type);
  return static_cast<T>(this);
}

//*****************************************************************************
// class StringToken
//*****************************************************************************

template <typename... Args>
utl::enable_if_t<utl::is_constructible_v<terse::StringToken, Args...>(), terse::TokenPtr>
inline terse::StringToken::make_ptr(Args... args) noexcept {
  return TokenPtr{new StringToken{std::forward<Args>(args)...}};
}

inline terse::StringToken::StringToken(utl::string_view value, int line, int col) noexcept
  : Token{TokenType::String, line, col}, value_{value} {}

inline utl::string_view terse::StringToken::value() const noexcept {
  return value_;
}

//*****************************************************************************
// class ErrorToken
//*****************************************************************************

inline terse::ErrorToken::ErrorToken(utl::string_view error, int line, int col) noexcept
  : Token{TokenType::Error, line, col}, error_{error} {}

inline utl::string_view terse::ErrorToken::error() const noexcept {
  return error_;
}

//*****************************************************************************
// class SymbolToken
//*****************************************************************************

template <typename... Args>
utl::enable_if_t<utl::is_constructible_v<terse::SymbolToken, Args...>(), terse::TokenPtr>
inline terse::SymbolToken::make_ptr(Args... args) noexcept {
  return TokenPtr{new SymbolToken{std::forward<Args>(args)...}};
}

inline terse::SymbolToken::SymbolToken(utl::string_view symbol, int line, int col) noexcept
  : Token{TokenType::Symbol, line, col}, symbol_{symbol} {}

inline utl::string_view terse::SymbolToken::symbol() const noexcept {
  return symbol_;
}

//*****************************************************************************
// class TokenDeleter
//*****************************************************************************

inline void terse::TokenDeleter::operator()(const Token *p) {
  switch (p->type()) {
    case TokenType::Error:
      delete p->as<const ErrorToken*>();
      break;

    case TokenType::String:
      delete p->as<const StringToken*>();
      break;

    case TokenType::Symbol:
      delete p->as<const SymbolToken*>();
      break;

    default:
      delete p;
      break;
  }
}
