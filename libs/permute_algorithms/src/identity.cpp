#include "compression_sorts/identity.hpp"

#include <numeric>

namespace CompressionSorts {

void IdentityPermute::GetPermutation(Block& block, std::vector<size_t>& order) const {
    order.resize(block.GetSize());
    std::iota(order.begin(), order.end(), 0);
}

std::string IdentityPermute::GetName() const {
    return "identity";
}

}  // namespace CompressionSorts
