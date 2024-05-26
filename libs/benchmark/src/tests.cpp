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

#define REGISTER_ALGORITHM(Type, ...) \
    (algorithms).push_back(std::make_unique<const Type>(__VA_ARGS__));

template <std::integral T>
void TestAllSingleIntegersColumnTests(Path dir) {
    std::vector<IPermutePtr> algorithms;
    // Just nothing
    REGISTER_ALGORITHM(IdentityPermute);
    // Shuffle with big budget
    REGISTER_ALGORITHM(ShufflePermute, 100ms);
    // LocalOptimizations with big budget
    REGISTER_ALGORITHM(LocalOptimizationsPermute, 100ms);
    // Just sort
    REGISTER_ALGORITHM(LexicographicSortPermute);

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

template void TestAllSingleIntegersColumnTests<int8_t>(Path dir);
template void TestAllSingleIntegersColumnTests<int64_t>(Path dir);

template <std::integral T>
void TestAllManyIntegersColumnsTests(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    REGISTER_ALGORITHM(IdentityPermute);
    // Shuffle with big budget
    REGISTER_ALGORITHM(ShufflePermute, 100ms);
    // LocalOptimizations with big budget
    REGISTER_ALGORITHM(LocalOptimizationsPermute, 100ms);
    // Just sort
    REGISTER_ALGORITHM(LexicographicSortPermute);
    // Offline best order
    REGISTER_ALGORITHM(LexicographicSortOfflineColumnOrderPermute);
    // Online best order
    REGISTER_ALGORITHM(LexicographicSortOnlineColumnOrderPermute);
    // Multiple lists
    REGISTER_ALGORITHM(MultipleListsPermute);

    ColumnParser parser(GenericColumnParser<ColumnIntegers<T>>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

template void TestAllManyIntegersColumnsTests<int8_t>(Path dir);
template void TestAllManyIntegersColumnsTests<int64_t>(Path dir);

void TestViaStrings(Path dir) {
    std::vector<IPermutePtr> algorithms;

    // Just nothing
    REGISTER_ALGORITHM(IdentityPermute);
    // Shuffle with big budget
    REGISTER_ALGORITHM(ShufflePermute, 100ms);
    // LocalOptimizations with big budget
    REGISTER_ALGORITHM(LocalOptimizationsPermute, 100ms);
    // Just sort
    REGISTER_ALGORITHM(LexicographicSortPermute);
    // Offline best order
    REGISTER_ALGORITHM(LexicographicSortOfflineColumnOrderPermute);
    // Online best order
    REGISTER_ALGORITHM(LexicographicSortOnlineColumnOrderPermute);
    // Suffix array based greedy
    REGISTER_ALGORITHM(SuffixArrayGreedyPermute);
    // Multiple lists
    REGISTER_ALGORITHM(MultipleListsPermute);

    ColumnParser parser(GenericColumnParser<ColumnStrings>);
    TestAllBenchmarksWithAlgorithms(dir, parser, algorithms, 1);
}

}  // namespace CompressionSorts
