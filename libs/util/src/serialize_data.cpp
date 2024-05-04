
#include "compression_sorts/serialize_data.hpp"

namespace CompressionSorts {

template <>
std::vector<char> SerializeData(const std::vector<std::string>& data) {
    std::vector<char> serialized_data;
    size_t total_size = 0;
    for (const auto& string : data) {
        total_size += string.size();
    }
    serialized_data.resize(total_size);
    size_t pos = 0;
    for (const auto& string : data) {
        auto result_begin = std::next(serialized_data.begin(), pos);
        std::copy(string.begin(), string.end(), result_begin);
        pos += string.size();
    }
    return serialized_data;
}

}  // namespace CompressionSorts
