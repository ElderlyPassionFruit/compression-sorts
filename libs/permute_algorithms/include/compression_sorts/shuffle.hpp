#pragma once

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/time.hpp"
#include "permute_interface.hpp"

namespace CompressionSorts {

namespace {

template <typename T>
class ShufflePermutationImpl {
public:
    ShufflePermutationImpl(Time budget, const std::vector<T>& data, std::vector<int>& order)
        : budget_(budget), calculator_(data), order_(order), score_(calculator_.GetCurrentScore()) {
        order_.resize(data.size());
        iota(order_.begin(), order_.end(), 0);
    }

    void Run() {
        TimeBudget time_budget(budget_);
        while (time_budget()) {
            TryImprove();
        }
    }

    bool TryImprove() {
        auto new_order = GenRandomPermutation(order_.size());
        calculator_.SetPermutation(new_order);
        size_t new_score = calculator_.GetCurrentScore();
        if (new_score >= score_) {
            return false;
        }
        order_ = new_order;
        score_ = new_score;
        return true;
    }

private:
    const Time budget_;

    Lz4OnlineCalculator<T> calculator_;
    std::vector<int>& order_;
    size_t score_;
};

}  // namespace

template <typename T>
class ShufflePermutation : public IPermute<T> {
public:
    ShufflePermutation(Time budget) : budget_(budget) {
    }

    void GetOrder(const std::vector<T>& data, std::vector<int>& order) const override {
        ShufflePermutationImpl<T> impl(budget_, data, order);
        impl.Run();
    }

    std::string GetName() const override {
        return "shuffle-" + std::to_string(budget_.count());
    }

private:
    const Time budget_;
};

}  // namespace CompressionSorts
