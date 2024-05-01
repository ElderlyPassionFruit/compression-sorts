#pragma once

#include "column_interface.hpp"
#include "compression_sorts/lz4.hpp"

namespace CompressionSorts {

class ColumnStrings : IColumn {
public:
    using Container = std::vector<std::string>;

    ColumnStrings(Container data);
    std::string GetTypeName() const override;
    size_t GetSize() const override;
    size_t GetSerializedSize() const override;
    size_t CalculateCompressionSize() const override;
    void ApplyPermutation(const std::vector<size_t>& order) override;
    size_t CalculateDistinctValuesInRange(const Range& range) const override;
    void UpdatePermutation(std::vector<size_t>& order, const Range& range,
                           Algorithms algorithm) override;
    void SwapRaws(size_t i, size_t j) override;

private:
    Container data_;
    Lz4OnlineCalculator<std::string> calculator_;
};

}  // namespace CompressionSorts
