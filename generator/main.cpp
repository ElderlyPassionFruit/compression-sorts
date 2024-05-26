#include "compression_sorts/generators.hpp"

int main() {
    using namespace CompressionSorts;

    // random small integers
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 1'000'000, .exponent = 1.2};
        RawGenerator raw_generator = GetIntegersRawGenerator(-100, 100, 1);
        GenerateTests("tests_data/int/random_small", batches_settings, raw_generator);
    }

    // random small integers 20 columns
    {
        constexpr size_t kCntSmallIntegerColumns = 20;
        constexpr size_t kMaxBatchSize = 1'000'000 / kCntSmallIntegerColumns;
        constexpr BatchesSettings batches_settings{.max_batch_size = kMaxBatchSize,
                                                   .exponent = 1.2};
        RawGenerator raw_generator = GetIntegersRawGenerator(-100, 100, kCntSmallIntegerColumns);
        GenerateTests("tests_data/int/many_random_small", batches_settings, raw_generator);
    }

    // random big integers
    {
        constexpr BatchesSettings batches_settings{.max_batch_size = 1'000'000, .exponent = 1.2};
        RawGenerator raw_generator = GetIntegersRawGenerator(
            std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max(), 1);
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
