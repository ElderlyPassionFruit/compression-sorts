#include "compression_sorts/suffix_array_greedy.hpp"

#include "compression_sorts/permutation.hpp"

namespace CompressionSorts {

void SuffixArrayGreedyPermute::GetPermutation(const Block& block,
                                              std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    EqualRanges equal_ranges{Range{.from = 0, .to = block_size}};
    auto& columns = block.GetData();
    order = GetIdentityPermutation(block_size);
    for (const auto& column : columns) {
        EqualRanges new_equal_ranges;
        new_equal_ranges.reserve(equal_ranges.size());
        for (const auto& range : equal_ranges) {
            column->UpdatePermutation(order, range, Algorithms::SuffixArrayGreedy);
            auto current_equal_ranges = column->GetEqualRanges(order, range);
            AddRanges(new_equal_ranges, std::move(current_equal_ranges));
        }
        equal_ranges.swap(new_equal_ranges);
    }
}

std::string SuffixArrayGreedyPermute::GetName() const {
    return "suffix-array-greedy";
}

}  // namespace CompressionSorts
