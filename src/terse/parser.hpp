#pragma once

#include "environment.hpp"
#include "error.hpp"
#include "grammar.hpp"
#include "lexer.hpp"
#include "token.hpp"

#include <memory>

namespace terse {

  class Parser {
    public:
      Parser(std::istream& stream) : lexer_{stream} {}

      // Parse the `stream` and return a `ParseResult` containing a pointer to
      // the constructed `Environment`.
      ParseResult<const Environment*> parse();

      // Returns a const reference to the current `Environment`.
      const Environment& environment() const noexcept;

    private:
      // The lexer from which token are read.
      Lexer lexer_;

      // The environment being constructed during parsing.
      Environment environment_{};

      // Holds tokens for `peak`ing and `putback`.
      std::unique_ptr<const Token> peak_token_{};
      std::unique_ptr<const Token> putback_token_{};

      // Current line and col as obtained from last read token.
      int line_{};
      int col_{};

      // Returns the next token in the stream.
      // `next_ignore_newlines` continues to consume tokens until a token of a
      // type other than `TokenType::Newline` is found.
      TokenResult next();
      TokenResult next_ignore_newlines();

      // Return the next token in the stream if its type is equivalent to
      // `type`, otherwise it returns an error.
      TokenResult next_require_type(TokenType type);

      // Puts the token `token` back to be returned on the next call to `next`.
      void putback(std::unique_ptr<const Token> token);

      // Returns the next token without consuming it such that a subsequent
      // call to next returns the same token.
      // `peak_ignore_newlines` will consume tokens of type
      // `TokenType::Newline`.
      ParseResult<const Token*> peak();
      ParseResult<const Token*> peak_ignore_newlines();

      ParseResult<Statements> parse_statements();
      ParseResult<Statement> parse_statement();
      ParseResult<MapStatement> parse_map_statement();
      ParseResult<WordExpression> parse_word_expression();
  };

}
