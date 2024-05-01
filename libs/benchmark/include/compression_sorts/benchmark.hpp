#pragma once

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/path.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/permute_interface.hpp"
#include "compression_sorts/read_data.hpp"
#include "compression_sorts/serialize_data.hpp"
#include "compression_sorts/time.hpp"
#include "test_results.hpp"

namespace CompressionSorts {

template <typename T>
CompressionScore CalculateCompressionScore(const std::vector<T>& data) {
    CompressionScore score;

    auto serialized_data = SerializeData(data);
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

template <typename T>
Time CalculateFindPermutationTime(const std::vector<T>& data, const IPermute<T>& algorithm,
                                  std::vector<size_t>& order) {
    auto helper = [&algorithm, &data, &order] { algorithm.GetOrder(data, order); };
    return MesuareTime(helper);
}

template <typename T>
BenchmarkResults TestAlgorithm(Path path, const IPermute<T>& algorithm, const size_t iterations) {
    auto data = ReadData<T>(path);
    BenchmarkResults benchmark_results;
    benchmark_results.name = path.string();
    benchmark_results.algorithm_name = algorithm.GetName();

    benchmark_results.array_length = data.size();
    benchmark_results.initial_score = CalculateCompressionScore(data);

    for (size_t i = 0; i < iterations; ++i) {
        TestResults test_results;

        std::vector<size_t> order;
        test_results.find_permutation_time = CalculateFindPermutationTime(data, algorithm, order);
        auto permuted_data = ApplyPermutation(data, order);
        test_results.permuted_score = CalculateCompressionScore(permuted_data);

        benchmark_results.permuted_scores.emplace_back(test_results);
    }

    return benchmark_results;
}

}  // namespace CompressionSorts
