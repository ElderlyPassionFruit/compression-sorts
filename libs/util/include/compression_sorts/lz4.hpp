#pragma once

#include <vector>

#include "time.hpp"

namespace CompressionSorts {

Time CalculateLz4CompressionTime(const std::vector<char>& /*data*/,
                                 std::vector<char>& /*compressed_data*/);

Time CalculateLz4DecompressionTime(const std::vector<char>& /*compressed_data*/,
                                   std::vector<char>& /*data*/);

}  // namespace CompressionSorts
