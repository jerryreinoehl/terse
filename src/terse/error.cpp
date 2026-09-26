#include "error.h"

using terse::ParseError;

ParseError::ParseError(int line, int col, std::string message)
  : line_{line}, col_{col}, message_{message} {}
