#include "compression_sorts/shuffle.hpp"

#include "compression_sorts/online_compression_calculator_interface.hpp"
#include "compression_sorts/permutation.hpp"

namespace CompressionSorts {

namespace {

bool TryImprove(const OnlineCompressionCalculatorPtr& online_calculator, std::vector<size_t>& order,
                size_t score) {
    auto new_order = GenRandomPermutation(order.size());
    online_calculator->ApplyPermutation(new_order);
    size_t new_score = online_calculator->GetCurrentCompressedSize();
    if (new_score >= score) {
        return false;
    }
    order = new_order;
    score = new_score;
    return true;
}

}  // namespace

ShufflePermute::ShufflePermute(Time budget) : budget_(budget) {
}

void ShufflePermute::GetPermutation(const Block& block, std::vector<size_t>& order) const {
    auto online_calculator = block.GetOnlineCompressionCalculator();
    order = GetIdentityPermutation(block.GetSize());
    TimeBudget time_budget(budget_);
    size_t score = block.CalculateCompressionSize();
    while (time_budget()) {
        TryImprove(online_calculator, order, score);
    }
}

std::string ShufflePermute::GetName() const {
    return "shuffle-" + std::to_string(budget_.count());
}

}  // namespace CompressionSorts
