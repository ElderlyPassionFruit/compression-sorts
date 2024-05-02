#include "compression_sorts/column_integers.hpp"

#include <gtest/gtest.h>

#include "compression_sorts/column_interface.hpp"

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
}
