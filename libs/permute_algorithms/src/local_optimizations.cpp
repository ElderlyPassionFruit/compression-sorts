#include "compression_sorts/local_optimizations.hpp"

#include <iostream>
#include <random>

#include "compression_sorts/permutation.hpp"
#include "compression_sorts/random.hpp"

namespace CompressionSorts {

namespace {

bool TryImprove(const OnlineCompressionCalculatorPtr& online_calculator, std::vector<size_t>& order,
                size_t score) {
    std::uniform_int_distribution<> distribution(0, order.size() - 1);
    size_t i = distribution(GetTwister());
    size_t j = distribution(GetTwister());
    online_calculator->Swap(i, j);
    size_t new_score = online_calculator->GetCurrentCompressedSize();
    if (new_score >= score) {
        online_calculator->Swap(i, j);
        return false;
    }
    std::swap(order[i], order[j]);
    score = new_score;
    return true;
}

}  // namespace

LocalOptimizationsPermute::LocalOptimizationsPermute(Time budget) : budget_(budget) {
}

void LocalOptimizationsPermute::GetPermutation(const Block& block,
                                               std::vector<size_t>& order) const {
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
    std::cerr << "LocalOptimizationsPermute: size = " << order.size()
              << " iterations = " << iterations << " improve_iterations = " << improve_iterations
              << std::endl;
}

std::string LocalOptimizationsPermute::GetName() const {
    return "local-optimizations-" + std::to_string(budget_.count());
}

}  // namespace CompressionSorts
