#include "emission.h"

Emission::Emission() : tokens_{}, map_{} {}

Emission::Emission(const std::vector<std::string>& tokens) : tokens_{tokens}, map_{} {}

std::vector<std::string> Emission::tokens() const noexcept {
  return tokens_;
}

void Emission::set_tokens(const std::vector<std::string>& tokens) {
  tokens_ = tokens;
}

std::optional<TokenMap> Emission::map() const noexcept {
  return map_;
}

void Emission::set_map(const TokenMap& map) {
  map_ = map;
}
