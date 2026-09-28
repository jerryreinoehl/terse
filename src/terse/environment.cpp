#include "environment.h"

using terse::Environment;
using terse::Translation;

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

const std::map<std::string, Translation>& Translation::map() const noexcept {
  return map_;
}

std::map<std::string, Translation>& Translation::map() noexcept {
  return map_;
}

const std::map<std::string, Translation>& Environment::map() const noexcept {
  return map_;
}

std::map<std::string, Translation>& Environment::map() noexcept {
  return map_;
}
