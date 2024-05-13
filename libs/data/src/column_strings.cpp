#include "compression_sorts/column_strings.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <unordered_set>

#include "compression_sorts/lz4.hpp"
#include "compression_sorts/permutation.hpp"
#include "compression_sorts/range.hpp"
#include "compression_sorts/serialize_data.hpp"
#include "compression_sorts/stupid_compression_calculator.hpp"
#include "compression_sorts/suffix_array.hpp"

namespace CompressionSorts {

namespace {

std::vector<size_t> SuffixArrayGreedyPermuteImpl(const std::vector<std::string>& data) {
    std::vector<size_t> prefix_sizes(data.size());
    char delimiter = '$';
    for (const auto& s : data) {
        if (!s.empty()) {
            delimiter =
                std::max(delimiter, static_cast<char>(*std::min_element(s.begin(), s.end()) - 1));
        }
    }
    std::cerr << "SuffixArrayGreedyPermuteImpl: choose delimiter = " << static_cast<int>(delimiter)
              << std::endl;
    for (size_t i = 0; i + 1 < data.size(); ++i) {
        prefix_sizes[i + 1] = prefix_sizes[i] + data[i].size();
    }
    const size_t estimated_size = prefix_sizes.back() + data.back().size() + data.size() - 1;
    std::string total_string;
    total_string.reserve(estimated_size);
    for (const auto& s : data) {
        if (!total_string.empty()) {
            total_string += delimiter;
        }
        total_string += s;
    }
    assert(total_string.size() == estimated_size);

    std::vector<size_t> string_ids;
    string_ids.assign(estimated_size, -1);

    size_t pos = 0;
    for (size_t string_id = 0; string_id < data.size(); ++string_id) {
        for (size_t i = 0; i < data[string_id].size(); ++i) {
            string_ids[pos++] = string_id;
        }
        ++pos;
    }

    const auto suffix_array = BuildSuffixArray(total_string);
    const auto inverse_suffix_array = GetInversePermutation(suffix_array);

    const auto lcp_array = BuildLCPArray(total_string, suffix_array);

    auto calc_lcp = [&lcp_array](size_t i, size_t j) -> int {
        assert(i != j);
        if (i > j) {
            std::swap(i, j);
        }
        return *std::min_element(lcp_array.begin() + i, lcp_array.begin() + j);
    };

    std::set<size_t> alive;

    for (size_t i = 0; i < suffix_array.size(); ++i) {
        if (total_string[i] != delimiter) {
            alive.insert(i);
        }
    }

    auto clear_alive = [&](size_t string_id) mutable {
        for (size_t i = 0; i < data[string_id].size(); ++i) {
            auto it = alive.find(inverse_suffix_array[prefix_sizes[string_id] + i]);
            assert(it != alive.end());
            alive.erase(it);
        }
    };

    constexpr int64_t checked_suffix = 1;

    std::vector<size_t> order(data.size());

    for (int64_t i = 0; i < order.size(); ++i) {
        int64_t best = 0;
        int64_t score = -1;

        auto try_update = [&](size_t id, size_t pos_sa) {
            int64_t cur_lcp = calc_lcp(pos_sa, id);
            if (cur_lcp > score) {
                best = string_ids[id];
                score = cur_lcp;
            }
        };

        for (int64_t j = std::max(i - checked_suffix, 0L); j < i; ++j) {
            size_t string_id = order[j];
            for (size_t pos = 0; pos < data[string_id].size(); ++pos) {
                size_t pos_sa = inverse_suffix_array[string_id + prefix_sizes[string_id] + pos];
                auto it = alive.lower_bound(pos_sa);
                // next
                if (it != alive.end()) {
                    try_update(*it, pos_sa);
                }
                // prev
                if (it != alive.begin()) {
                    --it;
                    try_update(*it, pos_sa);
                }
            }
        }
        clear_alive(best);
        order[i] = best;
    }

    return order;
}

void SuffixArrayGreedyPermuteImpl(const std::vector<std::string>& data, std::vector<size_t>& order,
                                  const Range& range) {
    std::vector<std::string> to_order(range.Size());

    for (size_t i = range.from; i < range.to; ++i) {
        to_order[i - range.from] = data[order[i]];
    }

    auto current_order = SuffixArrayGreedyPermuteImpl(to_order);

    for (size_t i = 0; i < current_order.size(); ++i) {
        current_order[i] = order[current_order[i] + range.from];
    }

    for (size_t i = range.from; i < range.to; ++i) {
        order[i] = current_order[i - range.from];
    }
}

}  // namespace

ColumnStrings::ColumnStrings(ColumnStrings::Container data) : data_(std::move(data)) {
}

std::string ColumnStrings::GetTypeName() const {
    return "std::string";
}

size_t ColumnStrings::GetSize() const {
    return data_.size();
}

size_t ColumnStrings::GetSerializedSize() const {
    return SerializeData(data_).size();
}

size_t ColumnStrings::CalculateCompressionSize() const {
    return CompressLz4(SerializeData(data_)).size();
}

void ColumnStrings::ApplyPermutation(const std::vector<size_t>& order) {
    data_ = CompressionSorts::ApplyPermutation(data_, order);
}

size_t ColumnStrings::CalculateDistinctValuesInRange(const Range& range) const {
    assert(range.from <= range.to);
    auto begin = std::ranges::next(data_.begin(), range.from);
    auto end = std::ranges::next(data_.begin(), range.to);
    std::unordered_set<std::string> elements{begin, end};
    return elements.size();
}

void ColumnStrings::UpdatePermutation(std::vector<size_t>& order, const Range& range,
                                      Algorithms algorithm) const {
    assert(range.from <= range.to);
    if (range.Size() <= 1ULL) {
        return;
    }
    switch (algorithm) {
        case Algorithms::LexicographicSort: {
            auto comparator = [&](size_t i, size_t j) { return data_[i] < data_[j]; };
            auto begin = std::ranges::next(order.begin(), range.from);
            auto end = std::ranges::next(order.begin(), range.to);
            std::sort(begin, end, comparator);
            break;
        }
        case Algorithms::SuffixArrayGreedy: {
            SuffixArrayGreedyPermuteImpl(data_, order, range);
            break;
        }
    }
}

OnlineCompressionCalculatorPtr ColumnStrings::GetOnlineCompressionCalculator() const {
    return std::make_unique<StupidOnlineCompressionCalculator<std::string>>(data_);
}

EqualRanges ColumnStrings::GetEqualRanges(const std::vector<size_t>& order,
                                          const Range& range) const {
    return ::CompressionSorts::GetEqualRanges(data_, order, range);
}

std::vector<char> ColumnStrings::GetSerializedData() const {
    return SerializeData(data_);
}

}  // namespace CompressionSorts
