#include "compression_sorts/split.hpp"

#include <stdexcept>
#include <string>

namespace CompressionSorts {

std::vector<std::string> SplitBySymbol(std::string s, char delimiter) {
    std::vector<std::string> result;
    std::string current;
    size_t cnt_bad = 0;
    for (const char c : s) {
        if (c == '\"') {
            ++cnt_bad;
        }
        if (c == delimiter && cnt_bad % 2 == 0) {
            result.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    result.push_back(current);
    return result;
}

std::vector<std::vector<std::string>> SplitAllStrings(std::vector<std::string> data,
                                                      char delimiter) {
    std::vector<std::vector<std::string>> result;
    result.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        auto current = SplitBySymbol(std::move(data[i]), delimiter);
        if (!result.empty() && (current.size() != result.back().size())) {
            throw std::runtime_error(
                "Incorrect sizes. current = " + std::to_string(current.size()) +
                " correct = " + std::to_string(result.back().size()));
        }
        result.push_back(std::move(current));
    }
    return result;
}

}  // namespace CompressionSorts
