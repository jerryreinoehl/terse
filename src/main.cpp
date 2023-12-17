#include "lexer.h"
#include "main.h"
#include "parser.h"
#include "token.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <unistd.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  fs::path config{std::getenv("HOME")};
  config += fs::path{"/.config/tiny/config"};

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

  Parser parser{buf.get(), static_cast<size_t>(size)};
  TokenMap map = parser.parse().value_or(TokenMap{});

  std::vector<std::string> args;
  for (int i = 1; i < argc; i++)
    args.push_back(argv[i]);
  std::vector<std::string> converted = translate(args, map);

  execute(converted);

  return 0;
}

std::vector<std::string> translate(const std::vector<std::string>& args, const TokenMap& map) {
  std::vector<std::string> converted;
  TokenMap tm = map;

  for (const auto& arg : args) {
    auto it = tm.find(arg);

    if (it != tm.end()) {
      for (const auto& token : it->second.tokens()) {
        converted.push_back(token);
        tm = it->second.map().value_or(tm);
      }
    } else {
      converted.push_back(arg);
    }
  }

  return converted;
}

int execute(const std::vector<std::string>& args) {
  const char **argv = new const char*[args.size() + 1];

  for (size_t i = 0; i < args.size(); i++)
    argv[i] = args[i].c_str();
  argv[args.size()] = nullptr;

  int rc = execvp(argv[0], (char* const*)argv);

  delete [] argv;

  return rc;
}
