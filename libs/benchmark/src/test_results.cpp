#include "compression_sorts/test_results.hpp"

namespace CompressionSorts {

CompressionScore& CompressionScore::operator+=(const CompressionScore& other) {
    serialized_size += other.serialized_size;
    compressed_size += other.compressed_size;
    compression_time_ns += other.compression_time_ns;
    decompression_time_ns += other.decompression_time_ns;
    return *this;
}

}  // namespace CompressionSorts
