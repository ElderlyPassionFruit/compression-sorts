#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace CompressionSorts {

// https://github.com/the-tourist/algo/blob/master/data/sparsetable.cpp

template <typename T, typename F>
class SparseTable {
public:
    size_t n;
    std::vector<std::vector<T>> mat;
    F func;

    SparseTable(const std::vector<T>& a, const F& f) : n(a.size()), func(f) {
        assert(n > 0);
        size_t max_log = 32 - __builtin_clz(n);
        mat.resize(max_log);
        mat[0] = a;
        for (size_t j = 1; j < max_log; j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (size_t i = 0; i <= n - (1 << j); i++) {
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T get(size_t from, size_t to) const {
        assert(from <= to && to <= n - 1);
        size_t lg = 32 - __builtin_clz(to - from + 1) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
    }
};

}  // namespace CompressionSorts
