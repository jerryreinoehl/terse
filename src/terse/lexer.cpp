#include "lexer.h"

#include <cctype>
#include <string_view>

using terse::Lexer;
using terse::Token;

Lexer::Lexer(const char *buf, size_t size)
  : buf_{buf}, cur_{buf}, end_{buf_ + size}
{}

Token Lexer::next() {
  const char *toks;
  std::string_view value;
  std::string_view::size_type len;

  if (at_end_of_statement_) {
    at_end_of_statement_ = false;
    return {Token::Type::END};
  }

  seek();

  if (cur_ >= end_) {
    return {Token::Type::STOP};
  }

  toks = cur_;
  while (cur_ <= end_ && !isspace(*cur_))
    ++cur_;

  if (cur_ >= end_)
    cur_ = end_;

  len = cur_ - toks;
  value = std::string_view{toks, len};

  if (value == "=>") {
    return {Token::Type::MAPS};
  } else if (value == "{") {
    return {Token::Type::MAP_BEGIN};
  } else if (value == "}") {
    return {Token::Type::MAP_END};
  } else {
    skip_space();
    if (*cur_ == '\n' || *cur_ == COMMENT)
      at_end_of_statement_ = true;

    return {Token::Type::STRING, value};
  }
}

void Lexer::skip_whitespace() {
  while (cur_ <= end_ && isspace(*cur_))
    ++cur_;
}

void Lexer::skip_space() {
  while (cur_ <= end_ && *cur_ == ' ')
    ++cur_;
}

void Lexer::skip_line() {
  while (cur_ <= end_ && *cur_ != '\n')
    ++cur_;
}

void Lexer::seek() {
  while (true) {
    skip_whitespace();
    if (cur_ <= end_ && *cur_ == COMMENT)
      skip_line();
    else
      break;
  }
}
