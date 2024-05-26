#include "compression_sorts/column_strings.hpp"

#include <algorithm>
#include <cstddef>
#include <unordered_set>

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"
#include "compression_sorts/serialize_data.hpp"
#include "compression_sorts/stupid_compression_calculator.hpp"
#include "compression_sorts/suffix_array.hpp"

namespace CompressionSorts {

namespace {

void SuffixArrayGreedyPermuteImpl(const std::vector<std::string>& data, std::vector<size_t>& order,
                                  const Range& range) {
    std::vector<std::string> to_order(range.Size());

    for (size_t i = range.from; i < range.to; ++i) {
        to_order[i - range.from] = data[order[i]];
    }

    auto current_order = GetSuffixArrayGreedyOrder(to_order);

    for (size_t i = 0; i < current_order.size(); ++i) {
        current_order[i] = order[current_order[i] + range.from];
    }

    for (size_t i = range.from; i < range.to; ++i) {
        order[i] = current_order[i - range.from];
    }
}

}  // namespace

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
    auto begin = std::ranges::next(data_.begin(), range.from);
    auto end = std::ranges::next(data_.begin(), range.to);
    std::unordered_set<std::string> elements{begin, end};
    return elements.size();
}

size_t ColumnStrings::CalculateSerializedSizeInRange(const Range& range) const {
    assert(range.from <= range.to);
    auto begin = std::ranges::next(data_.begin(), range.from);
    auto end = std::ranges::next(data_.begin(), range.to);
    return SerializeData<std::string>({begin, end}).size();
}

void ColumnStrings::UpdatePermutation(std::vector<size_t>& order, const Range& range,
                                      Algorithms algorithm) const {
    assert(range.from <= range.to);
    if (range.Size() <= 1ULL) {
        return;
    }
    switch (algorithm) {
        case Algorithms::LexicographicSort: {
            auto comparator = [&](size_t i, size_t j) { return data_[i] < data_[j]; };
            auto begin = std::ranges::next(order.begin(), range.from);
            auto end = std::ranges::next(order.begin(), range.to);
            std::sort(begin, end, comparator);
            break;
        }
        case Algorithms::SuffixArrayGreedy: {
            SuffixArrayGreedyPermuteImpl(data_, order, range);
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

int ColumnStrings::Compare(size_t lhs, size_t rhs) const {
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

}  // namespace CompressionSorts
