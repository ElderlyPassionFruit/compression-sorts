#pragma once

#include <string>
#include <vector>

#include "compression_sorts/range.hpp"

namespace CompressionSorts {

enum class Algorithms {
    LexicographicSort,
};

class IColumn {
public:
    virtual std::string GetTypeName() const = 0;

    virtual size_t GetSize() const = 0;
    virtual size_t GetSerializedSize() const = 0;
    virtual size_t CalculateCompressionSize() const = 0;
    virtual void ApplyPermutation(const std::vector<size_t>& /*order*/) = 0;
    virtual size_t CalculateDistinctValuesInRange(const Range& /*range*/) const = 0;
    virtual void UpdatePermutation(std::vector<size_t>& /*order*/, const Range& /*range*/,
                                   Algorithms /*algorithm*/) = 0;
    virtual EqualRanges GetEqualRanges(const std::vector<size_t>& /*order*/,
                                       const Range& /*range*/) = 0;

    // for common Simulated Annealing: https://en.wikipedia.org/wiki/Simulated_annealing
    virtual void SwapRaws(size_t /*i*/, size_t /*j*/) = 0;

    virtual ~IColumn() = default;
};

}  // namespace CompressionSorts
