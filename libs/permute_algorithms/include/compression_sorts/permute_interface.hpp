#pragma once

#include <string>
#include <vector>

#include "compression_sorts/block.hpp"

namespace CompressionSorts {

struct IPermute {
    virtual void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const = 0;
    virtual std::string GetName() const = 0;

    virtual ~IPermute() = default;
};

}  // namespace CompressionSorts
