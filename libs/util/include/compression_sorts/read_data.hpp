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

}  // namespace CompressionSorts
