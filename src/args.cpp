#include "args.h"

Args::Args(int argc, char **argv) {
  std::string arg;
  bool end_of_opts = false;

  for (char **it = &argv[1], **end = &argv[argc]; it != end; ++it) {
    arg = std::string{*it};

    if (end_of_opts || arg[0] != '-') {
      command_.push_back(arg);
      continue;
    }

    if (arg == "-d" || arg == "--dry-run") {
      dry_run_ = true;
      verbose_ = true;
    } else if (arg == "-v" || arg == "--verbose") {
      verbose_ = true;
    } else if (arg == "--") {
      end_of_opts = true;
    }
  }
}

std::vector<std::string> Args::command() const noexcept {
  return command_;
}

bool Args::dry_run() const noexcept {
  return dry_run_;
}

bool Args::verbose() const noexcept {
  return verbose_;
}
