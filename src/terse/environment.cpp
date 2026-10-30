#include "environment.hpp"

using terse::Environment;
using terse::Translation;
using terse::TranslationMap;

//*****************************************************************************
// struct Translation
//*****************************************************************************

Translation& Translation::operator=(Translation&& other) {
  if (this == &other) {
    return *this;
  }

  target_ = std::move(other.target_);
  map_ = std::move(other.map_);

  return *this;
}

const std::vector<terse::TokenPtr>& Translation::target() const noexcept {
  return target_;
}

const TranslationMap& Translation::map() const noexcept {
  return map_;
}

TranslationMap& Translation::map() noexcept {
  return map_;
}

//*****************************************************************************
// struct Environment
//*****************************************************************************

const TranslationMap& Environment::map() const noexcept {
  return global_map_;
}

TranslationMap& Environment::map() noexcept {
  return global_map_;
}

Translation& Environment::add_translation(TokenPtr source, std::vector<TokenPtr>&& target) {
  map_->emplace(source->as<StringToken>().value(), Translation{std::move(target)});
  return map_->at(source->as<StringToken>().value().to_string());
}

void Environment::push_map(TranslationMap& map) {
  maps_.push(map_);
  map_ = &map;
}

void Environment::pop_map() {
  map_ = maps_.top();
  maps_.pop();
}

void Environment::add_symbol(utl::string_view symbol, utl::string_view value) {
  symbol_map_.emplace(symbol, value);
}

std::vector<utl::string_view> Environment::translate(const std::vector<std::string>& args) const noexcept {
  std::vector<utl::string_view> translated{};
  translated.reserve(8);

  const TranslationMap *tm = &global_map_;

  for (auto& arg : args) {
    auto it = tm->find(arg);

    if (it != tm->end()) {
      for (auto& i : it->second.target()) {
        translated.push_back(i->as<StringToken>().value());
      }

      if (it->second.map().size() > 0) {
        tm = &it->second.map();
      }
    } else {
      translated.push_back(arg);
    }
  }

  return translated;
}
