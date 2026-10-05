#include "parser.h"
#include "unwrap.h"

#include <memory>
#include <stack>

using terse::Parser;
using terse::StringToken;
using terse::Token;
using terse::TokenMap;
using terse::TokenResult;
using terse::ParseResult;
using terse::Statements;
using terse::Statement;
using terse::MapStatement;
using terse::WordExpression;

std::optional<TokenMap> Parser::parse() {
  std::stack<TokenMap> maps;
  std::stack<std::string> emissions;
  TokenMap map;
  std::unique_ptr<const Token> tok;
  std::string src;
  std::vector<std::string> tokens;
  Emission emission;

  ParseResult<Statements> statements = parse_statements();
  if (!statements) {
    std::cout << "Bad Statements\n";
    std::cout << std::move(statements).error().to_string() << '\n';
  }

  //do {
  //  tokens.clear();

  //  tok = next();
  //  std::cout << "first: " << tok->type() << '\n';
  //  if (tok->type() == TokenType::Stop) {
  //    break;
  //  }

  //  if (tok->type() != TokenType::String) {
  //    return {};
  //  }
  //  src = static_cast<const StringToken*>(tok.get())->value();
  //  std::cout << src << '\n';

  //  tok = next();
  //  if (tok->type() != TokenType::Arm) {
  //    return {};
  //  }
  //  std::cout << "got arm\n";

  //  while ((tok = next())->type() == TokenType::String) {
  //    std::cout << static_cast<const StringToken*>(tok.get())->value() << '\n';
  //    tokens.push_back(static_cast<const StringToken*>(tok.get())->value());
  //  }
  //  std::cout << "next: " << tok->type() << '\n';

  //  emission = Emission{tokens};
  //  map[src] = emission;

  //  if (tok->type() == TokenType::Newline) {
  //    tok = next();
  //    std::cout << "1: " << tok->type() << '\n';
  //  }

  //  if (tok->type() == TokenType::LeftBrace) {
  //    emissions.push(src);
  //    maps.push(map);
  //    map = TokenMap{};
  //  }
  //  else if (tok->type() == TokenType::RightBrace) {
  //    while (tok->type() == TokenType::RightBrace) {
  //      maps.top()[emissions.top()].set_map(map);
  //      map = maps.top();
  //      maps.pop();
  //      emissions.pop();
  //      tok = next();
  //    }
  //    putback(std::move(tok));
  //  } else {
  //    putback(std::move(tok));
  //  }
  //} while (tok->type() != TokenType::Stop);

  return map;
}

TokenResult Parser::next() {
  if (putback_token_) {
    return std::move(putback_token_);
  }

  if (peak_token_) {
    return std::move(peak_token_);
  }

  auto result = lexer_.next();
  ///
  if (result) {
    std::cout << "Reading: " << result->get()->type() << '\n';
    line_ = result->get()->line();
    col_ = result->get()->col();
    return std::move(*result);
  } else {
    std::cout << "Error getting result from lexer\n";
    return result;
  }
  ///
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

void Parser::putback(std::unique_ptr<const Token> token) {
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

  std::cout << "Peaking:: " << peak_token_->type() << '\n';
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
  std::cout << "[] parse_statements\n";

  Statements statements{};

  do {
    if (unwrap(peak_ignore_newlines())->type() == TokenType::Stop) {
      break;
    }

    statements.add(unwrap(parse_statement()));
  } while (true);

  std::cout << "FINISHED parsing config!!!!!\n";
  return statements;
}

ParseResult<Statement> Parser::parse_statement() {
  std::cout << "[] parse_statement\n";

  std::unique_ptr<const Token> token = unwrap(next_require_type(TokenType::String));
  unwrap(unwrap(peak())->type() == TokenType::Arm, malformed_statement_error(line_, col_));
  putback(std::move(token));

  return unwrap(parse_map_statement());
}

ParseResult<MapStatement> Parser::parse_map_statement() {
  std::cout << "[] parse_map_statement\n";

  WordExpression source;
  WordExpression target;
  Statements statements;

  source = unwrap(parse_word_expression());
  // `source` must contain one and only one word to be valid.
  unwrap(source.size() == 1, invalid_map_source_error(line_, col_));

  unwrap(next_require_type(TokenType::Arm));

  target = unwrap(parse_word_expression());

  Translation& translation = environment_.add_translation(source.first(), target.words());

  if (unwrap(peak_ignore_newlines())->type() == TokenType::LeftBrace) {
    unwrap(next_require_type(TokenType::LeftBrace));

    environment_.push_map(translation.map());

    while (unwrap(peak_ignore_newlines())->type() != TokenType::RightBrace) {
      statements.add(unwrap(parse_statement()));
    }

    unwrap(next_require_type(TokenType::RightBrace));

    environment_.pop_map();
  }

  return MapStatement{source, target, statements};
}

ParseResult<WordExpression> Parser::parse_word_expression() {
  std::cout << "[] parse_word_statement\n";

  WordExpression expression;

  do {
    if (unwrap(peak())->type() != TokenType::String) {
      break;
    }
    expression.add(unwrap(next())->as<StringToken>().value());
  } while (true);

  return expression;
}
