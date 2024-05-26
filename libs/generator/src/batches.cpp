#include "compression_sorts/batches.hpp"

#include <cmath>
#include <limits>

namespace CompressionSorts {

std::vector<size_t> GenerateBatches(BatchesSettings batches_settings) {
    const auto [max_batch_size, exponent] = batches_settings;
    std::vector<size_t> batches;
    for (size_t new_batch_size = 1; new_batch_size <= max_batch_size;
         new_batch_size = new_batch_size < max_batch_size
                              ? std::min<size_t>(max_batch_size, ceil(exponent * new_batch_size))
                              : std::numeric_limits<size_t>::max()) {
        batches.push_back(new_batch_size);
    }
    return batches;
}

}  // namespace CompressionSorts
