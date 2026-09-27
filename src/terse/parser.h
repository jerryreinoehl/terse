#pragma once

#include "emission.h"
#include "error.h"
#include "grammar.h"
#include "lexer.h"
#include "token.h"

#include <memory>
#include <optional>

namespace terse {

  class Parser {
    public:
      Parser(std::istream& stream) : lexer_{stream} {}
      Parser(std::istream&& stream) : Parser{stream} {}
      std::optional<TokenMap> parse();

    private:
      Lexer lexer_;

      std::unique_ptr<const Token> peak_token_{};
      std::unique_ptr<const Token> putback_token_{};

      int line_{};
      int col_{};

      TokenResult next();
      TokenResult next_ignore_newlines();

      TokenResult next_require_type(TokenType type);

      void putback(std::unique_ptr<const Token> token);

      ParseResult<const Token*> peak();
      ParseResult<const Token*> peak_ignore_newlines();

      ParseResult<Statements> parse_statements();
      ParseResult<Statement> parse_statement();
      ParseResult<MapStatement> parse_map_statement();
      ParseResult<WordExpression> parse_word_expression();
      //ParseResult<WordExpression> parse_word_expression();
  };

}
