#include "compression_sorts/column_integers.hpp"

#include <gtest/gtest.h>

#include <cstddef>

#include "compression_sorts/column_interface.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"

TEST(ColumnIntegers, UnitJustWorks) {
    // plain object
    {
        CompressionSorts::ColumnIntegers<int32_t> column(std::vector<int32_t>{1, 2, 3, 4, 5});
        EXPECT_EQ(5, column.GetSize());
        EXPECT_EQ(std::string{"integer"}, column.GetTypeName());
    }
    // smart pointer
    {
        CompressionSorts::ColumnPtr column =
            std::make_unique<CompressionSorts::ColumnIntegers<int8_t>>(
                std::vector<int8_t>{-4, 3, 2, 1});
        EXPECT_EQ(4, column->GetSize());
        EXPECT_EQ(std::string{"integer"}, column->GetTypeName());
    }
    // permuting
    {
        std::vector<int64_t> data{57, 179, 256};
        CompressionSorts::ColumnIntegers<int64_t> column(data);
        EXPECT_EQ(data, column.GetData());
        std::vector<size_t> order{2, 0, 1};
        column.ApplyPermutation(order);
        data = {256, 57, 179};
        EXPECT_EQ(data, column.GetData());
        column.UpdatePermutation(order, CompressionSorts::Range{.from = 0, .to = 3},
                                 CompressionSorts::Algorithms::LexicographicSort);
        std::vector<size_t> new_order = {1, 2, 0};
        EXPECT_EQ(new_order, order);
        EXPECT_EQ(data, column.GetData());
        column.ApplyPermutation(order);
        data = {57, 179, 256};
        EXPECT_EQ(data, column.GetData());
        std::vector<CompressionSorts::Range> correct_ranges{
            {
                .from = 0,
                .to = 1,
            },
            {
                .from = 1,
                .to = 2,
            },
            {
                .from = 2,
                .to = 3,
            },
        };
        EXPECT_EQ(correct_ranges,
                  column.GetEqualRanges(order, CompressionSorts::Range{.from = 0, .to = 3}));
    }
    // equal ranges
    {

        CompressionSorts::ColumnPtr column =
            std::make_unique<CompressionSorts::ColumnIntegers<int8_t>>(
                std::vector<int8_t>{1, 1, 10, 10, 1});
        std::vector<CompressionSorts::Range> correct_ranges{
            {
                .from = 0,
                .to = 2,
            },
            {
                .from = 2,
                .to = 4,
            },
            {
                .from = 4,
                .to = 5,
            },
        };
        const size_t size = column->GetSize();
        std::vector<size_t> order = CompressionSorts::GetIdentityPermutation(size);
        EXPECT_EQ(correct_ranges,
                  column->GetEqualRanges(order, CompressionSorts::Range{.from = 0, .to = size}));
        correct_ranges = {
            {
                .from = 0,
                .to = 2,
            },
            {
                .from = 2,
                .to = 3,
            },
            {
                .from = 3,
                .to = 4,
            },
            {
                .from = 4,
                .to = 5,
            },
        };
        column->UpdatePermutation(order, CompressionSorts::Range{.from = 3, .to = 5},
                                  CompressionSorts::Algorithms::LexicographicSort);
        EXPECT_EQ(correct_ranges,
                  column->GetEqualRanges(order, CompressionSorts::Range{.from = 0, .to = size}));
        column->ApplyPermutation(order);
        order = CompressionSorts::GetIdentityPermutation(size);
        EXPECT_EQ(correct_ranges,
                  column->GetEqualRanges(order, CompressionSorts::Range{.from = 0, .to = size}));
        correct_ranges = {
            {
                .from = 2,
                .to = 3,
            },
            {
                .from = 3,
                .to = 4,
            },
            {
                .from = 4,
                .to = 5,
            },
        };
        EXPECT_EQ(correct_ranges,
                  column->GetEqualRanges(order, CompressionSorts::Range{.from = 2, .to = 5}));

        column->UpdatePermutation(order, CompressionSorts::Range{.from = 0, .to = size},
                                  CompressionSorts::Algorithms::LexicographicSort);
        column->ApplyPermutation(order);
        order = CompressionSorts::GetIdentityPermutation(size);

        for (size_t from = 0; from < size; ++from) {
            for (size_t to = from; to < size; ++to) {
                auto new_order = order;
                column->UpdatePermutation(new_order, CompressionSorts::Range{.from = 0, .to = size},
                                          CompressionSorts::Algorithms::LexicographicSort);
                EXPECT_EQ(order, new_order);
            }
        }
    }
}
