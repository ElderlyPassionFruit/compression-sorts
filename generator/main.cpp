#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "compression_sorts/path.hpp"

std::vector<size_t> GenBatches(size_t max_batch_size, double exponent) {
    std::vector<size_t> batches;
    for (size_t new_batch_size = 1; new_batch_size <= max_batch_size;
         new_batch_size = ceil(exponent * new_batch_size)) {
        batches.push_back(new_batch_size);
    }
    return batches;
}

void InitDirectory(CompressionSorts::Path dir) {
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);
}

using RawGenerator = std::function<std::string()>;

void GenerateTest(CompressionSorts::Path dir, size_t test_size, RawGenerator raw_generator) {
    const auto path = dir / (std::to_string(test_size) + ".csv");
    std::cout << "GenerateTest -" << " dir: " << dir << " test_size: " << test_size << std::endl;
    std::ofstream out(path);
    out.tie(0);
    for (size_t raw = 0; raw < test_size; ++raw) {
        out << raw_generator() << '\n';
    }
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

    auto raw_generator = [&in]() -> std::string {
        std::string buffer;
        if (!getline(in, buffer)) {
            throw std::runtime_error("GenerateSplitBatches - Too small tests file");
        }
        return buffer;
    };

    GenerateTests(dir, batches, raw_generator);
}

int main() {
    std::mt19937_64 rnd(179);

    // hits
    {
        const auto batches = GenBatches(1'000'000, 1.2);
        GenerateSplitBatches("tests_data/clickhouse/hits", batches, "generator/downloads/hits.csv");
    }

    // random small integers
    {
        const auto batches = GenBatches(1'000'000, 1.2);
        std::uniform_int_distribution<> distribution(-100, 100);
        auto raw_generator = [&rnd, &distribution]() -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_small", batches, raw_generator);
    }

    // random big integers
    {
        const auto batches = GenBatches(1'000'000, 1.2);
        std::uniform_int_distribution<> distribution(-1'000'000'000, 1'000'000'000);
        auto raw_generator = [&rnd, &distribution]() -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_big", batches, raw_generator);
    }
}
