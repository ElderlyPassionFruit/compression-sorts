#include <cstdlib>
#include <iostream>

#include "compression_sorts/benchmark.hpp"
#include "compression_sorts/block.hpp"
#include "compression_sorts/column_integers.hpp"
#include "compression_sorts/column_interface.hpp"
#include "compression_sorts/column_strings.hpp"
#include "compression_sorts/filesystem.hpp"
#include "compression_sorts/identity.hpp"
#include "compression_sorts/lexicographic_sort.hpp"
#include "compression_sorts/local_optimizations.hpp"
#include "compression_sorts/multiple_lists.hpp"
#include "compression_sorts/permute_interface.hpp"
#include "compression_sorts/read_data.hpp"
#include "compression_sorts/shuffle.hpp"
#include "compression_sorts/statistics_saver.hpp"
#include "compression_sorts/suffix_array_greedy.hpp"

using namespace CompressionSorts;

namespace {

void TestAllBenchmarksWithAlgorithms(Path dir, ColumnParser parser,
                                     const std::vector<IPermutePtr>& algorithms,
                                     const size_t iterations) {
    const auto paths = GetAllFiles(dir);
    for (const Path& path : paths) {
        auto data = ReadLines(path);
        for (const auto& algorithm : algorithms) {
            std::cerr << "name: " << path << " algorithm: " << algorithm->GetName() << std::endl;
            Block block = BlockParser(data, parser);
            auto benchmark_results = TestAlgorithm(path, std::move(block), *algorithm, iterations);
            SaveBenchmarkResults(benchmark_results);
        }
    }
}

template <std::integral T>
void TestAllSingleIntegersColumnTests(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    algorithms.push_back(std::make_unique<CompressionSorts::IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<CompressionSorts::LexicographicSortPermute>());

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

template <std::integral T>
void TestAllMannyIntegersColumnsTests(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    algorithms.push_back(std::make_unique<CompressionSorts::IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<CompressionSorts::LexicographicSortPermute>());
    // Offline best order
    algorithms.push_back(
        std::make_unique<CompressionSorts::LexicographicSortOfflineColumnOrderPermute>());
    // Online best order
    algorithms.push_back(
        std::make_unique<CompressionSorts::LexicographicSortOnlineColumnOrderPermute>());
    // Multiple lists
    algorithms.push_back(std::make_unique<CompressionSorts::MultipleListsPermute>());

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

void TestViaStrings(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    algorithms.push_back(std::make_unique<CompressionSorts::IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<CompressionSorts::LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<CompressionSorts::LexicographicSortPermute>());
    // Offline best order
    algorithms.push_back(
        std::make_unique<CompressionSorts::LexicographicSortOfflineColumnOrderPermute>());
    // Online best order
    algorithms.push_back(
        std::make_unique<CompressionSorts::LexicographicSortOnlineColumnOrderPermute>());
    // Suffix array based greedy
    algorithms.push_back(std::make_unique<CompressionSorts::SuffixArrayGreedyPermute>());
    // Multiple lists
    algorithms.push_back(std::make_unique<CompressionSorts::MultipleListsPermute>());

    ColumnParser parser(GenericColumnParser<ColumnStrings>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

}  // namespace

int main() {
    TestAllSingleIntegersColumnTests<int8_t>("tests_data/int/random_small");
    TestAllMannyIntegersColumnsTests<int8_t>("tests_data/int/many_random_small");
    TestAllSingleIntegersColumnTests<int64_t>("tests_data/int/random_big");
    TestViaStrings("tests_data/english/dictionary");
    TestViaStrings("tests_data/clickhouse/hits");
    TestViaStrings("tests_data/clickhouse/dish");
    TestViaStrings("tests_data/clickhouse/menu");
    TestViaStrings("tests_data/clickhouse/menu_item");
    TestViaStrings("tests_data/clickhouse/menu_page");
    TestViaStrings("tests_data/clickhouse/price_paid_transaction_data");
    return EXIT_SUCCESS;
}
