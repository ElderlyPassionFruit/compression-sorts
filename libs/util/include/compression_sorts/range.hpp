#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace CompressionSorts {

// Half interval [from, to)
struct Range {
    size_t from;
    size_t to;
};

using EqualRanges = std::vector<Range>;

template <std::equality_comparable T>
EqualRanges GetEqualRanges(const std::vector<T>& data, const std::vector<size_t>& order,
                           const Range& range) {
    assert(data.size() == order.size());
    assert(range.from <= range.to);
    assert(range.to <= order.size());
    EqualRanges equal_ranges;
    for (size_t i = range.from; i < range.to;) {
        size_t j = i;
        for (; j < range.to && data[order[i]] == data[order[j]]; ++j) {
        }
        equal_ranges.push_back({i, j});
        i = j;
    }
    return equal_ranges;
}

}  // namespace CompressionSorts
