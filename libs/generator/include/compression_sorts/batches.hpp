#pragma once

#include <cstddef>
#include <vector>

namespace CompressionSorts {

struct BatchesSettings {
    size_t max_batch_size;
    double exponent;
};

std::vector<size_t> GenerateBatches(BatchesSettings /*settings*/);

}  // namespace CompressionSorts
