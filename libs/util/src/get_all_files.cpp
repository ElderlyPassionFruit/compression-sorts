#include "compression_sorts/get_all_files.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace CompressionSorts {

namespace {

std::tuple<std::string, size_t, std::string> SplitWithPath(const Path& path) {
    auto string_path = path.string();
    auto isdigit = [](unsigned char c) { return std::isdigit(c); };
    auto first = std::find_if(string_path.begin(), string_path.end(), isdigit);
    auto last = std::find_if_not(first, string_path.end(), isdigit);
    std::string prefix{string_path.begin(), first};
    size_t value = std::stoi(std::string{first, last});
    std::string suffix{last, string_path.end()};
    return std::make_tuple(prefix, value, suffix);
}

}  // namespace

std::vector<Path> GetAllFiles(Path dir) {
    std::vector<Path> paths;
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir}) {
        paths.push_back(dir_entry.path());
    }
    std::sort(paths.begin(), paths.end(), [](const Path& lhs, const Path& rhs) {
        return SplitWithPath(lhs) < SplitWithPath(rhs);
    });
    return paths;
}

}  // namespace CompressionSorts