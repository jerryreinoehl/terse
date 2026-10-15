#pragma once

#include <string>
#include <vector>
#include <utl/string_view.hpp>

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
      void add(utl::string_view word);
      size_t size() const noexcept;

      const std::vector<std::string>& words() const noexcept;

      utl::string_view operator[](size_t index) const;

      utl::string_view first() const noexcept;

    private:
      std::vector<std::string> words_;
  };

  class Statement {
    public:
    private:
  };

  class Statements : public Statement {
    public:
      void add(const Statement& statement);

    private:
      std::vector<Statement> statements_;
  };

  class MapStatement : public Statement {
    public:
      MapStatement(WordExpression source, WordExpression target, Statements statements)
        : source{source}, target{target}, statements{statements} {}

      WordExpression source;
      WordExpression target;
      Statements statements;

    private:
  };
}
