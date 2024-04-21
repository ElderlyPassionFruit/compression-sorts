#pragma once

#include <cassert>
#include <vector>

namespace CompressionSorts {

bool IsPermutation(const std::vector<int>& /*order*/);

template <typename T>
std::vector<T> ApplyPermutation(const std::vector<int>& order, const std::vector<T>& a) {
    assert(order.size() == a.size());
    assert(IsPermutation(order));
    size_t n = order.size();
    std::vector<T> ans(n);
    for (size_t i = 0; i < order.size(); ++i) {
        ans[order[i]] = a[i];
    }
    return ans;
}

}  // namespace CompressionSorts
