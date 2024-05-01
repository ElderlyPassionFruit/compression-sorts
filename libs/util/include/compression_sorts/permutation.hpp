#pragma once

#include <cassert>
#include <vector>

namespace CompressionSorts {

bool IsPermutation(const std::vector<size_t>& /*order*/);

std::vector<size_t> GenRandomPermutation(size_t /*n*/);

template <typename T>
std::vector<T> ApplyPermutation(const std::vector<T>& data, const std::vector<size_t>& order) {
    assert(order.size() == data.size());
    assert(IsPermutation(order));
    size_t n = order.size();
    std::vector<T> ans(n);
    for (size_t i = 0; i < order.size(); ++i) {
        ans[i] = data[order[i]];
    }
    return ans;
}

}  // namespace CompressionSorts
