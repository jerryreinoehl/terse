#pragma once

#include "emission.h"

#include <string>
#include <vector>

std::vector<std::string> translate(const std::vector<std::string>& args, const TokenMap& map);

int execute(const std::vector<std::string>& args);
