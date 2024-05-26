#pragma once

#include <filesystem>
#include <vector>

namespace CompressionSorts {

using Path = std::filesystem::path;

void InitializeDirectory(CompressionSorts::Path /*dir*/);

void SaveTest(CompressionSorts::Path /*path*/, const std::vector<std::string>& /*test*/);

std::vector<Path> GetAllFiles(Path /*dir*/);

}  // namespace CompressionSorts
