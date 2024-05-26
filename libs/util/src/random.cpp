#include "compression_sorts/random.hpp"

#include <random>

namespace CompressionSorts {

std::mt19937_64& GetTwister() {
    static std::mt19937_64 rnd(179);
    return rnd;
}

RawGenerator GetIntegersGenerator(int64_t min, int64_t max, size_t columns) {
    std::uniform_int_distribution<int64_t> distribution(min, max);
    return [distribution, columns](size_t /*row*/) mutable -> std::string {
        std::stringstream buffer;
        for (size_t i = 0; i < columns; ++i) {
            if (i > 0) {
                buffer << ',';
            }
            buffer << distribution(GetTwister());
        }
        return buffer.str();
    };
}

}  // namespace CompressionSorts
