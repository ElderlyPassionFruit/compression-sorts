#pragma once

#include <cstring>
#include <stdexcept>
#include <vector>

namespace CompressionSorts {

template <typename T>
std::vector<char> SerializeData(const std::vector<T>& /*data*/) {
    throw std::invalid_argument("Not implemented");
}

template <std::integral T>
std::vector<char> SerializeData(const std::vector<T>& data) {
    std::vector<char> serialized_data(data.size() * sizeof(T));
    memcpy(serialized_data.data(), data.data(), serialized_data.size());
    return serialized_data;
}

}  // namespace CompressionSorts
