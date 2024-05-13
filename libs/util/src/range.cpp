#include "compression_sorts/range.hpp"

#include <tuple>

namespace CompressionSorts {

bool Range::operator==(const Range& other) const {
    return std::tie(from, to) == std::tie(other.from, other.to);
}

size_t Range::Size() const {
    assert(from <= to);
    return to - from;
}

void AddRanges(EqualRanges& equal_ranges, EqualRanges new_equal_ranges) {
    equal_ranges.reserve(equal_ranges.size() + new_equal_ranges.size());
    for (const auto& range : new_equal_ranges) {
        equal_ranges.push_back(range);
    }
}

}  // namespace CompressionSorts
