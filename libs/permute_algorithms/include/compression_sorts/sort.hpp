#pragma once

#include <numeric>

#include "permute_interface.hpp"

namespace CompressionSorts {

template <typename T>
class SortPermutation : public IPermute<T> {

public:
    void GetOrder(const std::vector<T>& data, std::vector<int>& order) const override {
        order.resize(data.size());
        std::iota(order.begin(), order.end(), 0);
        std::stable_sort(order.begin(), order.end(),
                         [&](int i, int j) { return data[i] < data[j]; });
    }

    std::string GetName() const override {
        return "Sort";
    }
};

}  // namespace CompressionSorts
