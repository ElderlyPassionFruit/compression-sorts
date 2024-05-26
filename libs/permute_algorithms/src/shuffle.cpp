#include "compression_sorts/shuffle.hpp"

#include <iostream>

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
    size_t iterations = 0;
    size_t improve_iterations = 0;
    while (time_budget()) {
        if (TryImprove(online_calculator, order, score)) {
            ++improve_iterations;
        }
        ++iterations;
    }
    std::cerr << "ShufflePermute::GetPermutation - size: " << order.size()
              << ", iterations: " << iterations << ", improve_iterations: " << improve_iterations
              << std::endl;
}

std::string ShufflePermute::GetName() const {
    const std::string budget =
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(budget_).count());

    return "shuffle " + budget + "ms";
}

}  // namespace CompressionSorts
