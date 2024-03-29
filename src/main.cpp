#include "main.h"

#include "args.h"
#include "terse/lexer.h"
#include "terse/parser.h"
#include "terse/token.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <unistd.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  Args args{argc, argv};

  fs::path config{std::getenv("HOME")};
  config += fs::path{"/.config/terse/terse.conf"};

  std::ifstream in{config, std::ios::ate};
  std::streampos size = in.tellg();
  if (size == -1) {
    fprintf(stderr, "Error opening %s\n", config.c_str());
    exit(1);
  }

  std::unique_ptr<char[]> buf{new char[size]};
  in.seekg(0, std::ios::beg);
  in.read(buf.get(), size);
  in.close();

  terse::Parser parser{buf.get(), static_cast<size_t>(size)};
  terse::TokenMap map = parser.parse().value_or(terse::TokenMap{});

  std::vector<std::string> converted = translate(args.command(), map);

  if (args.verbose())
    std::cout << "\e[1;35m==> Executing: " << converted << "\e[0m\n";

  int rc;
  if (args.dry_run())
    rc = 0;
  else
    rc = execute(converted);

  if (rc != 0)
    perror("Error");

  return rc;
}

std::vector<std::string> translate(
  const std::vector<std::string>& args, const terse::TokenMap& map
) {
  std::vector<std::string> converted;
  terse::TokenMap tm = map;

  for (const auto& arg : args) {
    auto it = tm.find(arg);

    if (it != tm.end()) {
      for (const auto& token : it->second.tokens()) {
        converted.push_back(token);
      }
      tm = it->second.map().value_or(tm);
    } else {
      converted.push_back(arg);
    }
  }

  return converted;
}

int execute(const std::vector<std::string>& args) {
  if (args.size() == 0) {
    errno = EINVAL;
    return -EINVAL;
  }

  const char **argv = new const char*[args.size() + 1];

  for (size_t i = 0; i < args.size(); i++)
    argv[i] = args[i].c_str();
  argv[args.size()] = nullptr;

  int rc = execvp(argv[0], (char* const*)argv);

  delete [] argv;

  return rc;
}
