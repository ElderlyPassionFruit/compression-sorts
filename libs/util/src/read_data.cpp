#include "compression_sorts/read_data.hpp"

namespace CompressionSorts {

std::vector<std::string> ReadLines(Path path) {
    std::ifstream in(path);
    in.tie(0);
    std::vector<std::string> data;
    std::string buffer;
    while (getline(in, buffer)) {
        data.push_back(std::move(buffer));
    }
    return data;
}

}  // namespace CompressionSorts
