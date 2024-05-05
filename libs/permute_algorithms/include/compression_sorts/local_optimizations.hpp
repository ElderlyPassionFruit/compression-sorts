#pragma once

#include "compression_sorts/time.hpp"
#include "permute_interface.hpp"

namespace CompressionSorts {

class LocalOptimizationsPermute : public IPermute {
public:
    explicit LocalOptimizationsPermute(Time /*budget*/);

    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;

private:
    const Time budget_;
};

}  // namespace CompressionSorts
