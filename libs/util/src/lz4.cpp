#include "compression_sorts/lz4.hpp"

#include <lz4.h>

#include <stdexcept>

#include "compression_sorts/time.hpp"

namespace CompressionSorts {

std::vector<char> CompressLz4(const std::vector<char>& input) {
    int max_compressed_size = LZ4_compressBound(input.size());
    std::vector<char> compressed(max_compressed_size);

    int compressedSize =
        LZ4_compress_default(input.data(), compressed.data(), input.size(), max_compressed_size);

    if (compressedSize > 0) {
        compressed.resize(compressedSize);
    } else {
        throw std::runtime_error("Compression failed");
    }

    return compressed;
}

std::vector<char> DecompressLz4(const std::vector<char>& compressed, int original_size) {
    std::vector<char> decompressed(original_size);

    int decompressedSize = LZ4_decompress_safe(compressed.data(), decompressed.data(),
                                               compressed.size(), original_size);

    if (decompressedSize < 0) {
        throw std::runtime_error("Decompression failed");
    }

    decompressed.resize(decompressedSize);
    return decompressed;
}

Time CalculateLz4CompressionTime(const std::vector<char>& data,
                                 std::vector<char>& compressed_data) {
    auto helper = [&data, &compressed_data]() mutable { compressed_data = CompressLz4(data); };
    return MesuareTime(helper);
}

Time CalculateLz4DecompressionTime(const std::vector<char>& compressed_data,
                                   std::vector<char>& data) {
    auto helper = [&compressed_data, original_size = data.size(), &data]() {
        data = DecompressLz4(compressed_data, original_size);
    };
    return MesuareTime(helper);
}

}  // namespace CompressionSorts
