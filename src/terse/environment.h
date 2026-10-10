#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace terse {

  class Translation;

  using TranslationMap = std::map<std::string, Translation>;

  class Translation {
    public:
      explicit Translation(const std::vector<std::string>& target) : target_{target} {}
      explicit Translation(std::vector<std::string>&& target) : target_{std::move(target)} {}

      Translation(const Translation& other) : target_{other.target_}, map_{other.map_} {}
      Translation(Translation&& other) : target_{std::move(other.target_)}, map_{std::move(other.map_)} {}

      Translation& operator=(const Translation& other);
      Translation& operator=(Translation&& other);

      const TranslationMap& map() const noexcept;
      TranslationMap& map() noexcept;

    private:
      std::vector<std::string> target_;
      TranslationMap map_{};
  };

  class Environment {
    public:
      const TranslationMap& map() const noexcept;
      TranslationMap& map() noexcept;

      Translation& add_translation(const std::string& source, const std::vector<std::string>& target);
      void push_map(TranslationMap& map);
      void pop_map();

      std::vector<std::string> translate(const std::vector<std::string>& args) const noexcept;

    private:
      TranslationMap global_map_{};
      TranslationMap *map_{&global_map_};
      std::stack<TranslationMap*> maps_{};
  };

}
