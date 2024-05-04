#include "compression_sorts/range.hpp"

#include <tuple>

namespace CompressionSorts {

bool Range::operator==(const Range& other) const {
    return std::tie(from, to) == std::tie(other.from, other.to);
}

}  // namespace CompressionSorts
