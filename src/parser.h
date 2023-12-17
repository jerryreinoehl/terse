#pragma once

#include "emission.h"
#include "lexer.h"
#include "token.h"

#include <optional>

class Parser {
  public:
    Parser(const char *buf, size_t size);
    std::optional<TokenMap> parse();

  private:
    Lexer lexer_;
    Token lookahead_tok_;
    bool lookahead_;

    Token next();
    void put_back(const Token& token);
};
