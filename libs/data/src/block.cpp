#include "compression_sorts/block.hpp"

#include <cassert>

#include "compression_sorts/column_interface.hpp"
#include "compression_sorts/online_compression_calculator_interface.hpp"
#include "compression_sorts/split.hpp"

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

class BlockOnlineCompressionCalculator : public IOnlineCompressionCalculator {
public:
    using Container = std::vector<OnlineCompressionCalculatorPtr>;

    explicit BlockOnlineCompressionCalculator(const Block::Container& data)
        : size_(data[0]->GetSize()), calculators_(CreateContainer(data)) {
        assert(!data.empty());
    }

    size_t GetCurrentCompressedSize() override {
        size_t total_compressed_size = 0;
        for (const auto& calculator : calculators_) {
            total_compressed_size += calculator->GetCurrentCompressedSize();
        }
        return total_compressed_size;
    }

    void ApplyPermutation(const std::vector<size_t>& order) override {
        assert(order.size() == size_);
        for (auto& calculator : calculators_) {
            calculator->ApplyPermutation(order);
        }
    }

    void Swap(size_t i, size_t j) override {
        assert(i < size_ && j < size_);
        for (auto& calculator : calculators_) {
            calculator->Swap(i, j);
        }
    }

    std::vector<size_t> GetCurrentOrder() override {
        assert(AllOrdersEqual());
        return calculators_[0]->GetCurrentOrder();
    }

private:
    Container CreateContainer(const Block::Container& data) {
        Container container(data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            container[i] = data[i]->GetOnlineCompressionCalculator();
        }
        return container;
    }

    bool AllOrdersEqual() const {
        for (size_t i = 1; i < calculators_.size(); ++i) {
            if (calculators_[i]->GetCurrentOrder() == calculators_[0]->GetCurrentOrder()) {
                return false;
            }
        }
        return true;
    }

    const size_t size_;
    Container calculators_;
};

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

void Block::ApplyPermutation(const std::vector<size_t>& order) {
    for (const auto& column : data_) {
        column->ApplyPermutation(order);
    }
}

const Block::Container& Block::GetData() const {
    return data_;
}

OnlineCompressionCalculatorPtr Block::GetOnlineCompressionCalculator() const {
    return std::make_unique<BlockOnlineCompressionCalculator>(data_);
}

namespace {

std::vector<std::vector<std::string>> Transpose(std::vector<std::vector<std::string>> data) {
    size_t n = data.size();
    size_t m = data[0].size();
    std::vector<std::vector<std::string>> result(m, std::vector<std::string>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result[j][i] = data[i][j];
        }
    }
    return result;
}

}  // namespace

Block BlockParser(const std::vector<std::string>& lines, ColumnParser parser) {
    auto columns_data = Transpose(SplitAllStrings(lines, ','));
    Block::Container columns(columns_data.size());
    for (size_t i = 0; i < columns.size(); ++i) {
        columns[i] = parser(columns_data[i]);
    }
    return Block(std::move(columns));
}

}  // namespace CompressionSorts
