#include "compression_sorts/lz4.hpp"

#include <lz4.h>

#include <stdexcept>

#include "compression_sorts/time.hpp"

namespace CompressionSorts {

Time CalculateLz4CompressionTime(const std::vector<char>& data,
                                 std::vector<char>& compressed_data) {
    compressed_data.resize(LZ4_compressBound(data.size()));

    auto helper = [&data, &compressed_data]() {
        int compressed_size = LZ4_compress_default(data.data(), compressed_data.data(), data.size(),
                                                   compressed_data.size());
        if (compressed_size <= 0) {
            throw std::runtime_error("Compression failed");
        }
    };

    return MesuareTime(helper);
}

Time CalculateLz4DecompressionTime(const std::vector<char>& compressed_data,
                                   std::vector<char>& data) {
    return MesuareTime(std::bind(LZ4_decompress_safe, compressed_data.data(), data.data(),
                                 compressed_data.size(), data.size()));
}

}  // namespace CompressionSorts
