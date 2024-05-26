#include "compression_sorts/lexicographic_sort.hpp"

#include <algorithm>

#include "compression_sorts/column_interface.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"

namespace CompressionSorts {

namespace {

using ColumnRangeScoreCalculator = std::function<size_t(const ColumnPtr&, Range)>;
using ScoreComparator = std::function<bool(size_t, size_t)>;

void UpdateColumnsOrder(const Block::Container& columns, size_t from, Range range,
                        std::vector<size_t>& columns_order, ColumnRangeScoreCalculator calculator,
                        ScoreComparator comparator) {
    std::vector<size_t> score(columns.size());
    for (size_t i = from; i < columns.size(); ++i) {
        size_t column_id = columns_order[i];
        score[column_id] = calculator(columns[column_id], range);
    }
    auto begin = std::ranges::next(columns_order.begin(), from);
    auto end = columns_order.end();
    std::sort(begin, end,
              [&score, &comparator](size_t i, size_t j) { return comparator(score[i], score[j]); });
}

void GetLexicographicSortPermutationWithColumnsOrder(const Block& block,
                                                     const std::vector<size_t>& columns_order,
                                                     std::vector<size_t>& order) {
    const size_t block_size = block.GetSize();
    EqualRanges equal_ranges{Range{.from = 0, .to = block_size}};
    auto& columns = block.GetData();

    order = GetIdentityPermutation(block_size);
    for (size_t column_id : columns_order) {
        const auto& column = columns[column_id];
        EqualRanges new_equal_ranges;
        new_equal_ranges.reserve(equal_ranges.size());
        for (const auto& range : equal_ranges) {
            column->UpdatePermutation(order, range, Algorithms::LexicographicSort);
            auto current_equal_ranges = column->GetEqualRanges(order, range);
            AddRanges(new_equal_ranges, std::move(current_equal_ranges));
        }
        equal_ranges.swap(new_equal_ranges);
    }
}

void GetLexicographicSortPermutationWithOnlineColumnsOrder(
    const Block& block, size_t processed_columns, std::vector<size_t> columns_order, Range range,
    ColumnRangeScoreCalculator calculator, ScoreComparator comparator, std::vector<size_t>& order) {
    assert(range.from <= range.to);
    if (range.from == range.to) {
        return;
    }
    const auto& columns = block.GetData();
    assert(columns.size() == columns_order.size());
    UpdateColumnsOrder(columns, processed_columns, range, columns_order, calculator, comparator);
    size_t column_id = columns_order[processed_columns];
    const auto& column = columns[column_id];
    column->UpdatePermutation(order, range, Algorithms::LexicographicSort);
    if (processed_columns + 1 < columns_order.size()) {
        auto equal_ranges = column->GetEqualRanges(order, range);
        for (const auto& new_equal_range : equal_ranges) {
            GetLexicographicSortPermutationWithOnlineColumnsOrder(block, processed_columns + 1,
                                                                  columns_order, new_equal_range,
                                                                  calculator, comparator, order);
        }
    }
}

}  // namespace

void LexicographicSortPermute::GetPermutation(const Block& block,
                                              std::vector<size_t>& order) const {
    const auto& columns = block.GetData();
    const std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    GetLexicographicSortPermutationWithColumnsOrder(block, columns_order, order);
}

std::string LexicographicSortPermute::GetName() const {
    return "lexicographic sort";
}

void LexicographicSortOfflineCardinalityOrderPermute::GetPermutation(
    const Block& block, std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    auto& columns = block.GetData();
    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    auto calculator = [](const ColumnPtr& column, Range range) {
        return column->CalculateDistinctValuesInRange(range);
    };
    UpdateColumnsOrder(columns, 0, Range{.from = 0, .to = block_size}, columns_order, calculator,
                       std::less<size_t>());

    GetLexicographicSortPermutationWithColumnsOrder(block, columns_order, order);
}

std::string LexicographicSortOfflineCardinalityOrderPermute::GetName() const {
    return "lexicographic sort offline cardinality order";
}

void LexicographicSortOnlineCardinalityOrderPermute::GetPermutation(
    const Block& block, std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    auto& columns = block.GetData();
    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    auto calculator = [](const ColumnPtr& column, Range range) {
        return column->CalculateDistinctValuesInRange(range);
    };
    order = GetIdentityPermutation(block_size);
    GetLexicographicSortPermutationWithOnlineColumnsOrder(block, 0, columns_order,
                                                          Range{.from = 0, .to = block_size},
                                                          calculator, std::less<size_t>(), order);
}

std::string LexicographicSortOnlineCardinalityOrderPermute::GetName() const {
    return "lexicographic sort online cardinality order";
}

void LexicographicSortOfflineSizeOrderPermute::GetPermutation(const Block& block,
                                                              std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    auto& columns = block.GetData();
    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    auto calculator = [](const ColumnPtr& column, Range range) {
        return column->CalculateSerializedSizeInRange(range);
    };
    UpdateColumnsOrder(columns, 0, Range{.from = 0, .to = block_size}, columns_order, calculator,
                       std::greater<size_t>());

    GetLexicographicSortPermutationWithColumnsOrder(block, columns_order, order);
}

std::string LexicographicSortOfflineSizeOrderPermute::GetName() const {
    return "lexicographic sort offline size order";
}

void LexicographicSortOnlineSizeOrderPermute::GetPermutation(const Block& block,
                                                             std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    auto& columns = block.GetData();
    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    auto calculator = [](const ColumnPtr& column, Range range) {
        return column->CalculateSerializedSizeInRange(range);
    };
    order = GetIdentityPermutation(block_size);
    GetLexicographicSortPermutationWithOnlineColumnsOrder(block, 0, columns_order,
                                                          Range{.from = 0, .to = block_size},
                                                          calculator, std::less<size_t>(), order);
}

std::string LexicographicSortOnlineSizeOrderPermute::GetName() const {
    return "lexicographic sort online size order";
}

}  // namespace CompressionSorts
