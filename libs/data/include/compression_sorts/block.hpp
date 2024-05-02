#pragma once

#include "column_interface.hpp"
#include "compression_sorts/online_compression_calculator_interface.hpp"

namespace CompressionSorts {

class Block {
public:
    using Container = std::vector<ColumnPtr>;

    explicit Block(Container /*data*/);

    Block(Block&&) = default;
    Block& operator=(Block&&) = delete;

    Block(Block&) = delete;
    Block& operator=(Block&) = delete;

    size_t GetSize() const;
    size_t GetSerializedSize() const;
    size_t CalculateCompressionSize() const;

    void ApplyPermutation(const std::vector<size_t>& /*order*/);

    const Container& GetData() const;

    OnlineCompressionCalculatorPtr GetOnlineCompressionCalculator() const;

private:
    Container data_;
    const size_t size_;
};

}  // namespace CompressionSorts
