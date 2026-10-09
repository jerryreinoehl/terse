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
  std::cout << "command: " << args.command() << '\n';

  for (const auto& pair : map) {
    std::cout << pair.first << '\n';
  }

  //std::vector<std::string> converted = translate(args.command(), map);
  //std::cout << "converted: " << converted << '\n';

  //std::cout << terse::TokenType::from_lexeme("=>") << '\n';
  //std::cout << terse::TokenType::from_lexeme("(") << '\n';
  //std::cout << terse::TokenType::from_lexeme(")") << '\n';
  //std::cout << terse::TokenType::from_lexeme("{") << '\n';
  //std::cout << terse::TokenType::from_lexeme("}") << '\n';
  //std::cout << terse::TokenType::from_lexeme("$") << '\n';
  //std::cout << terse::TokenType::from_lexeme("\"") << '\n';
  //std::cout << terse::TokenType::from_lexeme("\n") << '\n';
  //std::cout << terse::TokenType::from_lexeme("cows") << '\n';

  //terse::WordToken token{"moocow"};
  //terse::Token t = terse::WordToken{"cat"};
  //std::cout << "value is " << token.value() << '\n';
  //std::cout << "type is " << token.type() << '\n';

  //f.close();

  //Args args{argc, argv};

  //fs::path config{std::getenv("HOME")};
  //config += fs::path{"/.config/terse/terse.conf"};

  //std::ifstream in{config, std::ios::ate};
  //std::streampos size = in.tellg();
  //if (size == -1) {
  //  fprintf(stderr, "Error opening %s\n", config.c_str());
  //  exit(1);
  //}

  //std::unique_ptr<char[]> buf{new char[size]};
  //in.seekg(0, std::ios::beg);
  //in.read(buf.get(), size);
  //in.close();

  //terse::Parser parser{buf.get(), static_cast<size_t>(size)};
  //terse::TokenMap map = parser.parse().value_or(terse::TokenMap{});

  //std::vector<std::string> converted = translate(args.command(), map);

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
