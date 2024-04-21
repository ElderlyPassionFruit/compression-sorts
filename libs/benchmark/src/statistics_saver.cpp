#include "compression_sorts/statistics_saver.hpp"

#include <fstream>
#include <unordered_set>

#include "compression_sorts/test_results.hpp"

namespace CompressionSorts {

namespace {

static const std::string kTestsInfoScheme =
    "name,array_length,serialized_size,compressed_size,compression_time_ns,decompression_time_ns";

static const std::string kAlgorithmsScheme =
    "name,algorithm_name,find_permutation_time,serialized_size,compressed_size,compression_time_ns,"
    "decompression_time_ns";

}  // namespace

class StatisticsSaver {
public:
    StatisticsSaver() {
        tests_info_out.open("tests_info.csv");
        tests_info_out << kTestsInfoScheme << std::endl;

        algorithms_out.open("algorithms_results.csv");
        algorithms_out << kAlgorithmsScheme << std::endl;
    }

    void SaveBenchmarkResults(BenchmarkResults benchmark_results) {
        // for test_info
        if (!used_tests.contains(benchmark_results.name)) {
            used_tests.insert(benchmark_results.name);
            const auto& score = benchmark_results.initial_score;
            tests_info_out << benchmark_results.name << "," << benchmark_results.array_length << ","
                           << score.serialized_size << "," << score.compressed_size << ","
                           << score.compression_time_ns.count() << ","
                           << score.decompression_time_ns.count() << std::endl;
        }
        // for algorithms
        for (const auto& result : benchmark_results.permuted_scores) {
            const auto& score = result.permuted_score;
            algorithms_out << benchmark_results.name << "," << benchmark_results.algorithm_name
                           << "," << result.find_permutation_time.count() << ","
                           << score.serialized_size << "," << score.compressed_size << ","
                           << score.compression_time_ns.count() << ","
                           << score.decompression_time_ns.count() << std::endl;
        }
    }

private:
    std::unordered_set<std::string> used_tests;
    std::ofstream tests_info_out;
    std::ofstream algorithms_out;
};

void SaveBenchmarkResults(BenchmarkResults benchmark_results) {
    static StatisticsSaver saver;
    saver.SaveBenchmarkResults(benchmark_results);
}

}  // namespace CompressionSorts