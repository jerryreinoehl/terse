#pragma once

#include <map>
#include <string>
#include <vector>

namespace terse {

  class Translation {
    public:
      Translation(const std::vector<std::string>& target) : target_{target} {}
      Translation(std::vector<std::string>&& target) : target_{std::move(target)} {}

      Translation(const Translation& other) : target_{other.target_}, map_{other.map_} {}
      Translation(Translation&& other) : target_{std::move(other.target_)}, map_{std::move(other.map_)} {}

      Translation& operator=(const Translation& other);
      Translation& operator=(Translation&& other);

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
