#pragma once

#include <chrono>
#include <functional>

namespace CompressionSorts {

using Time = std::chrono::nanoseconds;

Time MesuareTime(std::function<void()> routine);

}  // namespace CompressionSorts