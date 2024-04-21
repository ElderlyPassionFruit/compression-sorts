#pragma once

#include <fstream>
#include <optional>
#include <vector>

#include "compression_sorts/path.hpp"

namespace CompressionSorts {

template <typename T>
std::optional<T> ReadNext(std::ifstream& in) {
    T value;
    if (in >> value) {
        return value;
    }
    return std::nullopt;
}

template <typename T>
std::vector<T> ReadData(Path path) {
    std::ifstream in(path);
    std::vector<T> data;
    while (true) {
        if (auto value = ReadNext<T>(in); value.has_value()) {
            data.push_back(*value);
        } else {
            break;
        }
    }
    return data;
}

}  // namespace CompressionSorts
