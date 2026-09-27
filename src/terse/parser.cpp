#include "parser.h"
#include "unwrap.h"
#include "to_string.h"

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

  if (token->type() != type) {
    return unexpected<ParseError>{
      token->line(),
      token->col(),
      "Expected token of type " + std::string{type.to_string()} +
      " but got " + std::string{token->type().to_string()},
    };
  }

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
  return peak_token_.get();
}

ParseResult<const Token*> Parser::peak_ignore_newlines() {
  if (putback_token_ && putback_token_->type() != TokenType::Newline) {
    return putback_token_.get();
  } else {
    putback_token_ = std::unique_ptr<const Token>{};
  }

  if (peak_token_ && peak_token_->type() != TokenType::Newline) {
    return peak_token_.get();
  } else {
    peak_token_ = std::unique_ptr<const Token>{};
  }

  do {
    auto token = unwrap(next());
    if (token->type() != TokenType::Newline) {
      peak_token_ = std::move(token);
      break;
    }
  } while (true);

  return peak_token_.get();
}

ParseResult<Statements> Parser::parse_statements() {
  std::cout << "[] parse_statements\n";

  Statements statements{};

  do {
    auto token = unwrap(peak_ignore_newlines());
    if (token->type() == TokenType::Stop) {
      break;
    }

    statements.add(unwrap(parse_statement()));

  } while (true);

  std::cout << "FINISHED parsing config!!!!!\n";
  return statements;
}

ParseResult<Statement> Parser::parse_statement() {
  std::cout << "[] parse_statement\n";

  return unwrap(parse_map_statement());
}

ParseResult<MapStatement> Parser::parse_map_statement() {
  std::cout << "[] parse_map_statement\n";

  WordExpression source;
  WordExpression target;
  Statements statements;

  source = unwrap(parse_word_expression());

  unwrap(next_require_type(TokenType::Arm));

  target = unwrap(parse_word_expression());

  return MapStatement{source, target, statements};
}

ParseResult<WordExpression> Parser::parse_word_expression() {
  std::cout << "[] parse_word_statement\n";

  return WordExpression{};
}
