#include "environment.h"

using terse::Environment;
using terse::Translation;
using terse::TranslationMap;

Translation& Translation::operator=(const Translation& other) {
  if (this == &other) {
    return *this;
  }

  target_ = other.target_;
  map_ = other.map_;

  return *this;
}

Translation& Translation::operator=(Translation&& other) {
  if (this == &other) {
    return *this;
  }

  target_ = std::move(other.target_);
  map_ = std::move(other.map_);

  return *this;
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

inline const TranslationMap& Environment::map() const noexcept {
  return global_map_;
}

inline TranslationMap& Environment::map() noexcept {
  return global_map_;
}

inline Translation& Environment::add_translation(const std::string& source, const std::vector<std::string>& target) {
  map_->emplace(source, Translation{target});
  return map_->at(source);
}

inline void Environment::push_map(TranslationMap& map) {
  maps_.push(&map);
  map_ = maps_.top();
}

inline void Environment::pop_map() {
  maps_.pop();
}

std::vector<std::string> Environment::translate(const std::vector<std::string>& args) const noexcept {
  std::vector<std::string> translated{args.size()};

  for (auto& arg : args) {

  }

  return translated;
}
