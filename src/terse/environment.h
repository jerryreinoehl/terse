#pragma once

#include <map>
#include <string>
#include <vector>

namespace terse {

  class Translation {
    public:
      Translation(const std::vector<std::string>& target) : target_{target} {}
      Translation(std::vector<std::string>&& target) : target_{std::move(target)} {}

      const std::map<std::string, Translation>& map() const noexcept;
      std::map<std::string, Translation>& map() noexcept;

    private:
      std::vector<std::string> target_;
      std::map<std::string, Translation> map_{};
  };

  class Environment {
    public:
      const std::map<std::string, Translation>& map() const noexcept;
      std::map<std::string, Translation>& map() noexcept;

    private:
      std::map<std::string, Translation> map_{};
  };

}
