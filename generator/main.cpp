#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "compression_sorts/path.hpp"
#include "compression_sorts/read_data.hpp"

std::mt19937_64 rnd(179);

std::vector<size_t> GenBatches(size_t max_batch_size, double exponent) {
    std::vector<size_t> batches;
    for (size_t new_batch_size = 1; new_batch_size <= max_batch_size;
         new_batch_size = new_batch_size < max_batch_size
                              ? std::min<size_t>(max_batch_size, ceil(exponent * new_batch_size))
                              : static_cast<size_t>(ceil(exponent * new_batch_size))) {
        batches.push_back(new_batch_size);
    }
    return batches;
}

void InitDirectory(CompressionSorts::Path dir) {
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);
}

using RawGenerator = std::function<std::string(size_t)>;

void SaveTest(CompressionSorts::Path path, const std::vector<std::string>& test) {
    std::ofstream out(path);
    out.tie(0);
    for (const auto& raw : test) {
        out << raw << "\n";
    }
}

void GenerateTest(CompressionSorts::Path dir, size_t test_size, RawGenerator raw_generator) {
    std::cout << "GenerateTest -" << " dir: " << dir << " test_size: " << test_size << std::endl;
    const auto path = dir / (std::to_string(test_size) + ".csv");
    std::vector<std::string> test(test_size);
    for (size_t raw = 0; raw < test_size; ++raw) {
        test[raw] = raw_generator(raw);
    }
    std::shuffle(test.begin(), test.end(), rnd);
    SaveTest(path, test);
}

void GenerateTests(CompressionSorts::Path dir, const std::vector<size_t>& tests_sizes,
                   RawGenerator raw_generator) {
    InitDirectory(dir);
    for (size_t test_size : tests_sizes) {
        GenerateTest(dir, test_size, raw_generator);
    }
}

void GenerateSplitBatches(CompressionSorts::Path dir, const std::vector<size_t>& batches,
                          CompressionSorts::Path path_in) {
    std::ifstream in(path_in);
    in.tie(0);

    auto raw_generator = [&in](size_t /*raw*/) -> std::string {
        std::string buffer;
        if (!getline(in, buffer)) {
            throw std::runtime_error("GenerateSplitBatches - Too small tests file");
        }
        return buffer;
    };

    GenerateTests(dir, batches, raw_generator);
}

void GenerateRandomPrefixBatches(CompressionSorts::Path dir, const std::vector<size_t>& batches,
                                 CompressionSorts::Path path_in) {
    auto data = CompressionSorts::ReadLines(path_in);
    std::shuffle(data.begin(), data.end(), rnd);
    auto raw_generator = [&data](size_t raw) -> std::string {
        if (raw >= data.size()) {
            throw std::runtime_error("GenerateRandomPrefixBatches - Too small tests file");
        }
        return data[raw];
    };

    GenerateTests(dir, batches, raw_generator);
}

int main() {

    // random small integers
    {
        constexpr size_t kMaxBatchSize = 1000000;
        const auto batches = GenBatches(kMaxBatchSize, 1.2);
        std::uniform_int_distribution<> distribution(-100, 100);
        auto raw_generator = [&distribution](size_t /*raw*/) -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_small", batches, raw_generator);
    }

    // random big integers
    {
        constexpr size_t kMaxBatchSize = 1000000;
        const auto batches = GenBatches(kMaxBatchSize, 1.2);
        std::uniform_int_distribution<int64_t> distribution(std::numeric_limits<int64_t>::min(),
                                                            std::numeric_limits<int64_t>::max());
        auto raw_generator = [&distribution](size_t /*raw*/) -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_big", batches, raw_generator);
    }

    // hits
    {
        constexpr size_t kMaxBatchSize = 100000;
        const auto batches = GenBatches(kMaxBatchSize, 1.2);
        GenerateSplitBatches("tests_data/clickhouse/hits", batches, "generator/downloads/hits.csv");
    }

    // dictionary
    {
        constexpr size_t kMaxBatchSize = 54555;
        const auto batches = GenBatches(kMaxBatchSize, 1.2);
        GenerateRandomPrefixBatches("tests_data/english/dictionary", batches,
                                    "generator/downloads/dictionary.csv");
    }
}
