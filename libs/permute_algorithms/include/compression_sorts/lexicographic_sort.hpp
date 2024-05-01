#pragma once

#include "permute_interface.hpp"

namespace CompressionSorts {

class LexicographicSortPermute : public IPermute {
public:
    void GetPermutation(Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

}  // namespace CompressionSorts
