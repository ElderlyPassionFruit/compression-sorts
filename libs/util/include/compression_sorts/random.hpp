#pragma once

#include <random>

#include "compression_sorts/generator.hpp"

namespace CompressionSorts {

std::mt19937_64& GetTwister();

RawGenerator GetIntegersGenerator(int64_t /*min*/, int64_t /*max*/, size_t /*columns*/);

}  // namespace CompressionSorts
