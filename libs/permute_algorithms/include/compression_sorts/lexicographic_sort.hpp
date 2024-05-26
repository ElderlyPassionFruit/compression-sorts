#pragma once

#include "permute_interface.hpp"

namespace CompressionSorts {

class LexicographicSortPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOfflineCardinalityOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOnlineCardinalityOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOfflineSizeOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

class LexicographicSortOnlineSizeOrderPermute : public IPermute {
public:
    void GetPermutation(const Block& /*block*/, std::vector<size_t>& /*order*/) const override;
    std::string GetName() const override;
};

}  // namespace CompressionSorts
