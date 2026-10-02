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

const TranslationMap& Environment::map() const noexcept {
  return global_map_;
}

TranslationMap& Environment::map() noexcept {
  return global_map_;
}

Translation& Environment::add_translation(const std::string& source, const std::vector<std::string>& target) {
  map_->emplace(source, Translation{target});
  return map_->at(source);
}

void Environment::push_map(TranslationMap& map) {
  maps_.push(&map);
  map_ = maps_.top();
}

void Environment::pop_map() {
  maps_.pop();
}
