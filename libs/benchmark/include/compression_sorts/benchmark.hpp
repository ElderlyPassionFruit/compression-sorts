#pragma once

#include "compression_sorts/permute_interface.hpp"
#include "compression_sorts/time.hpp"
#include "test_results.hpp"

namespace CompressionSorts {

CompressionScore CalculateCompressionScore(const IColumn& column);

CompressionScore CalculateCompressionScore(const Block& block);

Time CalculateFindPermutationTime(const Block& block, const IPermute& algorithm,
                                  std::vector<size_t>& order);

BenchmarkResults TestAlgorithm(const std::string& test_name, Block block, const IPermute& algorithm,
                               const size_t iterations);

}  // namespace CompressionSorts
