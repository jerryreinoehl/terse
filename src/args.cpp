#include "args.h"

Args::Args(int argc, char **argv) {
  std::string arg;
  for (char **it = &argv[1], **end = &argv[argc]; it != end; ++it) {
    arg = std::string{*it};

    if (arg[0] != '-') {
      command_.push_back(arg);
      continue;
    }

    if (arg == "-v")
      verbose_ = true;
  }
}

std::vector<std::string> Args::command() const noexcept {
  return command_;
}

bool Args::verbose() const noexcept {
  return verbose_;
}
