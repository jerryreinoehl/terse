#include "main.hpp"

#include "args.hpp"
#include "terse/parser.hpp"
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
  terse::ParseResult<terse::Environment*> result = parser.parse();
  if (!result) {
    std::cerr << std::move(result).error().to_string() << '\n';
    return 1;
  }
  terse::Environment& environment = **result;

  std::vector<utl::string_view> translated = environment.translate(args.command());

  f.close();

  if (args.verbose()) {
    std::cout << "\e[90m" << translated << "\e[0m\n";
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

int execute(const std::vector<utl::string_view>& args) {
  if (args.size() == 0) {
    errno = EINVAL;
    return -EINVAL;
  }

  const char **argv = new const char*[args.size() + 1];

  for (size_t i = 0; i < args.size(); i++) {
    argv[i] = args[i].data();
  }
  argv[args.size()] = nullptr;

  int rc = execvp(argv[0], (char* const*)argv);

  delete [] argv;

  return rc;
}
