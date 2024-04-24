#pragma once

#include <cassert>
// #include <iostream>
#include <vector>

namespace CompressionSorts {

bool IsPermutation(const std::vector<int>& /*order*/);

std::vector<int> GenRandomPermutation(size_t /*n*/);

template <typename T>
std::vector<T> ApplyPermutation(const std::vector<T>& data, const std::vector<int>& order) {
    assert(order.size() == data.size());
    // std::cerr << "order = " << std::endl;
    // for (auto i : order) {
    //     std::cerr << i << " ";
    // }
    // std::cerr << std::endl;
    assert(IsPermutation(order));
    size_t n = order.size();
    std::vector<T> ans(n);
    for (size_t i = 0; i < order.size(); ++i) {
        ans[i] = data[order[i]];
    }
    return ans;
}

}  // namespace CompressionSorts
