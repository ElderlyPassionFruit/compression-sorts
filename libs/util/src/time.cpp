#include "compression_sorts/time.hpp"

#include <chrono>

namespace CompressionSorts {

namespace {

auto Now() {
    return std::chrono::high_resolution_clock::now();
}

}  // namespace

Time MesuareTime(std::function<void()> routine) {
    auto start_time = Now();
    routine();
    auto finish_time = Now();
    return std::chrono::duration_cast<Time>(finish_time - start_time);
}

}  // namespace CompressionSorts