#include "error.h"

using terse::ParseError;

ParseError::ParseError(int line, int col, std::string message)
  : line_{line}, col_{col}, message_{message} {}

unexpected<ParseError> terse::invalid_map_source_error(int line, int col) {
  return unexpected{ParseError{line, col, "Source must be a single word"}};
}
