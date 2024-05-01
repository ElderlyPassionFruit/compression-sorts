#include "compression_sorts/lexicographic_sort.hpp"

#include "compression_sorts/range.hpp"

namespace CompressionSorts {

namespace {

void AddRanges(EqualRanges& equal_ranges, EqualRanges new_equal_ranges) {
    equal_ranges.reserve(equal_ranges.size() + new_equal_ranges.size());
    for (const auto& range : new_equal_ranges) {
        equal_ranges.push_back(range);
    }
}

}  // namespace

void LexicographicSortPermute::GetPermutation(Block& block, std::vector<size_t>& order) const {
    EqualRanges equal_ranges{Range{.from = 0, .to = block.GetSize()}};
    auto& columns = block.GetData();
    for (const auto& column : columns) {
        EqualRanges new_equal_ranges;
        for (const auto& range : equal_ranges) {
            column->UpdatePermutation(order, range, Algorithms::LexicographicSort);
            auto current_equal_ranges = column->GetEqualRanges(order, range);
            AddRanges(new_equal_ranges, std::move(current_equal_ranges));
        }
        equal_ranges.swap(new_equal_ranges);
    }
}

std::string LexicographicSortPermute::GetName() const {
    return "lexicographic sort";
}

}  // namespace CompressionSorts
