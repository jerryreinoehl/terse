#pragma once

#include <map>
#include <string>
#include <vector>

namespace terse {

  class Translation {
    public:

    private:
      std::vector<std::string> target_;
      std::map<std::string, Translation> map_{};
  };

  class Environment {
    public:
    private:
      std::map<std::string, Translation> map_{};
  };

}
