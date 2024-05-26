#include <cstdlib>

#include "compression_sorts/tests.hpp"

int main() {
    using namespace CompressionSorts;

    TestAllSingleIntegersColumnTests<int8_t>("tests_data/int/random_small");
    TestAllManyIntegersColumnsTests<int8_t>("tests_data/int/many_random_small");
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
