#include "environment.h"

using terse::Environment;
using terse::Translation;
using terse::TranslationMap;

//*****************************************************************************
// struct Translation
//*****************************************************************************

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

const std::vector<std::string>& Translation::target() const noexcept {
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

std::vector<std::string> Environment::translate(const std::vector<std::string>& args) const noexcept {
  std::vector<std::string> translated{};
  translated.reserve(8);

  const TranslationMap *tm = &global_map_;

  for (auto& arg : args) {
    auto it = tm->find(arg);

    if (it != tm->end()) {
      for (auto& i : it->second.target()) {
        translated.push_back(i);
        if (it->second.map().size() > 0) {
          tm = &it->second.map();
        }
      }
    } else {
      translated.push_back(arg);
    }
  }

  return translated;
}
