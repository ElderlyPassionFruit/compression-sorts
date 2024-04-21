#pragma once

#include <numeric>

#include "interfaces.hpp"

namespace CompressionSorts {

template <typename T>
class IdentityPermutation : public IPermute<T> {

public:
    void GetOrder(const std::vector<T>& data, std::vector<int>& order) const override {
        order.resize(data.size());
        std::iota(order.begin(), order.end(), 0);
    }

    std::string GetName() const override {
        return "Identity";
    }
};

}  // namespace CompressionSorts
