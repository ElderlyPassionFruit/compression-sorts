#pragma once

#include <functional>
#include <string>
#include <vector>

#include "compression_sorts/online_compression_calculator_interface.hpp"
#include "compression_sorts/range.hpp"
#include "compression_sorts/read_data.hpp"

namespace CompressionSorts {

enum class Algorithms {
    LexicographicSort,
    SuffixArrayGreedy,
};

struct IColumn {
    virtual ~IColumn() = default;

    virtual std::string GetTypeName() const = 0;

    virtual size_t GetSize() const = 0;
    virtual size_t GetSerializedSize() const = 0;
    virtual size_t CalculateCompressionSize() const = 0;
    virtual void ApplyPermutation(const std::vector<size_t>& /*order*/) = 0;
    virtual size_t CalculateDistinctValuesInRange(const Range& /*range*/) const = 0;
    virtual void UpdatePermutation(std::vector<size_t>& /*order*/, const Range& /*range*/,
                                   Algorithms /*algorithm*/) const = 0;
    virtual EqualRanges GetEqualRanges(const std::vector<size_t>& /*order*/,
                                       const Range& /*range*/) const = 0;

    virtual OnlineCompressionCalculatorPtr GetOnlineCompressionCalculator() const = 0;
    virtual std::vector<char> GetSerializedData() const = 0;

    // -1 - <, 0 - =, 1 - >
    virtual int Compare(size_t /*lhs*/, size_t /*rhs*/) const = 0;
};

using ColumnPtr = std::unique_ptr<IColumn>;

template <typename TColumn>
ColumnPtr GenericColumnParser(const std::vector<std::string>& rows) {
    using Container = TColumn::Container;
    using ValueType = TColumn::ValueType;
    Container data(rows.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        data[i] = FromString<ValueType>(rows[i]);
    }
    return std::make_unique<TColumn>(std::move(data));
}

using ColumnParser = std::function<ColumnPtr(const std::vector<std::string>& /*rows*/)>;

}  // namespace CompressionSorts
