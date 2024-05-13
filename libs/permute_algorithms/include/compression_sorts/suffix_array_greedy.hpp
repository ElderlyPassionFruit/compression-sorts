#pragma once

#include "permute_interface.hpp"

namespace CompressionSorts {

class SuffixArrayGreedyPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;

private:
    void GetPermutationForColumnOrder(const Block& /*block*/,
                                      const std::vector<size_t>& /*column_order*/,
                                      std::vector<size_t>& /*order*/) const;
};

}  // namespace CompressionSorts
