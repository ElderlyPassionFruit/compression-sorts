#include "compression_sorts/permutation.hpp"

#include <numeric>
#include <random>

namespace CompressionSorts {

bool IsPermutation(const std::vector<size_t>& order) {
    std::vector<size_t> id(order.size());
    std::iota(id.begin(), id.end(), 0);
    return std::is_permutation(order.begin(), order.end(), id.begin());
}

std::vector<size_t> GenRandomPermutation(size_t n) {
    static std::mt19937_64 rnd(57);
    std::vector<size_t> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(), rnd);
    return permutation;
}

}  // namespace CompressionSorts
