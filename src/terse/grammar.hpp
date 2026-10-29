#pragma once

#include <vector>
#include <utl/string_view.hpp>

#include "token.hpp"

namespace terse {

  class Statement;
  class Statements;
  class MapStatement;
  class Expression;
  class WordExpression;

  class Expression {
    public:
    private:
  };

  class WordExpression : public Expression {
    public:
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

  class MapStatement : public Statement {
    public:
      MapStatement(WordExpression source, WordExpression target, Statements statements)
        : source{source}, target{target}, statements{statements} {}

      WordExpression source{};
      WordExpression target{};
      Statements statements{};

    private:
  };
}
