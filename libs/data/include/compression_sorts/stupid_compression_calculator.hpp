#pragma once

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/online_compression_calculator_interface.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/serialize_data.hpp"

namespace CompressionSorts {

template <typename T>
class StupidOnlineCompressionCalculator : public IOnlineCompressionCalculator {
public:
    explicit StupidOnlineCompressionCalculator(const std::vector<T>& data)
        : data_(data), order_(GetIdentityPermutation(data_.size())) {
    }

    size_t GetCurrentCompressedSize() {
        const auto permuted_data = ::CompressionSorts::ApplyPermutation(data_, order_);
        const auto serialized_data = SerializeData(permuted_data);
        const auto compressed_data = CompressLz4(serialized_data);
        return compressed_data.size();
    }

    void ApplyPermutation(const std::vector<size_t>& order) {
        order_ = order;
    }

    void Swap(size_t i, size_t j) {
        std::swap(order_[i], order_[j]);
    }

    std::vector<size_t> GetCurrentOrder() {
        return order_;
    }

private:
    const std::vector<T> data_;
    std::vector<size_t> order_;
};

}  // namespace CompressionSorts
