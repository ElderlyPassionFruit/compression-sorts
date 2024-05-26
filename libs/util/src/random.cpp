#include "compression_sorts/random.hpp"

#include <random>

namespace CompressionSorts {

std::mt19937_64& GetTwister() {
    static std::mt19937_64 rnd(179);
    return rnd;
}

}  // namespace CompressionSorts
