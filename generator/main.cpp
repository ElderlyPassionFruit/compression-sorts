#include <random>

#include "compression_sorts/generator.hpp"

std::mt19937_64 rnd(179);

int main() {
    using CompressionSorts::BatchesSettings;

    // random small integers
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 1'000'000, .exponent = 1.2};
        std::uniform_int_distribution<> distribution(-100, 100);
        auto raw_generator = [&distribution](size_t /*raw*/) -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_small", batches_settings, raw_generator);
    }

    // random integer column tables
    {
        constexpr size_t kCntSmallIntegerColumns = 20;
        constexpr size_t kMaxBatchSize = 1'000'000 / kCntSmallIntegerColumns;

        constexpr BatchesSettings batches_settings{.max_batch_size = kMaxBatchSize,
                                                   .exponent = 1.2};

        std::uniform_int_distribution<> distribution(-100, 100);
        auto raw_generator = [&distribution](size_t /*raw*/) -> std::string {
            std::string buffer;
            for (size_t i = 0; i < kCntSmallIntegerColumns; ++i) {
                if (i > 0) {
                    buffer += ',';
                }
                buffer += std::to_string(distribution(rnd));
            }
            return buffer;
        };
        GenerateTests("tests_data/int/many_random_small", batches_settings, raw_generator);
    }

    // random big integers
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 1'000'000, .exponent = 1.2};
        std::uniform_int_distribution<int64_t> distribution(std::numeric_limits<int64_t>::min(),
                                                            std::numeric_limits<int64_t>::max());
        auto raw_generator = [&distribution](size_t /*raw*/) -> std::string {
            return std::to_string(distribution(rnd));
        };
        GenerateTests("tests_data/int/random_big", batches_settings, raw_generator);
    }

    // hits
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 100'000, .exponent = 1.2};
        GenerateSplitBatches("tests_data/clickhouse/hits", batches_settings,
                             "generator/downloads/hits.csv", 105);
    }

    // dictionary
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 54'555, .exponent = 1.2};
        GenerateRandomPrefixBatches("tests_data/english/dictionary", batches_settings,
                                    "generator/downloads/dictionary.csv");
    }

    // price_paid_transaction_data
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 100'000, .exponent = 1.2};
        GenerateSplitBatches("tests_data/clickhouse/price_paid_transaction_data", batches_settings,
                             "generator/downloads/price_paid_transaction_data.csv", 16);
    }

    // "What's on the Menu?"
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 1000, .exponent = 1.2};
        GenerateSplitBatches("tests_data/clickhouse/dish", batches_settings,
                             "generator/downloads/Dish.csv", 9);
        GenerateSplitBatches("tests_data/clickhouse/menu", batches_settings,
                             "generator/downloads/Menu.csv", 20);
        GenerateSplitBatches("tests_data/clickhouse/menu_item", batches_settings,
                             "generator/downloads/MenuItem.csv", 9);
        GenerateSplitBatches("tests_data/clickhouse/menu_page", batches_settings,
                             "generator/downloads/MenuPage.csv", 7);
    }

    return EXIT_SUCCESS;
}
