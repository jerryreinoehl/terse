#pragma once

#include <optional>
#include <string>

class Token {
  public:
    enum Type { NONE, STRING, MAPS, MAP_BEGIN, MAP_END, END, STOP };

    Token(Type type = NONE, std::string value = {});

    Type type() const noexcept;
    std::string value() const noexcept;

  private:
    Type type_;
    std::optional<std::string> value_;
};
