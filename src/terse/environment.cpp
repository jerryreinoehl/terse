#include "environment.h"

using terse::Environment;
using terse::Translation;

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
