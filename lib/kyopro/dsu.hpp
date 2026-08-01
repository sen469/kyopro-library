#ifndef KYOPRO_DSU_HPP
#define KYOPRO_DSU_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <unordered_set>
#include <vector>

namespace kyopro {

class dsu {
private:
    int n_;
    std::vector<int> parent_or_size_;
    std::vector<long long> edge_count_;
    std::vector<long long> unique_edge_count_;
    std::unordered_set<std::uint64_t> edges_;

    int leader_internal(int a) {
        if (parent_or_size_[a] < 0) return a;
        return parent_or_size_[a] = leader_internal(parent_or_size_[a]);
    }

    static std::uint64_t edge_key(int a, int b) {
        if (b < a) std::swap(a, b);
        return (std::uint64_t)(std::uint32_t)a << 32 | (std::uint32_t)b;
    }

public:
    dsu() : n_(0) {}

    explicit dsu(int n)
        : n_(n), parent_or_size_(n, -1), edge_count_(n, 0), unique_edge_count_(n, 0) {
        assert(0 <= n);
    }

    int merge(int a, int b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);

        bool is_new_edge = edges_.insert(edge_key(a, b)).second;
        int x = leader(a);
        int y = leader(b);
        if (x == y) {
            edge_count_[x]++;
            if (is_new_edge) unique_edge_count_[x]++;
            return x;
        }

        if (-parent_or_size_[x] < -parent_or_size_[y]) std::swap(x, y);
        parent_or_size_[x] += parent_or_size_[y];
        parent_or_size_[y] = x;
        edge_count_[x] += edge_count_[y] + 1;
        unique_edge_count_[x] += unique_edge_count_[y] + (is_new_edge ? 1 : 0);
        return x;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        return leader(a) == leader(b);
    }

    int leader(int a) {
        assert(0 <= a && a < n_);
        return leader_internal(a);
    }

    int size(int a) {
        assert(0 <= a && a < n_);
        return -parent_or_size_[leader(a)];
    }

    long long edge_count(int a) {
        assert(0 <= a && a < n_);
        return edge_count_[leader(a)];
    }

    long long unique_edge_count(int a) {
        assert(0 <= a && a < n_);
        return unique_edge_count_[leader(a)];
    }

    std::vector<std::vector<int>> groups() {
        std::vector<int> leader_buf(n_), group_size(n_);
        for (int i = 0; i < n_; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(n_);
        for (int i = 0; i < n_; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < n_; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [&](const std::vector<int>& v) { return v.empty(); }),
                     result.end());
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DSU_HPP
