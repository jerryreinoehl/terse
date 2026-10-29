#include "parser.hpp"
#include "unwrap.hpp"

using terse::Environment;
//using terse::MapStatement;
using terse::ParseResult;
using terse::Parser;
using terse::Statement;
using terse::Statements;
using terse::Token;
using terse::TokenResult;
using terse::WordExpression;

ParseResult<const Environment*> Parser::parse() {
  unwrap(parse_statements());
  return &environment_;
}

const Environment& Parser::environment() const noexcept {
  return environment_;
}

TokenResult Parser::next() {
  if (putback_token_) {
    return std::move(putback_token_);
  }

  if (peak_token_) {
    return std::move(peak_token_);
  }

  auto token = unwrap(lexer_.next());
  line_ = token->line();
  col_ = token->col();

  return token;
}

TokenResult Parser::next_ignore_newlines() {
  do {
    auto token = unwrap(next());
    if (token->type() != TokenType::Newline) {
      return token;
    }
  } while (true);
}

TokenResult Parser::next_require_type(TokenType type) {
  auto token = unwrap(next_ignore_newlines());

  unwrap(token->type() == type, unexpected_token_error(line_, col_, type, token->type()));

  return token;
}

void Parser::putback(TokenPtr token) {
  // TODO: Check to ensure `putback_token_` is empty (nullptr).
  putback_token_ = std::move(token);
}

ParseResult<const Token*> Parser::peak() {
  if (putback_token_) {
    return putback_token_.get();
  }

  if (!peak_token_) {
    peak_token_ = unwrap(lexer_.next());
  }

  line_ = peak_token_->line();
  col_ = peak_token_->col();
  return peak_token_.get();
}

ParseResult<const Token*> Parser::peak_ignore_newlines() {
  if (putback_token_ && putback_token_->type() != TokenType::Newline) {
    return putback_token_.get();
  } else {
    putback_token_.reset();
  }

  if (peak_token_ && peak_token_->type() != TokenType::Newline) {
    return peak_token_.get();
  } else {
    peak_token_.reset();
  }

  do {
    auto token = unwrap(next());
    if (token->type() != TokenType::Newline) {
      peak_token_ = std::move(token);
      break;
    }
  } while (true);

  line_ = peak_token_->line();
  col_ = peak_token_->col();
  return peak_token_.get();
}

ParseResult<Statements> Parser::parse_statements() {
  Statements statements{};

  do {
    if (unwrap(peak_ignore_newlines())->type() == TokenType::Stop) {
      break;
    }

    statements.add(unwrap(parse_statement()));
  } while (true);

  return statements;
}

ParseResult<Statement> Parser::parse_statement() {
  TokenPtr token = unwrap(next_require_type(TokenType::String));
  unwrap(unwrap(peak())->type() == TokenType::Arm, malformed_statement_error(line_, col_));
  putback(std::move(token));

  return unwrap(parse_map_statement());
}

ParseResult<Statement> Parser::parse_map_statement() {
  WordExpression source;
  WordExpression target;
  Statements statements;

  source = unwrap(parse_word_expression());
  // `source` must contain one and only one word to be valid.
  unwrap(source.size() == 1, invalid_map_source_error(line_, col_));
  // `source` must be a `TokenType::String` type.
  unwrap(source.first().type() == TokenType::String, invalid_map_source_error(line_, col_));
  // TODO: Add two different errors for source size and source type. ^^^

  unwrap(next_require_type(TokenType::Arm));

  target = unwrap(parse_word_expression());

  Translation& translation = environment_.add_translation(std::move(source).first(), std::move(target).words());

  if (unwrap(peak_ignore_newlines())->type() == TokenType::LeftBrace) {
    unwrap(next_require_type(TokenType::LeftBrace));

    environment_.push_map(translation.map());

    while (unwrap(peak_ignore_newlines())->type() != TokenType::RightBrace) {
      statements.add(unwrap(parse_statement()));
    }

    unwrap(next_require_type(TokenType::RightBrace));

    environment_.pop_map();
  }

  return Statement{};
}

ParseResult<WordExpression> Parser::parse_word_expression() {
  WordExpression expression;
  bool in_word_expr{true};

  do {
    switch (unwrap(peak())->type()) {
      case TokenType::String:
      case TokenType::Symbol:
        expression.add(unwrap(next()));
        break;
      default:
        in_word_expr = false;
        break;
    }
  } while (in_word_expr);

  return expression;
}
