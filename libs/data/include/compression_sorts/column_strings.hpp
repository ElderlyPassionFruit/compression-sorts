#pragma once

#include "column_interface.hpp"

namespace CompressionSorts {

class ColumnStrings final : public IColumn {
public:
    using Container = std::vector<std::string>;

    ColumnStrings(Container data);
    std::string GetTypeName() const override;
    size_t GetSize() const override;
    size_t GetSerializedSize() const override;
    size_t CalculateCompressionSize() const override;
    void ApplyPermutation(const std::vector<size_t>& /*order*/) override;
    size_t CalculateDistinctValuesInRange(const Range& /*range*/) const override;
    void UpdatePermutation(std::vector<size_t>& /*order*/, const Range& /*range*/,
                           Algorithms /*algorithm*/) const override;
    OnlineCompressionCalculatorPtr GetOnlineCompressionCalculator() const override;
    EqualRanges GetEqualRanges(const std::vector<size_t>& /*order*/,
                               const Range& /*range*/) const override;
    std::vector<char> GetSerializedData() const override;

private:
    Container data_;
};

}  // namespace CompressionSorts
