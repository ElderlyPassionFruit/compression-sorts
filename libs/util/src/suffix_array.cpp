#include "compression_sorts/suffix_array.hpp"

#include <numeric>
#include <set>

#include "compression_sorts/permutation.hpp"
#include "compression_sorts/sparse_table.hpp"

namespace CompressionSorts {

namespace {

// https://github.com/the-tourist/algo/blob/master/string/suffix_array.cpp

template <typename T>
std::vector<int> suffix_array(int n, const T &s, int char_bound) {
    std::vector<int> a(n);
    if (n == 0) {
        return a;
    }
    if (char_bound != -1) {
        std::vector<int> aux(char_bound, 0);
        for (int i = 0; i < n; i++) {
            aux[s[i]]++;
        }
        int sum = 0;
        for (int i = 0; i < char_bound; i++) {
            int add = aux[i];
            aux[i] = sum;
            sum += add;
        }
        for (int i = 0; i < n; i++) {
            a[aux[s[i]]++] = i;
        }
    } else {
        std::iota(a.begin(), a.end(), 0);
        std::sort(a.begin(), a.end(), [&s](int i, int j) { return s[i] < s[j]; });
    }
    std::vector<int> sorted_by_second(n);
    std::vector<int> ptr_group(n);
    std::vector<int> new_group(n);
    std::vector<int> group(n);
    group[a[0]] = 0;
    for (int i = 1; i < n; i++) {
        group[a[i]] = group[a[i - 1]] + (!(s[a[i]] == s[a[i - 1]]));
    }
    int cnt = group[a[n - 1]] + 1;
    int step = 1;
    while (cnt < n) {
        int at = 0;
        for (int i = n - step; i < n; i++) {
            sorted_by_second[at++] = i;
        }
        for (int i = 0; i < n; i++) {
            if (a[i] - step >= 0) {
                sorted_by_second[at++] = a[i] - step;
            }
        }
        for (int i = n - 1; i >= 0; i--) {
            ptr_group[group[a[i]]] = i;
        }
        for (int i = 0; i < n; i++) {
            int x = sorted_by_second[i];
            a[ptr_group[group[x]]++] = x;
        }
        new_group[a[0]] = 0;
        for (int i = 1; i < n; i++) {
            if (group[a[i]] != group[a[i - 1]]) {
                new_group[a[i]] = new_group[a[i - 1]] + 1;
            } else {
                int pre = (a[i - 1] + step >= n ? -1 : group[a[i - 1] + step]);
                int cur = (a[i] + step >= n ? -1 : group[a[i] + step]);
                new_group[a[i]] = new_group[a[i - 1]] + (pre != cur);
            }
        }
        swap(group, new_group);
        cnt = group[a[n - 1]] + 1;
        step <<= 1;
    }
    return a;
}

template <typename T>
std::vector<int> suffix_array(const T &s, int char_bound) {
    return suffix_array((int)s.size(), s, char_bound);
}

template <typename T>
std::vector<int> build_lcp(int n, const T &s, const std::vector<int> &sa) {
    assert((int)sa.size() == n);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        pos[sa[i]] = i;
    }
    std::vector<int> lcp(std::max(n - 1, 0));
    int k = 0;
    for (int i = 0; i < n; i++) {
        k = std::max(k - 1, 0);
        if (pos[i] == n - 1) {
            k = 0;
        } else {
            int j = sa[pos[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
                k++;
            }
            lcp[pos[i]] = k;
        }
    }
    return lcp;
}

template <typename T>
std::vector<int> build_lcp(const T &s, const std::vector<int> &sa) {
    return build_lcp((int)s.size(), s, sa);
}

}  // namespace

std::vector<size_t> BuildSuffixArray(const std::string &s) {
    auto sa = suffix_array(s, -1);
    return {sa.begin(), sa.end()};
}

std::vector<int> BuildLCPArray(const std::string &s, const std::vector<size_t> &sa) {
    return build_lcp(s, {sa.begin(), sa.end()});
}

std::vector<size_t> GetSuffixArrayGreedyOrder(const std::vector<std::string> &data) {
    std::vector<size_t> prefix_sizes(data.size());
    char delimiter = '$';
    for (const auto &s : data) {
        if (!s.empty()) {
            delimiter =
                std::min(delimiter, static_cast<char>(*std::min_element(s.begin(), s.end()) - 1));
        }
    }
    for (size_t i = 0; i + 1 < data.size(); ++i) {
        prefix_sizes[i + 1] = prefix_sizes[i] + data[i].size();
    }
    const size_t estimated_size = prefix_sizes.back() + data.back().size() + data.size();
    std::string total_string;
    total_string.reserve(estimated_size);
    for (const auto &s : data) {
        total_string += s;
        total_string += delimiter;
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

    std::vector<int> max_length(estimated_size, 0);
    for (size_t i = estimated_size; i > 0; --i) {
        if (total_string[i - 1] != delimiter) {
            max_length[i - 1] = max_length[i] + 1;
        }
    }

    const auto suffix_array = BuildSuffixArray(total_string);
    const auto inverse_suffix_array = GetInversePermutation(suffix_array);

    const auto lcp_array = BuildLCPArray(total_string, suffix_array);

    const auto sparse_table = SparseTable(lcp_array, [](int i, int j) { return std::min(i, j); });

    auto calc_lcp = [&sparse_table, &inverse_suffix_array, &max_length](size_t i, size_t j) -> int {
        assert(i != j);
        if (i > j) {
            std::swap(i, j);
        }
        return std::min<int>({sparse_table.get(i, j - 1), max_length[inverse_suffix_array[i]],
                              max_length[inverse_suffix_array[j]]});
    };

    std::set<size_t> alive;

    for (size_t i = 0; i < suffix_array.size(); ++i) {
        if (string_ids[i] != -1) {
            alive.insert(inverse_suffix_array[i]);
        }
    }

    auto clear_alive = [&](size_t string_id) mutable {
        for (size_t i = 0; i < data[string_id].size(); ++i) {
            size_t pos_to_erase = inverse_suffix_array[string_id + prefix_sizes[string_id] + i];
            assert(string_ids[suffix_array[pos_to_erase]] == string_id);
            auto it = alive.find(pos_to_erase);
            assert(it != alive.end());
            alive.erase(it);
        }
    };

    constexpr int64_t checked_suffix = 1;

    std::vector<size_t> order(data.size(), -1);

    std::vector<bool> used(order.size(), false);
    size_t next_not_used = 0;

    for (int64_t i = 0; i < order.size(); ++i) {
        while (next_not_used < order.size() && used[next_not_used]) {
            ++next_not_used;
        }
        assert(next_not_used < order.size());
        int64_t best = next_not_used;
        int64_t score = -1;

        auto try_update = [&](size_t id, size_t pos_sa) {
            int64_t cur_lcp = calc_lcp(pos_sa, id);
            if (cur_lcp > score) {
                best = string_ids[suffix_array[id]];
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
        assert(!used[best]);
        used[best] = true;
        clear_alive(best);
        order[i] = best;
    }

    assert(IsPermutation(order));
    return order;
}

}  // namespace CompressionSorts
