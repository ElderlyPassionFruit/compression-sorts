#include <iostream>

#include "compression_sorts/benchmark.hpp"
#include "compression_sorts/get_all_files.hpp"
#include "compression_sorts/identity.hpp"
#include "compression_sorts/permute_interface.hpp"
#include "compression_sorts/shuffle.hpp"
#include "compression_sorts/sort.hpp"
#include "compression_sorts/statistics_saver.hpp"

using namespace CompressionSorts;

template <typename T>
void TestAllBenchmarksWithAlgorithm(Path dir, const CompressionSorts::IPermute<T>& algorithm,
                                    const size_t iterations) {
    auto paths = GetAllFiles(dir);
    for (const auto& path : paths) {
        std::cerr << "path = " << path << " algorithm = " << algorithm.GetName() << std::endl;
        auto benchmark_results = TestAlgorithm(path, algorithm, iterations);
        SaveBenchmarkResults(benchmark_results);
    }
}

int main() {
    for (const auto tests_path : {"tests_data/int/random_small", "tests_data/int/random_big"}) {
        // Just nothing
        {
            CompressionSorts::IdentityPermutation<int> identity;
            TestAllBenchmarksWithAlgorithm<int>(tests_path, identity, 3);
        }
        // Shuffle with small budget
        {
            CompressionSorts::ShufflePermutation<int> shuffle(1ms);
            TestAllBenchmarksWithAlgorithm<int>(tests_path, shuffle, 10);
        }
        // Shuffle with big budget
        {
            CompressionSorts::ShufflePermutation<int> shuffle(1s);
            TestAllBenchmarksWithAlgorithm<int>(tests_path, shuffle, 10);
        }
        // Just sort
        {
            CompressionSorts::SortPermutation<int> sort;
            TestAllBenchmarksWithAlgorithm<int>(tests_path, sort, 10);
        }
    }
}
