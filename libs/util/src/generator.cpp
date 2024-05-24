#include "compression_sorts/generator.hpp"

#include <fstream>
#include <iostream>
#include <random>

#include "compression_sorts/filesystem.hpp"
#include "compression_sorts/read_data.hpp"
#include "compression_sorts/split.hpp"

namespace CompressionSorts {

std::mt19937_64 rnd(179);

std::vector<size_t> GenerateBatches(BatchesSettings batches_settings) {
    std::vector<size_t> batches;
    for (size_t new_batch_size = 1; new_batch_size <= batches_settings.max_batch_size;
         new_batch_size =
             new_batch_size < batches_settings.max_batch_size
                 ? std::min<size_t>(batches_settings.max_batch_size,
                                    ceil(batches_settings.exponent * new_batch_size))
                 : static_cast<size_t>(ceil(batches_settings.exponent * new_batch_size))) {
        batches.push_back(new_batch_size);
    }
    return batches;
}

using RawGenerator = std::function<std::string(size_t)>;

void GenerateTest(CompressionSorts::Path dir, size_t test_size, RawGenerator raw_generator) {
    std::cerr << "GenerateTest -" << " dir: " << dir << " test_size: " << test_size << std::endl;
    const auto path = dir / (std::to_string(test_size) + ".csv");
    std::vector<std::string> test(test_size);
    for (size_t raw = 0; raw < test_size; ++raw) {
        test[raw] = raw_generator(raw);
    }
    std::shuffle(test.begin(), test.end(), rnd);
    SaveTest(path, test);
}

void GenerateTests(CompressionSorts::Path dir, BatchesSettings batches_settings,
                   RawGenerator raw_generator) {
    InitializeDirectory(dir);
    const auto batches = GenerateBatches(batches_settings);
    for (size_t test_size : batches) {
        GenerateTest(dir, test_size, raw_generator);
    }
}

void GenerateSplitBatches(CompressionSorts::Path dir, BatchesSettings batches_settings,
                          CompressionSorts::Path path_in, size_t cnt_columns) {
    std::ifstream in(path_in);
    in.tie(0);

    auto raw_generator = [&in, cnt_columns](size_t /*raw*/) -> std::string {
        std::string buffer;
        while (getline(in, buffer)) {
            if (CompressionSorts::SplitBySymbol(buffer, ',').size() != cnt_columns) {
                std::cerr << "WARNING! Raw size = "
                          << CompressionSorts::SplitBySymbol(buffer, ',').size()
                          << " and != " << cnt_columns << std::endl;
                continue;
            }
            return buffer;
        }
        throw std::runtime_error("GenerateSplitBatches - Too small tests file");
    };

    GenerateTests(dir, batches_settings, raw_generator);
}

void GenerateRandomPrefixBatches(CompressionSorts::Path dir, BatchesSettings batches_settings,
                                 CompressionSorts::Path path_in) {
    auto data = CompressionSorts::ReadLines(path_in);
    std::shuffle(data.begin(), data.end(), rnd);
    auto raw_generator = [&data](size_t raw) -> std::string {
        if (raw >= data.size()) {
            throw std::runtime_error("GenerateRandomPrefixBatches - Too small tests file");
        }
        return data[raw];
    };

    GenerateTests(dir, batches_settings, raw_generator);
}

}  // namespace CompressionSorts
