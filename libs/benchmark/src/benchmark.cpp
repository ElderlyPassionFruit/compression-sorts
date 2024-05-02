#include "compression_sorts/benchmark.hpp"

#include <cassert>

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/test_results.hpp"

namespace CompressionSorts {

CompressionScore CalculateCompressionScore(const IColumn& column) {
    CompressionScore score;

    auto serialized_data = column.GetSerializedData();
    score.serialized_size = serialized_data.size();

    std::vector<char> compressed_data;
    score.compression_time_ns = CalculateLz4CompressionTime(serialized_data, compressed_data);
    score.compressed_size = compressed_data.size();

    std::vector<char> verify_serialized_data;
    verify_serialized_data.resize(serialized_data.size());
    score.decompression_time_ns =
        CalculateLz4DecompressionTime(compressed_data, verify_serialized_data);
    assert(verify_serialized_data == serialized_data);
    return score;
}

CompressionScore CalculateCompressionScore(const Block& block) {
    CompressionScore block_score;
    for (const auto& column : block.GetData()) {
        const CompressionScore column_score = CalculateCompressionScore(*column);
        block_score += column_score;
    }
    return block_score;
}

Time CalculateFindPermutationTime(const Block& block, const IPermute& algorithm,
                                  std::vector<size_t>& order) {
    auto helper = [&block, &algorithm, &order]() mutable {
        algorithm.GetPermutation(block, order);
    };
    return MesuareTime(helper);
}

BenchmarkResults TestAlgorithm(const std::string& test_name, Block block, const IPermute& algorithm,
                               const size_t iterations) {
    BenchmarkResults benchmark_results;
    benchmark_results.name = test_name;
    benchmark_results.algorithm_name = algorithm.GetName();

    benchmark_results.raws = block.GetSize();
    benchmark_results.columns = block.GetData().size();

    benchmark_results.initial_score = CalculateCompressionScore(block);

    for (size_t i = 0; i < iterations; ++i) {
        TestResults test_results;

        std::vector<size_t> order;
        test_results.find_permutation_time = CalculateFindPermutationTime(block, algorithm, order);
        auto inverse_order = GetInversePermutation(order);
        block.ApplyPermutation(order);
        test_results.permuted_score = CalculateCompressionScore(block);
        block.ApplyPermutation(inverse_order);
        benchmark_results.permuted_scores.emplace_back(test_results);
    }

    return benchmark_results;
}

}  // namespace CompressionSorts
