#pragma once

#include <functional>
#include <string>

#include "batches.hpp"
#include "compression_sorts/filesystem.hpp"

namespace CompressionSorts {

using RawGenerator = std::function<std::string(size_t)>;

RawGenerator GetIntegersRawGenerator(int64_t /*min*/, int64_t /*max*/, size_t /*columns*/);

void GenerateTest(CompressionSorts::Path /*dir*/, size_t /*test_size*/,
                  RawGenerator /*raw_generator*/);

void GenerateTests(CompressionSorts::Path /*dir*/, BatchesSettings /*batches_settings*/,
                   RawGenerator /*raw_generator*/);

void GenerateSplitBatches(CompressionSorts::Path /*dir*/, BatchesSettings /*batches_settings*/,
                          CompressionSorts::Path /*path_in*/, size_t /*cnt_columns*/);

void GenerateRandomPrefixBatches(CompressionSorts::Path /*dir*/,
                                 BatchesSettings /*batches_settings*/,
                                 CompressionSorts::Path /*path_in*/);

}  // namespace CompressionSorts
