#include "compression_sorts/permutation.hpp"

#include <gtest/gtest.h>

#include <random>

#include "compression_sorts/random.hpp"

void CheckIsInverse(const std::vector<size_t>& a, const std::vector<size_t>& inverse_a) {
    auto lhs_id = CompressionSorts::ApplyPermutation(a, inverse_a);
    auto rhs_id = CompressionSorts::ApplyPermutation(inverse_a, a);
    auto id = CompressionSorts::GetIdentityPermutation(a.size());
    EXPECT_EQ(lhs_id, id);
    EXPECT_EQ(rhs_id, id);
}

TEST(Permutation, UnitInversePermutation) {
    {
        std::vector<size_t> a = {0, 1, 2};
        auto inverse_a = CompressionSorts::GetInversePermutation(a);
        EXPECT_EQ(a, inverse_a);
        CheckIsInverse(a, inverse_a);
    }
    {
        std::vector<size_t> a = {1, 0, 2};
        const auto inverse_a = CompressionSorts::GetInversePermutation(a);
        EXPECT_EQ(a, inverse_a);
        CheckIsInverse(a, inverse_a);
    }
    {
        std::vector<size_t> a = {1, 2, 0};
        const auto inverse_a = CompressionSorts::GetInversePermutation(a);
        const std::vector<size_t> correct_inverse{2, 0, 1};
        EXPECT_EQ(correct_inverse, inverse_a);
        CheckIsInverse(a, inverse_a);
    }
}

TEST(Permutation, StressInversePermutation) {
    constexpr size_t kIterations = 1000;
    constexpr size_t kMaxPermutationSize = 100;
    std::uniform_int_distribution<size_t> size_distribution(1, kMaxPermutationSize);

    for (size_t i = 0; i < kIterations; ++i) {
        const size_t n = size_distribution(CompressionSorts::GetTwister());

        const auto p = CompressionSorts::GenRandomPermutation(n);
        const auto inverse_p = CompressionSorts::GetInversePermutation(p);

        CheckIsInverse(p, inverse_p);
    }
}
