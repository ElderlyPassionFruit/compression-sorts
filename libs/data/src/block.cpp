#include "compression_sorts/block.hpp"

#include <cassert>

namespace CompressionSorts {

namespace {

void VerifyBlockData(const Block::Container& data) {
    assert(data.size() > 0);
    size_t size = -1;
    for (const auto& column : data) {
        if (size == -1) {
            size = column->GetSize();
        }
        assert(size == column->GetSize());
    }
}

}  // namespace

Block::Block(Container data) : data_(std::move(data)), size_(data_[0]->GetSize()) {
    VerifyBlockData(data_);
}

size_t Block::GetSize() const {
    return size_;
}

size_t Block::GetSerializedSize() const {
    size_t total_serialized_size = 0;
    for (const auto& column : data_) {
        total_serialized_size += column->GetSerializedSize();
    }
    return total_serialized_size;
}

size_t Block::CalculateCompressionSize() const {
    size_t total_compression_size = 0;
    for (const auto& column : data_) {
        total_compression_size += column->CalculateCompressionSize();
    }
    return total_compression_size;
}

void Block::SwapRaws(size_t i, size_t j) {
    for (const auto& column : data_) {
        column->SwapRaws(i, j);
    }
}

void Block::ApplyPermutation(const std::vector<size_t>& order) {
    for (const auto& column : data_) {
        column->ApplyPermutation(order);
    }
}

Block::Container& Block::GetData() {
    return data_;
}

}  // namespace CompressionSorts
