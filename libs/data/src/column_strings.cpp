#include "compression_sorts/column_strings.hpp"

#include <unordered_set>

namespace CompressionSorts {

ColumnStrings::ColumnStrings(ColumnStrings::Container data)
    : data_(std::move(data)), calculator_(data_) {
}

std::string ColumnStrings::GetTypeName() const {
    return "std::string";
}

size_t ColumnStrings::GetSize() const {
    return data_.size();
}

size_t ColumnStrings::GetSerializedSize() const {
    return SerializeData(data_).size();
}

size_t ColumnStrings::CalculateCompressionSize() const {
    return calculator_.GetCurrentScore();
}

void ColumnStrings::ApplyPermutation(const std::vector<size_t>& order) {
    ::CompressionSorts::ApplyPermutation(data_, order);
    calculator_.SetPermutation(order);
}

size_t ColumnStrings::CalculateDistinctValuesInRange(const Range& range) const {
    // std::unordered_set<T> elements{std::advance(data_.begin(), range.from),
    //                                std::advance(data_.begin(), range.to)};
    std::unordered_set<std::string> elements{data_.begin() + range.from, data_.begin() + range.to};
    return elements.size();
}

void ColumnStrings::UpdatePermutation(std::vector<size_t>& order, const Range& range,
                                      Algorithms algorithm) {
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

void ColumnStrings::SwapRaws(size_t i, size_t j) {
    swap(data_[i], data_[j]);
}

}  // namespace CompressionSorts
