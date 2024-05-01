#pragma once

#include <memory>

#include "column_interface.hpp"

namespace CompressionSorts {

class Block {
public:
    using ColumnPtr = std::unique_ptr<IColumn>;
    using Container = std::vector<ColumnPtr>;

    explicit Block(Container /*data*/);

    size_t GetSize() const;
    size_t GetSerializedSize() const;
    size_t CalculateCompressionSize() const;

    void ApplyPermutation(const std::vector<size_t>& /*order*/);
    // for common Simulated Annealing: https://en.wikipedia.org/wiki/Simulated_annealing
    void SwapRaws(size_t /*i*/, size_t /*j*/);

    Container& GetData();

private:
    Container data_;
    const size_t size_;
};

}  // namespace CompressionSorts
