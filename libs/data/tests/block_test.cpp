#include "compression_sorts/block.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "compression_sorts/column_integers.hpp"
#include "compression_sorts/column_strings.hpp"

TEST(Block, UnitJustWorks) {
    CompressionSorts::Block::Container columns;
    columns.push_back(std::make_unique<CompressionSorts::ColumnIntegers<int8_t>>(
        std::vector<int8_t>{1, -1, 2, -2, 3}));
    columns.push_back(std::make_unique<CompressionSorts::ColumnIntegers<uint64_t>>(
        std::vector<uint64_t>{123, 234, 345, 456, 567}));
    columns.push_back(std::make_unique<CompressionSorts::ColumnStrings>(
        std::vector<std::string>{"a", "aba", "abacaba", "ClickHouse", "The best"}));
    CompressionSorts::Block block(std::move(columns));
    EXPECT_EQ(5, block.GetSize());
    EXPECT_EQ(74, block.GetSerializedSize());
    EXPECT_EQ(64, block.CalculateCompressionSize());
}
