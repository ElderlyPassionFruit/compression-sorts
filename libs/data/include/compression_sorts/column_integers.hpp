#pragma once

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
        data_ = CompressionSorts::ApplyPermutation(data_, order);
    }

    size_t CalculateDistinctValuesInRange(const Range& range) const override {
        assert(range.from <= range.to);
        auto begin = std::ranges::next(data_.begin(), range.from);
        auto end = std::ranges::next(data_.begin(), range.to);
        std::unordered_set<T> elements{begin, end};
        return elements.size();
    }

    void UpdatePermutation(std::vector<size_t>& order, const Range& range,
                           Algorithms algorithm) const override {
        assert(range.from <= range.to);
        switch (algorithm) {
            case Algorithms::LexicographicSort:
            case Algorithms::SuffixArrayGreedy: {
                auto comparator = [&](size_t i, size_t j) { return data_[i] < data_[j]; };
                auto begin = std::ranges::next(order.begin(), range.from);
                auto end = std::ranges::next(order.begin(), range.to);
                std::sort(begin, end, comparator);
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

    const Container& GetData() const {
        return data_;
    }

    int Compare(size_t lhs, size_t rhs) const override {
        assert(lhs < data_.size());
        assert(rhs < data_.size());
        if (data_[lhs] < data_[rhs]) {
            return -1;
        } else if (data_[lhs] == data_[rhs]) {
            return 0;
        } else {
            return 1;
        }
    }

private:
    Container data_;
};

}  // namespace CompressionSorts
