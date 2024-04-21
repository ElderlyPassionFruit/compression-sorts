#include "compression_sorts/permutation.hpp"

#include <numeric>

namespace CompressionSorts {

bool IsPermutation(const std::vector<int>& order) {
    std::vector<int> id(order.size());
    std::iota(id.begin(), id.end(), 0);
    return std::is_permutation(order.begin(), order.end(), id.begin());
}

}  // namespace CompressionSorts
