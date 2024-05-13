#include <cstdlib>
#include <iostream>

#include "compression_sorts/benchmark.hpp"
#include "compression_sorts/block.hpp"
#include "compression_sorts/column_integers.hpp"
#include "compression_sorts/column_strings.hpp"
#include "compression_sorts/get_all_files.hpp"
#include "compression_sorts/identity.hpp"
#include "compression_sorts/lexicographic_sort.hpp"
#include "compression_sorts/local_optimizations.hpp"
#include "compression_sorts/read_data.hpp"
#include "compression_sorts/shuffle.hpp"
#include "compression_sorts/statistics_saver.hpp"
#include "compression_sorts/suffix_array_greedy.hpp"

using namespace CompressionSorts;

namespace {

struct TestWithName {
    Block block;
    std::string name;
};

using Tests = std::vector<TestWithName>;

template <typename T>
Block ReadSingleColumnBlock(Path path) {
    auto data = ReadData<T>(path);

    if constexpr (std::is_integral_v<T>) {
        ColumnPtr column(std::make_unique<ColumnIntegers<T>>(std::move(data)));
        Block::Container columns;
        columns.push_back(std::move(column));
        Block block(std::move(columns));
        return block;
    } else if constexpr (std::is_same_v<std::string, T>) {
        ColumnPtr column(std::make_unique<ColumnStrings>(std::move(data)));
        Block::Container columns;
        columns.push_back(std::move(column));
        Block block(std::move(columns));
        return block;
    } else {
        throw std::runtime_error("Not implemented");
    }
}

template <typename T>
Tests GetAllSingleColumnTests(Path dir) {
    auto paths = GetAllFiles(dir);
    Tests tests;
    for (const auto& path : paths) {
        Block block(ReadSingleColumnBlock<T>(path));
        tests.push_back({
            .block = std::move(block),
            .name = path,
        });
    }
    return tests;
}

std::vector<std::string> SplitBySymbol(std::string s, char delimiter) {
    std::vector<std::string> result;
    std::string current;
    size_t cnt_bad = 0;
    for (const char c : s) {
        if (c == '\"') {
            ++cnt_bad;
        }
        if (c == delimiter && cnt_bad % 2 == 0) {
            result.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    result.push_back(current);
    return result;
}

std::vector<std::vector<std::string>> SplitAllStrings(std::vector<std::string> data,
                                                      char delimiter) {
    std::vector<std::vector<std::string>> result;
    result.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        auto current = SplitBySymbol(std::move(data[i]), delimiter);
        if (!result.empty() && (current.size() != result.back().size())) {
            std::cerr << "WARNING: Incorrect sizes " << current.size() << " "
                      << result.back().size() << std::endl;
            continue;
        }
        result.push_back(std::move(current));
    }
    return result;
}

std::vector<std::vector<std::string>> Transpose(std::vector<std::vector<std::string>> data) {
    size_t n = data.size();
    size_t m = data[0].size();
    std::vector<std::vector<std::string>> result(m, std::vector<std::string>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result[j][i] = data[i][j];
        }
    }
    return result;
}

Tests GetAllFullAsStringsTests(Path dir) {
    auto paths = GetAllFiles(dir);
    Tests tests;
    for (const auto& path : paths) {
        auto data = ReadData<std::string>(path);
        auto columns_data = Transpose(SplitAllStrings(data, ','));
        size_t cnt_columns = columns_data.size();
        Block::Container columns(cnt_columns);
        for (size_t i = 0; i < cnt_columns; ++i) {
            columns[i] = std::make_unique<ColumnStrings>(std::move(columns_data[i]));
        }
        Block block(std::move(columns));
        tests.push_back({
            .block = std::move(block),
            .name = path,
        });
    }
    return tests;
}

template <std::integral T>
std::vector<T> StringsToIntegers(const std::vector<std::string>& data) {
    std::vector<T> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = FromString<T>(data[i]);
    }
    return result;
}

template <std::integral T>
Tests GetAllFullAsIntegerTests(Path dir) {
    auto paths = GetAllFiles(dir);
    Tests tests;
    for (const auto& path : paths) {
        auto data = ReadData<std::string>(path);
        auto columns_data_strings = Transpose(SplitAllStrings(data, ','));
        std::vector<std::vector<T>> columns_data;
        for (const auto& column_string : columns_data_strings) {
            columns_data.push_back(StringsToIntegers<T>(column_string));
        }
        size_t cnt_columns = columns_data.size();
        Block::Container columns(cnt_columns);
        for (size_t i = 0; i < cnt_columns; ++i) {
            columns[i] = std::make_unique<ColumnIntegers<T>>(std::move(columns_data[i]));
        }
        Block block(std::move(columns));
        tests.push_back({
            .block = std::move(block),
            .name = path,
        });
    }
    return tests;
}

