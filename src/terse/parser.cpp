#include "parser.h"

#include <stack>

using terse::Parser;
using terse::Token;
using terse::TokenMap;

Parser::Parser(const char *buf, size_t size) : lexer_{buf, size} {}

std::optional<TokenMap> Parser::parse() {
  std::stack<TokenMap> maps;
  std::stack<std::string> emissions;
  TokenMap map;
  Token tok;
  std::string src;
  std::vector<std::string> tokens;
  Emission emission;

  do {
    tokens.clear();

    tok = next();
    if (tok.type() == Token::Type::STOP)
      break;

    if (tok.type() != Token::Type::STRING)
      return {};
    src = tok.value();

    tok = next();
    if (tok.type() != Token::Type::MAPS)
      return {};

    while ((tok = next()).type() == Token::Type::STRING)
      tokens.push_back(tok.value());

    emission = Emission{tokens};
    map[src] = emission;

    if (tok.type() == Token::Type::END)
      tok = next();

    switch (tok.type()) {
      case Token::Type::MAP_BEGIN:
        emissions.push(src);
        maps.push(map);
        map = TokenMap{};
        break;

      case Token::Type::MAP_END:
        while (tok.type() == Token::Type::MAP_END) {
          maps.top()[emissions.top()].set_map(map);
          map = maps.top();
          maps.pop();
          emissions.pop();
          tok = next();
        }
        put_back(tok);
        break;

      default:
        put_back(tok);
    }
  } while (tok.type() != Token::Type::STOP);

  return map;
}

Token Parser::next() {
  if (lookahead_) {
    lookahead_ = false;
    return lookahead_tok_;
  }

  return lexer_.next();
}

void Parser::put_back(const Token& token) {
  lookahead_tok_ = token;
  lookahead_ = true;
}
