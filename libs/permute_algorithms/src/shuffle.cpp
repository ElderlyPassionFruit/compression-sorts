#include "compression_sorts/shuffle.hpp"

// #include "compression_sorts/permutation.hpp"

namespace CompressionSorts {

// namespace {

// template <typename T>
// class ShufflePermutationImpl {
// public:
//     ShufflePermutationImpl(Time budget, const std::vector<T>& data, std::vector<size_t>& order)
//         : budget_(budget), calculator_(data), order_(order),
//         score_(calculator_.GetCurrentScore()) { order_.resize(data.size()); iota(order_.begin(),
//         order_.end(), 0);
//     }

//     void Run() {
//         TimeBudget time_budget(budget_);
//         while (time_budget()) {
//             TryImprove();
//         }
//     }

//     bool TryImprove() {
//         auto new_order = GenRandomPermutation(order_.size());
//         calculator_.SetPermutation(new_order);
//         size_t new_score = calculator_.GetCurrentScore();
//         if (new_score >= score_) {
//             return false;
//         }
//         order_ = new_order;
//         score_ = new_score;
//         return true;
//     }

// private:
//     const Time budget_;

//     Lz4OnlineCalculator<T> calculator_;
//     std::vector<size_t>& order_;
//     size_t score_;
// };

// }  // namespace

namespace {

// bool TryImprove(Block& block, std::vector<size_t>& order, size_t score) {
//     auto new_order = GenRandomPermutation(order.size());
//     block.ApplyPermutation(const std::vector<size_t>&)

//         size_t new_score = calculator_.GetCurrentScore();
//     if (new_score >= score_) {
//         return false;
//     }
//     order_ = new_order;
//     score_ = new_score;
//     return true;
// }

}  // namespace

ShufflePermute::ShufflePermute(Time budget) : budget_(budget) {
}

void ShufflePermute::GetPermutation(Block& /*block*/, std::vector<size_t>& /*order*/) const {
    // TimeBudget time_budget(budget_);
    // size_t score = block.CalculateCompressionSize();
    // while (time_budget()) {
    //     TryImprove(block, order, score);
    // }
    throw std::runtime_error("Not implemented");
}

std::string ShufflePermute::GetName() const {
    return "shuffle-" + std::to_string(budget_.count());
}

}  // namespace CompressionSorts
