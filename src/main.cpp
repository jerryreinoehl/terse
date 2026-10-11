#include "main.h"

#include "args.h"
#include "terse/parser.h"
#include "os.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>

int main(int argc, char **argv) {
  // Try to get terse config in this order:
  //   1. $TERSE_CONFIG
  //   2. $XDG_CONFIG_HOME/terse/terse.conf
  //   3. $HOME/.config/terse/terse.conf
  std::string config = os::env::expand("${TERSE_CONFIG:-${XDG_CONFIG_HOME:-$HOME/.config}/terse/terse.conf}");

  std::ifstream f;
  f.open(config, std::ifstream::in);

  Args args{argc, argv};

  terse::Parser parser{f};
  terse::ParseResult<const terse::Environment*> result = parser.parse();
  if (!result) {
    std::cerr << std::move(result).error().to_string() << '\n';
    return 1;
  }
  const terse::Environment& environment = **result;

  for (const auto& pair : environment.map()) {
    std::cout << "key: " << pair.first << '\n';
  }

  std::vector<std::string> translated = environment.translate(args.command());

  std::cout << "command: ";
  for (const auto& word : args.command()) {
    std::cout << word << ' ';
  }
  std::cout << '\n';

  std::cout << "translated: ";
  for (auto& word : translated) {
    std::cout << "'" << word << "'" << ' ';
  }
  std::cout << '\n';

  f.close();

  if (args.verbose()) {
    std::cout << "\e[1;35m==> Executing: " << translated << "\e[0m\n";
  }

  int rc;
  if (args.dry_run()) {
    rc = 0;
  } else {
    rc = execute(translated);
  }

  if (rc != 0) {
    perror("Error");
  }

  return rc;
}

int execute(const std::vector<std::string>& args) {
  if (args.size() == 0) {
    errno = EINVAL;
    return -EINVAL;
  }

  const char **argv = new const char*[args.size() + 1];

  for (size_t i = 0; i < args.size(); i++) {
    argv[i] = args[i].c_str();
  }
  argv[args.size()] = nullptr;

  int rc = execvp(argv[0], (char* const*)argv);

  delete [] argv;

  return rc;
}
