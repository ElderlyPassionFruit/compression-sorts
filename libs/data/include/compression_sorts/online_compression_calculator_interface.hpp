#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace CompressionSorts {

struct IOnlineCompressionCalculator {
    virtual size_t GetCurrentCompressedSize() = 0;

    virtual void ApplyPermutation(const std::vector<size_t>& /*order*/) = 0;

    // for common Simulated Annealing: https://en.wikipedia.org/wiki/Simulated_annealing
    virtual void Swap(size_t /*i*/, size_t /*j*/) = 0;

    virtual std::vector<size_t> GetCurrentOrder() = 0;

    virtual ~IOnlineCompressionCalculator() = default;
};

using OnlineCompressionCalculatorPtr = std::unique_ptr<IOnlineCompressionCalculator>;

}  // namespace CompressionSorts
