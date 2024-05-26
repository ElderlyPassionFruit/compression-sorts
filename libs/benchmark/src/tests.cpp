#include "compression_sorts/tests.hpp"

#include "compression_sorts/benchmark.hpp"
#include "compression_sorts/column_integers.hpp"
#include "compression_sorts/column_strings.hpp"
#include "compression_sorts/identity.hpp"
#include "compression_sorts/lexicographic_sort.hpp"
#include "compression_sorts/local_optimizations.hpp"
#include "compression_sorts/multiple_lists.hpp"
#include "compression_sorts/shuffle.hpp"
#include "compression_sorts/suffix_array_greedy.hpp"

namespace CompressionSorts {

template <std::integral T>
void TestAllSingleIntegersColumnTests(Path dir) {
    std::vector<IPermutePtr> algorithms;
    // Just nothing
    algorithms.push_back(std::make_unique<IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<LexicographicSortPermute>());

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

template void TestAllSingleIntegersColumnTests<int8_t>(Path dir);
template void TestAllSingleIntegersColumnTests<int64_t>(Path dir);

template <std::integral T>
void TestAllManyIntegersColumnsTests(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    algorithms.push_back(std::make_unique<IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<LexicographicSortPermute>());
    // Offline best order
    algorithms.push_back(std::make_unique<LexicographicSortOfflineColumnOrderPermute>());
    // Online best order
    algorithms.push_back(std::make_unique<LexicographicSortOnlineColumnOrderPermute>());
    // Multiple lists
    algorithms.push_back(std::make_unique<MultipleListsPermute>());

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

template void TestAllManyIntegersColumnsTests<int8_t>(Path dir);
template void TestAllManyIntegersColumnsTests<int64_t>(Path dir);

void TestViaStrings(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    algorithms.push_back(std::make_unique<IdentityPermute>());
    // Shuffle with big budget
    algorithms.push_back(std::make_unique<ShufflePermute>(100ms));
    // LocalOptimizations with big budget
    algorithms.push_back(std::make_unique<LocalOptimizationsPermute>(100ms));
    // Just sort
    algorithms.push_back(std::make_unique<LexicographicSortPermute>());
    // Offline best order
    algorithms.push_back(std::make_unique<LexicographicSortOfflineColumnOrderPermute>());
    // Online best order
    algorithms.push_back(std::make_unique<LexicographicSortOnlineColumnOrderPermute>());
    // Suffix array based greedy
    algorithms.push_back(std::make_unique<SuffixArrayGreedyPermute>());
    // Multiple lists
    algorithms.push_back(std::make_unique<MultipleListsPermute>());

    ColumnParser parser(GenericColumnParser<ColumnStrings>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

}  // namespace CompressionSorts
