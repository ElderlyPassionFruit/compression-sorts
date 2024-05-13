#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace CompressionSorts {

std::vector<size_t> BuildSuffixArray(const std::string& /*s*/);

std::vector<int> BuildLCPArray(const std::string& /*s*/, const std::vector<size_t>& /*sa*/);

std::vector<size_t> GetSuffixArrayGreedyOrder(const std::vector<std::string>& /*data*/);

}  // namespace CompressionSorts
