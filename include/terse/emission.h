#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

class Emission;

using TokenMap = std::map<std::string, Emission>;

class Emission {
  public:
    Emission();
    Emission(const std::vector<std::string>& tokens);

    std::vector<std::string> tokens() const noexcept;
    void set_tokens(const std::vector<std::string>& tokens);

    std::optional<TokenMap> map() const noexcept;
    void set_map(const TokenMap& map);

  private:
    std::vector<std::string> tokens_{};
    std::optional<TokenMap> map_{};
};
