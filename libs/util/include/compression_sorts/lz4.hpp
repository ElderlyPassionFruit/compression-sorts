#pragma once

#include <numeric>
#include <vector>

#include "compression_sorts/serialize_data.hpp"
#include "permutation.hpp"
#include "time.hpp"

namespace CompressionSorts {

std::vector<char> CompressLz4(const std::vector<char>& /*input*/);

std::vector<char> DecompressData(const std::vector<char>& /*compressed*/, int /*original_size*/);

Time CalculateLz4CompressionTime(const std::vector<char>& /*data*/,
                                 std::vector<char>& /*compressed_data*/);

Time CalculateLz4DecompressionTime(const std::vector<char>& /*compressed_data*/,
                                   std::vector<char>& /*data*/);

template <typename T>
class Lz4OnlineCalculator {
public:
    Lz4OnlineCalculator(const std::vector<T>& data) : data_(data), order_(data.size()) {
        std::iota(order_.begin(), order_.end(), 0);
    }

    void Swap(size_t i, size_t j) {
        if (i == j) {
            return;
        }
        std::swap(order_[i], order_[j]);
    }

    void SetPermutation(const std::vector<int>& order) {
        order_ = order;
    }

    size_t GetCurrentScore() const {
        const auto current_data = ApplyPermutation(data_, order_);
        const auto input = SerializeData(current_data);
        std::vector<char> compressed_data;
        /*Time time = */ CalculateLz4CompressionTime(input, compressed_data);
        return compressed_data.size();
    }

private:
    std::vector<T> data_;
    std::vector<int> order_;
};

}  // namespace CompressionSorts
