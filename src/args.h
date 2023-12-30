#pragma once

#include <string>
#include <vector>

class Args {
  public:
    Args(int argc, char **argv);

    std::vector<std::string> command() const noexcept;
    bool dry_run() const noexcept;
    bool verbose() const noexcept;

  private:
    std::vector<std::string> command_{};
    bool dry_run_ = false;
    bool verbose_ = false;
};
