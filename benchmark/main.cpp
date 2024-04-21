#include "compression_sorts/benchmark.hpp"
#include "compression_sorts/get_all_files.hpp"
#include "compression_sorts/identity.hpp"
#include "compression_sorts/interfaces.hpp"
#include "compression_sorts/statistics_saver.hpp"

using namespace CompressionSorts;

template <typename T>
void TestAllBenchmarksWithAlgorithm(Path dir, const CompressionSorts::IPermute<T>& algoritm) {
    auto paths = GetAllFiles(dir);
    for (const auto& path : paths) {
        auto benchmark_results = TestAlgorithm(path, algoritm, 1);
        SaveBenchmarkResults(benchmark_results);
    }
}

int main() {
    CompressionSorts::IdentityPermutation<int> identity;
    TestAllBenchmarksWithAlgorithm<int>("tests_data/int", identity);
}
