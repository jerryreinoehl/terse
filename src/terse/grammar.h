#pragma once

#include <string>
#include <vector>

#include "token.h"

namespace idx {
  struct last_t {};
  [[maybe_unused]] static last_t last;
}

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
      void add(const std::string& word);
      size_t size() const noexcept;

      const std::string& operator[](size_t index) const;
      const std::string& operator[](idx::last_t) const;

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
