#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <utl/string_view.hpp>

#include "token.hpp"

namespace terse {

  class Translation;

  using TranslationMap = std::map<std::string, Translation>;

  class Translation {
    public:
      explicit Translation(std::vector<TokenPtr>&& target) : target_{std::move(target)} {}

      Translation(const Translation& other) = delete;
      Translation(Translation&& other) : target_{std::move(other.target_)}, map_{std::move(other.map_)} {}

      Translation& operator=(const Translation& other) = delete;
      Translation& operator=(Translation&& other);

      const std::vector<TokenPtr>& target() const noexcept;

      const TranslationMap& map() const noexcept;
      TranslationMap& map() noexcept;

    private:
      std::vector<TokenPtr> target_;
      TranslationMap map_{};
  };

  class Environment {
    public:
      const TranslationMap& map() const noexcept;
      TranslationMap& map() noexcept;

      Translation& add_translation(TokenPtr source, std::vector<TokenPtr>&& target);
      void push_map(TranslationMap& map);
      void pop_map();

      std::vector<std::string> translate(const std::vector<std::string>& args) const noexcept;

    private:
      TranslationMap global_map_{};
      TranslationMap *map_{&global_map_};
      std::stack<TranslationMap*> maps_{};
  };

}