void TestAllBenchmarksWithAlgorithm(Tests tests, const IPermute& algorithm,
                                    const size_t iterations) {
    for (auto& [block, name] : tests) {
        std::cerr << "name = " << name << " algorithm = " << algorithm.GetName() << std::endl;
        auto benchmark_results = TestAlgorithm(name, std::move(block), algorithm, iterations);
        SaveBenchmarkResults(benchmark_results);
    }
}

template <std::integral T>
void TestAllSingleIntegersColumnTests(Path dir) {
    // Just nothing
    {
        auto tests = GetAllSingleColumnTests<T>(dir);
        CompressionSorts::IdentityPermute identity;
        TestAllBenchmarksWithAlgorithm(std::move(tests), identity, 1);
    }
    // Shuffle with big budget
    {
        auto tests = GetAllSingleColumnTests<T>(dir);
        CompressionSorts::ShufflePermute shuffle(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), shuffle, 1);
    }
    // LocalOptimizations with big budget
    {
        auto tests = GetAllSingleColumnTests<T>(dir);
        CompressionSorts::LocalOptimizationsPermute local_optimizations(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), local_optimizations, 1);
    }
    // Just sort
    {
        auto tests = GetAllSingleColumnTests<T>(dir);
        CompressionSorts::LexicographicSortPermute sort;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort, 1);
    }
}

template <std::integral T>
void TestAllMannyIntegersColumnsTests(Path dir) {
    // Just nothing
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::IdentityPermute identity;
        TestAllBenchmarksWithAlgorithm(std::move(tests), identity, 1);
    }
    // Shuffle with big budget
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::ShufflePermute shuffle(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), shuffle, 1);
    }
    // LocalOptimizations with big budget
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::LocalOptimizationsPermute local_optimizations(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), local_optimizations, 1);
    }
    // Just sort
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::LexicographicSortPermute sort;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort, 1);
    }
    // Offline best order
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::LexicographicSortOfflineColumnOrderPermute sort_offline_order;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort_offline_order, 1);
    }
    // Online best order
    {
        auto tests = GetAllFullAsIntegerTests<T>(dir);
        CompressionSorts::LexicographicSortOnlineColumnOrderPermute sort_online_order;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort_online_order, 1);
    }
}

void TestViaStrings(Path dir) {
    // Just nothing
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::IdentityPermute identity;
        TestAllBenchmarksWithAlgorithm(std::move(tests), identity, 1);
    }
    // Shuffle with big budget
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::ShufflePermute shuffle(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), shuffle, 1);
    }
    // LocalOptimizations with big budget
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::LocalOptimizationsPermute local_optimizations(100ms);
        TestAllBenchmarksWithAlgorithm(std::move(tests), local_optimizations, 1);
    }
    // Just sort
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::LexicographicSortPermute sort;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort, 1);
    }
    // Offline best order
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::LexicographicSortOfflineColumnOrderPermute sort_offline_order;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort_offline_order, 1);
    }
    // Online best order
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::LexicographicSortOnlineColumnOrderPermute sort_online_order;
        TestAllBenchmarksWithAlgorithm(std::move(tests), sort_online_order, 1);
    }
    // Suffix array based greedy
    {
        auto tests = GetAllFullAsStringsTests(dir);
        CompressionSorts::SuffixArrayGreedyPermute suffix_array_greedy;
        TestAllBenchmarksWithAlgorithm(std::move(tests), suffix_array_greedy, 1);
    }
}

}  // namespace

int main() {
    TestAllSingleIntegersColumnTests<int8_t>("tests_data/int/random_small");
    TestAllMannyIntegersColumnsTests<int8_t>("tests_data/int/many_random_small");
    TestAllSingleIntegersColumnTests<int64_t>("tests_data/int/random_big");
    TestViaStrings("tests_data/english/dictionary");
    // TestViaStrings("tests_data/clickhouse/hits");
    TestViaStrings("tests_data/clickhouse/dish");
    TestViaStrings("tests_data/clickhouse/menu");
    TestViaStrings("tests_data/clickhouse/menu_item");
    TestViaStrings("tests_data/clickhouse/menu_page");
    TestViaStrings("tests_data/clickhouse/price_paid_transaction_data");
    return EXIT_SUCCESS;
}
