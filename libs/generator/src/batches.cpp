#include "compression_sorts/batches.hpp"

#include <cmath>

namespace CompressionSorts {

std::vector<size_t> GenerateBatches(BatchesSettings batches_settings) {
    std::vector<size_t> batches;
    for (size_t new_batch_size = 1; new_batch_size <= batches_settings.max_batch_size;
         new_batch_size =
             new_batch_size < batches_settings.max_batch_size
                 ? std::min<size_t>(batches_settings.max_batch_size,
                                    ceil(batches_settings.exponent * new_batch_size))
                 : static_cast<size_t>(ceil(batches_settings.exponent * new_batch_size))) {
        batches.push_back(new_batch_size);
    }
    return batches;
}

}  // namespace CompressionSorts
