#include "compression_sorts/generators.hpp"

#include <fstream>
#include <iostream>

#include "compression_sorts/filesystem.hpp"
#include "compression_sorts/random.hpp"
#include "compression_sorts/read_data.hpp"
#include "compression_sorts/split.hpp"

namespace CompressionSorts {

RawGenerator GetIntegersRawGenerator(int64_t min, int64_t max, size_t columns) {
    std::uniform_int_distribution<int64_t> distribution(min, max);
    return [distribution, columns](size_t /*row*/) mutable -> std::string {
        std::stringstream buffer;
        for (size_t i = 0; i < columns; ++i) {
            if (i > 0) {
                buffer << ',';
            }
            buffer << distribution(GetTwister());
        }
        return buffer.str();
    };
}

void GenerateTest(CompressionSorts::Path dir, size_t test_size, RawGenerator raw_generator) {
    std::cerr << "GenerateTest -" << " dir: " << dir << " test_size: " << test_size << std::endl;
    const auto path = dir / (std::to_string(test_size) + ".csv");
    std::vector<std::string> test(test_size);
    for (size_t raw = 0; raw < test_size; ++raw) {
        test[raw] = raw_generator(raw);
    }
    std::shuffle(test.begin(), test.end(), GetTwister());
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
    std::shuffle(data.begin(), data.end(), GetTwister());
    auto raw_generator = [&data](size_t raw) -> std::string {
        if (raw >= data.size()) {
            throw std::runtime_error("GenerateRandomPrefixBatches - Too small tests file");
        }
        return data[raw];
    };

    GenerateTests(dir, batches_settings, raw_generator);
}

}  // namespace CompressionSorts
