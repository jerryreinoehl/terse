#include "main.h"

#include "args.h"
#include "terse/lexer.h"
#include "terse/parser.h"
#include "os.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <unistd.h>

#include <sstream>

int main(int argc, char **argv) {
  // Try to get terse config in this order:
  //   1. $TERSE_CONFIG
  //   2. $XDG_CONFIG_HOME/terse/terse.conf
  //   3. $HOME/.config/terse/terse.conf
  std::string config = os::env::expand("${TERSE_CONFIG:-${XDG_CONFIG_HOME:-$HOME/.config}/terse/terse.conf}");

  std::ifstream f;
  f.open(config, std::ifstream::in);
  //f.open("/etc/hosts", std::ifstream::in);

  //terse::Lexer lexer{f};

  //std::unique_ptr<const terse::Token> tok;
  //while ((tok = lexer.next())->type() != terse::TokenType::STOP) {
  //  if (tok->type() == terse::TokenType::ERROR) {
  //    std::cout
  //      << static_cast<const terse::ErrorToken*>(tok.get())->error()
  //      << ": line " << tok->line() << ":" << tok->col() << "\n\n";
  //  } else if (tok->type() == terse::TokenType::STRING) {
  //    std::cout
  //      << '"' << static_cast<const terse::StringToken*>(tok.get())->value() << '"'
  //      << ": line " << tok->line() << ":" << tok->col() << "\n\n";
  //  } else {
  //    std::cout << tok->type() << " " << tok->line() << " " << tok->col() << "\n\n";
  //  }
  //}

  Args args{argc, argv};

  terse::Parser parser{f};
  terse::TokenMap map = parser.parse().value_or(terse::TokenMap{});
  terse::Environment environment = parser.environment();

  f.close();

  //if (args.verbose())
  //  std::cout << "\e[1;35m==> Executing: " << converted << "\e[0m\n";

  //int rc;
  //if (args.dry_run())
  //  rc = 0;
  //else
  //  rc = execute(converted);

  //if (rc != 0)
  //  perror("Error");

  //return rc;
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

//int execute(const std::vector<std::string>& args) {
//  if (args.size() == 0) {
//    errno = EINVAL;
//    return -EINVAL;
//  }
//
//  const char **argv = new const char*[args.size() + 1];
//
//  for (size_t i = 0; i < args.size(); i++)
//    argv[i] = args[i].c_str();
//  argv[args.size()] = nullptr;
//
//  int rc = execvp(argv[0], (char* const*)argv);
//
//  delete [] argv;
//
//  return rc;
//}
