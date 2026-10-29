#pragma once

#include <vector>
#include <utl/string_view.hpp>

#include "token.hpp"

namespace terse {

  class Statement;
  class Statements;
  class Expression;
  class WordExpression;

  class Expression {
    public:
    private:
  };

  class WordExpression : public Expression {
    public:
      WordExpression() : words_{} {}
      WordExpression(const WordExpression&) = delete;
      WordExpression(WordExpression&& other) : words_{std::move(other.words_)} {}

      WordExpression& operator=(const WordExpression&) = delete;
      WordExpression& operator=(WordExpression&& other) noexcept;

      void add(TokenPtr token);
      size_t size() const noexcept;

      const std::vector<TokenPtr>& words() const& noexcept;
      std::vector<TokenPtr> words() && noexcept;

      const Token& operator[](size_t index) const;

      const Token& first() const& noexcept;
      TokenPtr first() && noexcept;

    private:
      std::vector<TokenPtr> words_{};
  };

  class Statement {
    public:
    private:
  };

  class Statements : public Statement {
    public:
      void add(const Statement& statement);

    private:
      std::vector<Statement> statements_{};
  };
}
