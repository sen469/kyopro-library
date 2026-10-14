#ifndef KYOPRO_MINIMUM_SPANNING_TREE_HPP
#define KYOPRO_MINIMUM_SPANNING_TREE_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <tuple>
#include <vector>

#include <atcoder/dsu>

namespace kyopro {

template <class T>
struct mst_result {
    T cost;
    std::vector<int> edge_ids;
    int component_count;

    bool is_connected() const { return component_count <= 1; }
};

template <class T>
class minimum_spanning_tree {
    int n_;
    std::vector<std::tuple<int, int, T>> edges_;

public:
    explicit minimum_spanning_tree(int n = 0) : n_(n) {
        assert(0 <= n);
    }

    minimum_spanning_tree(int n, const std::vector<std::tuple<int, int, T>>& edges)
        : minimum_spanning_tree(n) {
        assert(edges.size() <= (std::size_t)std::numeric_limits<int>::max());
        edges_.reserve(edges.size());
        for (const auto& edge : edges) {
            add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
    }

    int add_edge(int u, int v, T cost) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        assert(edges_.size() < (std::size_t)std::numeric_limits<int>::max());
        int id = (int)edges_.size();
        edges_.emplace_back(u, v, cost);
        return id;
    }

    int size() const { return n_; }
    int edge_count() const { return (int)edges_.size(); }

    std::tuple<int, int, T> get_edge(int id) const {
        assert(0 <= id && id < edge_count());
        return edges_[id];
    }

    mst_result<T> build() const {
        int m = edge_count();
        std::vector<int> order(m);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            const T& x = std::get<2>(edges_[a]);
            const T& y = std::get<2>(edges_[b]);
            if (x < y) return true;
            if (y < x) return false;
            return a < b;
        });

        atcoder::dsu uf(n_);
        mst_result<T> result{T(0), {}, n_};
        result.edge_ids.reserve(std::min(m, n_ == 0 ? 0 : n_ - 1));
        for (int id : order) {
            int u = std::get<0>(edges_[id]), v = std::get<1>(edges_[id]);
            if (uf.same(u, v)) continue;
            uf.merge(u, v);
            result.cost = result.cost + std::get<2>(edges_[id]);
            result.edge_ids.push_back(id);
            --result.component_count;
            if (result.component_count == 1) break;
        }
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_MINIMUM_SPANNING_TREE_HPP
