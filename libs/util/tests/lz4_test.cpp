#include "compression_sorts/lz4.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <random>

#include "compression_sorts/random.hpp"
#include "compression_sorts/serialize_data.hpp"

TEST(LZ4, UnitCompressionDecompression) {
    std::vector<int64_t> a = {1, 2, 3, 4, 5, 179, -10, 256, 1LL << 63};
    const auto serialized_data = CompressionSorts::SerializeData(a);
    EXPECT_EQ(serialized_data.size(), 72);
    const auto compressed_data = CompressionSorts::CompressLz4(serialized_data);
    EXPECT_EQ(compressed_data.size(), 43);
    const auto decompressed_data =
        CompressionSorts::DecompressLz4(compressed_data, serialized_data.size());
    EXPECT_EQ(serialized_data, decompressed_data);
}

TEST(LZ4, StressCompressionDecompression) {
    constexpr size_t kIterations = 1000;
    constexpr size_t kMaxArraySize = 1000;
    std::uniform_int_distribution<size_t> size_distribution(1, kMaxArraySize);
    std::uniform_int_distribution<int64_t> element_distribution(
        std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());

    for (size_t i = 0; i < kIterations; ++i) {
        const size_t n = size_distribution(CompressionSorts::GetTwister());
        std::vector<int64_t> a(n);
        for (auto& value : a) {
            value = element_distribution(CompressionSorts::GetTwister());
        }
        const auto serialize_data = CompressionSorts::SerializeData(a);
        const auto compressed_data = CompressionSorts::CompressLz4(serialize_data);
        const auto decompressed_data =
            CompressionSorts::DecompressLz4(compressed_data, serialize_data.size());
        EXPECT_EQ(serialize_data, decompressed_data);
    }
}
