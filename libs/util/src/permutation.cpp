#include "compression_sorts/permutation.hpp"

#include <numeric>
#include <random>

namespace CompressionSorts {

bool IsPermutation(const std::vector<size_t>& order) {
    auto id = GetIdentityPermutation(order.size());
    return std::is_permutation(order.begin(), order.end(), id.begin());
}

std::vector<size_t> GetIdentityPermutation(size_t n) {
    std::vector<size_t> id(n);
    std::iota(id.begin(), id.end(), 0);
    return id;
}

std::vector<size_t> GenRandomPermutation(size_t n) {
    static std::mt19937_64 rnd(57);
    auto permutation = GetIdentityPermutation(n);
    std::shuffle(permutation.begin(), permutation.end(), rnd);
    return permutation;
}

std::vector<size_t> GetInversePermutation(const std::vector<size_t>& order) {
    std::vector<size_t> inverse(order.size());
    for (size_t i = 0; i < order.size(); ++i) {
        inverse[order[i]] = i;
    }
    assert(IsPermutation(order));
    return inverse;
}

}  // namespace CompressionSorts
