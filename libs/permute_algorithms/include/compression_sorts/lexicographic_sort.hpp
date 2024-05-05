#pragma once

#include "compression_sorts/range.hpp"
#include "permute_interface.hpp"

namespace CompressionSorts {

class LexicographicSortPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOfflineColumnOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOnlineColumnOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;

private:
    void GetPermutation(const Block& /*block*/, size_t /*processed_columns*/,
                        std::vector<size_t> /*columns_order*/, Range /*range*/,
                        std::vector<size_t>& /*order*/) const;
};

}  // namespace CompressionSorts
