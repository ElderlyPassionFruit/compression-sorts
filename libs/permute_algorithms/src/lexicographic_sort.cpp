#include "compression_sorts/lexicographic_sort.hpp"

#include <algorithm>

#include "compression_sorts/column_interface.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"

namespace CompressionSorts {

namespace {

void AddRanges(EqualRanges& equal_ranges, EqualRanges new_equal_ranges) {
    equal_ranges.reserve(equal_ranges.size() + new_equal_ranges.size());
    for (const auto& range : new_equal_ranges) {
        equal_ranges.push_back(range);
    }
}

void UpdateUniqueElementsOrder(const Block::Container& columns, size_t from, Range range,
                               std::vector<size_t>& columns_order) {
    std::vector<size_t> unique_elements(columns.size());
    for (size_t i = from; i < columns.size(); ++i) {
        size_t column_id = columns_order[i];
        unique_elements[column_id] = columns[column_id]->CalculateDistinctValuesInRange(range);
    }
    auto begin = std::ranges::next(columns_order.begin(), from);
    auto end = columns_order.end();
    std::sort(begin, end, [&unique_elements](size_t i, size_t j) {
        return unique_elements[i] < unique_elements[j];
    });
}

}  // namespace

void LexicographicSortPermute::GetPermutation(const Block& block,
                                              std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    EqualRanges equal_ranges{Range{.from = 0, .to = block_size}};
    auto& columns = block.GetData();
    order = GetIdentityPermutation(block_size);
    for (const auto& column : columns) {
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

std::string LexicographicSortPermute::GetName() const {
    return "lexicographic sort";
}

void LexicographicSortOfflineColumnOrderPermute::GetPermutation(const Block& block,
                                                                std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    EqualRanges equal_ranges{Range{.from = 0, .to = block_size}};
    auto& columns = block.GetData();

    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    UpdateUniqueElementsOrder(columns, 0, Range{.from = 0, .to = block_size}, columns_order);

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

std::string LexicographicSortOfflineColumnOrderPermute::GetName() const {
    return "lexicographic sort offline column order";
}

void LexicographicSortOnlineColumnOrderPermute::GetPermutation(const Block& block,
                                                               std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    auto& columns = block.GetData();
    std::vector<size_t> columns_order = GetIdentityPermutation(columns.size());
    order = GetIdentityPermutation(block_size);
    GetPermutation(block, 0, columns_order, Range{.from = 0, .to = block_size}, order);
}

void LexicographicSortOnlineColumnOrderPermute::GetPermutation(const Block& block,
                                                               size_t processed_columns,
                                                               std::vector<size_t> columns_order,
                                                               Range range,
                                                               std::vector<size_t>& order) const {
    assert(range.from <= range.to);
    if (range.from == range.to) {
        return;
    }
    const auto& columns = block.GetData();
    assert(columns.size() == columns_order.size());
    UpdateUniqueElementsOrder(columns, processed_columns, range, columns_order);
    size_t column_id = columns_order[processed_columns];
    const auto& column = columns[column_id];
    column->UpdatePermutation(order, range, Algorithms::LexicographicSort);
    if (processed_columns + 1 < columns_order.size()) {
        auto equal_ranges = column->GetEqualRanges(order, range);
        for (const auto& new_equal_range : equal_ranges) {
            GetPermutation(block, processed_columns + 1, columns_order, new_equal_range, order);
        }
    }
}

std::string LexicographicSortOnlineColumnOrderPermute::GetName() const {
    return "lexicographic sort online column order";
}

}  // namespace CompressionSorts
