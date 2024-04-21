#pragma once

#include <string>
#include <vector>

namespace CompressionSorts {

template <typename T>
struct IPermute {
    virtual void GetOrder(const std::vector<T>& /*data*/, std::vector<int>& /*order*/) const = 0;
    virtual std::string GetName() const = 0;
};

}  // namespace CompressionSorts
