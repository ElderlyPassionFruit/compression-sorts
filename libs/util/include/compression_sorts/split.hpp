#pragma once

#include <string>
#include <vector>

namespace CompressionSorts {

std::vector<std::string> SplitBySymbol(std::string s, char delimiter);

std::vector<std::vector<std::string>> SplitAllStrings(std::vector<std::string> data,
                                                      char delimiter);

}  // namespace CompressionSorts
