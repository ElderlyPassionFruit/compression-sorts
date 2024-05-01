#pragma once

#include <typeinfo>
#include <unordered_set>

#include "column_interface.hpp"
#include "compression_sorts/lz4.hpp"
#include "compression_sorts/serialize_data.hpp"

namespace CompressionSorts {

template <std::integral T>
class ColumnIntegers : IColumn {
public:
    using Container = std::vector<T>;

    ColumnIntegers(Container data) : data_(std::move(data)), calculator_(data_) {
    }

    std::string GetTypeName() const override {
        return typeid(T).name();
    }

    size_t GetSize() const override {
        return data_.size();
    }

    size_t GetSerializedSize() const override {
        return SerializeData(data_).size();
    }

    size_t CalculateCompressionSize() const override {
        return calculator_.GetCurrentScore();
    }

    void ApplyPermutation(const std::vector<size_t>& order) override {
        ::CompressionSorts::ApplyPermutation(data_, order);
        calculator_.SetPermutation(order);
    }

    size_t CalculateDistinctValuesInRange(const Range& range) const override {
        // std::unordered_set<T> elements{std::advance(data_.begin(), range.from),
        //                                std::advance(data_.begin(), range.to)};
        std::unordered_set<T> elements{data_.begin() + range.from, data_.begin() + range.to};
        return elements.size();
    }

    void UpdatePermutation(std::vector<size_t>& order, const Range& range,
                           Algorithms algorithm) override {
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

    void SwapRaws(size_t i, size_t j) override {
        swap(data_[i], data_[j]);
    }

private:
    Container data_;
    Lz4OnlineCalculator<T> calculator_;
};

}  // namespace CompressionSorts
