#include "compression_sorts/suffix_array_greedy.hpp"

#include "compression_sorts/permutation.hpp"

namespace CompressionSorts {

void SuffixArrayGreedyPermute::GetPermutation(const Block& block,
                                              std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    order = GetIdentityPermutation(block_size);
    size_t best_score = block.CalculateCompressionSize();
    const size_t columns_size = block.GetData().size();
    constexpr size_t kMaxIteration = 16;
    auto online_calculator = block.GetOnlineCompressionCalculator();
    for (size_t it = 0; it < kMaxIteration; ++it) {
        const auto column_order = GenRandomPermutation(columns_size);
        std::vector<size_t> current_order;
        GetPermutationForColumnOrder(block, column_order, current_order);
        online_calculator->ApplyPermutation(current_order);
        size_t current_score = online_calculator->GetCurrentCompressedSize();
        if (current_score < best_score) {
            order = current_order;
            best_score = current_score;
        }
        online_calculator->ApplyPermutation(GetInversePermutation(current_order));
    }
}

void SuffixArrayGreedyPermute::GetPermutationForColumnOrder(const Block& block,
                                                            const std::vector<size_t>& column_order,
                                                            std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    EqualRanges equal_ranges{Range{.from = 0, .to = block_size}};
    auto& columns = block.GetData();
    assert(column_order.size() == columns.size());
    order = GetIdentityPermutation(block_size);
    for (size_t i = 0; i < columns.size(); ++i) {
        auto& column = columns[column_order[i]];
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
