#include "compression_sorts/suffix_array.hpp"

#include <gtest/gtest.h>

TEST(SuffixArray, UnitGetSuffixArrayGreedyOrder) {
    const std::vector<std::string> a{"1", "2", "3", "1", "2", "3"};
    const auto order = CompressionSorts::GetSuffixArrayGreedyOrder(a);
    const std::vector<size_t> correct_order{0, 3, 4, 1, 5, 2};
    EXPECT_EQ(correct_order, order);
}
