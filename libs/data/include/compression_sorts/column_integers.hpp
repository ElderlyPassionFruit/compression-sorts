#pragma once

#include <type_traits>
#include <unordered_set>

#include "column_interface.hpp"
#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"
#include "compression_sorts/serialize_data.hpp"
#include "compression_sorts/stupid_compression_calculator.hpp"

namespace CompressionSorts {

template <std::integral T>
class ColumnIntegers : public IColumn {
public:
    using Container = std::vector<T>;

    explicit ColumnIntegers(Container data) : data_(std::move(data)) {
    }

    std::string GetTypeName() const override {
        return "integer";
    }

    size_t GetSize() const override {
        return data_.size();
    }

    size_t GetSerializedSize() const override {
        return SerializeData(data_).size();
    }

    size_t CalculateCompressionSize() const override {
        return CompressLz4(SerializeData(data_)).size();
    }

    void ApplyPermutation(const std::vector<size_t>& order) override {
        CompressionSorts::ApplyPermutation(data_, order);
    }

    size_t CalculateDistinctValuesInRange(const Range& range) const override {
        // std::unordered_set<T> elements{std::advance(data_.begin(), range.from),
        //                                std::advance(data_.begin(), range.to)};
        std::unordered_set<T> elements{data_.begin() + range.from, data_.begin() + range.to};
        return elements.size();
    }

    void UpdatePermutation(std::vector<size_t>& order, const Range& range,
                           Algorithms algorithm) const override {
        // std::sort(std::advance(order.begin(), range.from), std::advance(order.begin(),
        // range.to));
        switch (algorithm) {
            case Algorithms::LexicographicSort: {
                auto comparator = [&](size_t i, size_t j) { return data_[i] < data_[j]; };
                std::sort(order.begin() + range.from, order.begin() + range.to, comparator);
                break;
            }
        }
    }

    EqualRanges GetEqualRanges(const std::vector<size_t>& order,
                               const Range& range) const override {
        return ::CompressionSorts::GetEqualRanges(data_, order, range);
    }

    OnlineCompressionCalculatorPtr GetOnlineCompressionCalculator() const override {
        return std::make_unique<StupidOnlineCompressionCalculator<T>>(data_);
    }

    std::vector<char> GetSerializedData() const override {
        return SerializeData(data_);
    }

private:
    Container data_;
};

}  // namespace CompressionSorts
