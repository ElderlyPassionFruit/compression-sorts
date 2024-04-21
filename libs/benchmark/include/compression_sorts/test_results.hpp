#pragma once

#include <cstddef>
#include <string>

#include "compression_sorts/time.hpp"

namespace CompressionSorts {

using namespace std::chrono_literals;

struct CompressionScore {
    size_t serialized_size = 0;
    size_t compressed_size = 0;
    Time compression_time_ns = 0ns;
    Time decompression_time_ns = 0ns;
};

struct TestResults {
    Time find_permutation_time = 0ns;
    CompressionScore permuted_score;
};

struct BenchmarkResults {
    std::string name;
    std::string algorithm_name;
    size_t array_length = 0;

    CompressionScore initial_score{};
    std::vector<TestResults> permuted_scores{};
};

}  // namespace CompressionSorts
