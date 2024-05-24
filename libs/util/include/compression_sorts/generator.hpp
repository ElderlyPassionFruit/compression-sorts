#pragma once

#include <functional>

#include "compression_sorts/filesystem.hpp"

namespace CompressionSorts {

struct BatchesSettings {
    size_t max_batch_size;
    double exponent;
};

std::vector<size_t> GenerateBatches(BatchesSettings /*settings*/);

using RawGenerator = std::function<std::string(size_t)>;

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
