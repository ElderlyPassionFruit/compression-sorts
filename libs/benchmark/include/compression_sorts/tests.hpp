#pragma once

#include <concepts>

#include "compression_sorts/filesystem.hpp"

namespace CompressionSorts {

template <std::integral T>
void TestAllSingleIntegersColumnTests(Path /*dir*/);

template <std::integral T>
void TestAllManyIntegersColumnsTests(Path /*dir*/);

void TestViaStrings(Path /*dir*/);

}  // namespace CompressionSorts
