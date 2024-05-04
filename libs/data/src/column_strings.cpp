#include "compression_sorts/column_strings.hpp"

#include <unordered_set>

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"
#include "compression_sorts/serialize_data.hpp"
#include "compression_sorts/stupid_compression_calculator.hpp"

namespace CompressionSorts {

ColumnStrings::ColumnStrings(ColumnStrings::Container data) : data_(std::move(data)) {
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
    return CompressLz4(SerializeData(data_)).size();
}

void ColumnStrings::ApplyPermutation(const std::vector<size_t>& order) {
    data_ = CompressionSorts::ApplyPermutation(data_, order);
}

size_t ColumnStrings::CalculateDistinctValuesInRange(const Range& range) const {
    assert(range.from <= range.to);
    auto begin = std::next(data_.begin(), range.from);
    auto end = std::next(data_.begin(), range.to);
    std::unordered_set<std::string> elements{begin, end};
    return elements.size();
}

void ColumnStrings::UpdatePermutation(std::vector<size_t>& order, const Range& range,
                                      Algorithms algorithm) const {
    assert(range.from <= range.to);
    switch (algorithm) {
        case Algorithms::LexicographicSort: {
            auto comparator = [&](size_t i, size_t j) { return data_[i] < data_[j]; };
            auto begin = std::next(order.begin(), range.from);
            auto end = std::next(order.begin(), range.to);
            std::sort(begin, end, comparator);
            break;
        }
    }
}

OnlineCompressionCalculatorPtr ColumnStrings::GetOnlineCompressionCalculator() const {
    return std::make_unique<StupidOnlineCompressionCalculator<std::string>>(data_);
}

EqualRanges ColumnStrings::GetEqualRanges(const std::vector<size_t>& order,
                                          const Range& range) const {
    return ::CompressionSorts::GetEqualRanges(data_, order, range);
}

std::vector<char> ColumnStrings::GetSerializedData() const {
    return SerializeData(data_);
}

}  // namespace CompressionSorts
