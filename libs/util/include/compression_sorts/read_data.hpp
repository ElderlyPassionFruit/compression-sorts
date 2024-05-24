#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "compression_sorts/filesystem.hpp"

namespace CompressionSorts {

std::vector<std::string> ReadLines(Path path);

template <typename T>
T FromString(std::string s) {
    if constexpr (std::is_same_v<T, std::string>) {
        return s;
    } else if (std::is_integral_v<T>) {
        if (std::is_signed_v<T>) {
            return std::stoll(s);
        } else {
            return std::stoull(s);
        }
    } else {
        throw std::runtime_error("Not implemented");
    }
}

template <typename T>
std::vector<T> ReadData(Path path) {
    auto lines = ReadLines(path);
    std::vector<T> data(lines.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        data[i] = FromString<T>(std::move(lines[i]));
    }
    return data;
}

}  // namespace CompressionSorts
