#pragma once

#include <vector>

#include "time.hpp"

namespace CompressionSorts {

std::vector<char> CompressLz4(const std::vector<char>& /*input*/);

std::vector<char> DecompressLz4(const std::vector<char>& /*compressed*/, int /*original_size*/);

Time CalculateLz4CompressionTime(const std::vector<char>& /*data*/,
                                 std::vector<char>& /*compressed_data*/);

Time CalculateLz4DecompressionTime(const std::vector<char>& /*compressed_data*/,
                                   std::vector<char>& /*data*/);

}  // namespace CompressionSorts
