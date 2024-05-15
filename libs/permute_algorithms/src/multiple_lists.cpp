#include "compression_sorts/multiple_lists.hpp"

#include <list>

#include "compression_sorts/permutation.hpp"

namespace CompressionSorts {

void MultipleListsPermute::GetPermutation(const Block& block, std::vector<size_t>& order) const {
    const size_t block_size = block.GetSize();
    if (block_size == 0) {
        order.clear();
        return;
    }
    const auto& columns = block.GetData();
    order = GetIdentityPermutation(block_size);
    std::vector<std::vector<size_t>> column_orders(columns.size(), order);
    for (size_t i = 0; i < columns.size(); ++i) {
        const auto& column = columns[i];
        column->UpdatePermutation(column_orders[i], Range{.from = 0, .to = block_size},
                                  Algorithms::LexicographicSort);
    }

    auto calculate_hamming_distance = [&columns](size_t lhs, size_t rhs) -> uint64_t {
        uint64_t hamming_distance = 0;
        for (size_t i = 0; i < columns.size(); ++i) {
            if (columns[i]->Compare(lhs, rhs) != 0) {
                ++hamming_distance;
            }
        }
        return hamming_distance;
    };

    std::vector<std::list<size_t>> alive(columns.size());
    std::vector<std::vector<std::list<size_t>::const_iterator>> positions(columns.size());

    for (size_t i = 0; i < columns.size(); ++i) {
        positions[i].resize(block_size);
        for (size_t j = 0; j < block_size; ++j) {
            alive[i].push_back(column_orders[i][j]);
            positions[i][column_orders[i][j]] = --alive[i].end();
        }
    }

    std::vector<bool> used(block_size, false);

    size_t first_id = column_orders[0][0];
    order[0] = first_id;
    used[first_id] = true;

    for (size_t i = 1; i < block_size; ++i) {
        size_t prev_id = order[i - 1];
        size_t best_next_id = -1;
        uint64_t best_distance = columns.size() + 1;

        auto try_update = [&](size_t position) {
            uint64_t cur_distance = calculate_hamming_distance(position, prev_id);
            if (cur_distance < best_distance) {
                best_distance = cur_distance;
                best_next_id = position;
            }
        };

        for (size_t j = 0; j < columns.size(); ++j) {
            auto& list = alive[j];
            auto position = positions[j][prev_id];
            if (position != list.begin()) {
                try_update(*std::prev(position));
            }
            assert(position != list.end());
            auto next_position = std::next(position);
            if (next_position != list.end()) {
                try_update(*next_position);
            }
            list.erase(position);
        }

        assert(best_next_id != -1);
        assert(!used[best_next_id]);
        used[best_next_id] = true;
        order[i] = best_next_id;
    }
}

std::string MultipleListsPermute::GetName() const {
    return "multiple lists";
}

}  // namespace CompressionSorts
