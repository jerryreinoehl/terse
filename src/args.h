#pragma once

#include <string>
#include <vector>

class Args {
  public:
    Args(int argc, char **argv);

    std::vector<std::string> command() const noexcept;
    bool verbose() const noexcept;

  private:
    std::vector<std::string> command_{};
    bool verbose_{false};
};
