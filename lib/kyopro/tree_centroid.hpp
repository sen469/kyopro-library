#ifndef KYOPRO_TREE_CENTROID_HPP
#define KYOPRO_TREE_CENTROID_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

class tree_centroid {
    std::vector<std::vector<int>> graph_;

public:
    explicit tree_centroid(int n = 0) {
        assert(0 <= n);
        graph_.resize(n);
    }
    explicit tree_centroid(const std::vector<std::vector<int>>& graph) : graph_(graph) {
        for (const auto& adj : graph_) for (int to : adj) {
            assert(0 <= to && to < size());
            (void)to;
        }
    }
    tree_centroid(int n, const std::vector<std::pair<int, int>>& edges) : tree_centroid(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }
    void add_edge(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(u != v);
        graph_[u].push_back(v);
        graph_[v].push_back(u);
    }
    int size() const { return (int)graph_.size(); }

    std::vector<int> build() const { return build(std::vector<int>(size(), 1)); }

    template <class T>
    std::vector<int> build(const std::vector<T>& weights) const {
        static_assert(std::numeric_limits<T>::is_integer && !std::is_same<T, bool>::value,
                      "weights must have an integer type other than bool");
        const auto& graph = graph_;
        int n = size();
        assert(weights.size() == graph.size());
        for (const auto& weight : weights) {
            assert(T(0) <= weight);
            (void)weight;
        }
        if (n == 0) return {};

        std::vector<int> parent(n, -1), order;
        order.reserve(n);
        parent[0] = 0;
        order.push_back(0);
        for (int i = 0; i < (int)order.size(); i++) {
            int v = order[i];
            for (int to : graph[v]) {
                assert(0 <= to && to < n);
                if (to == parent[v]) continue;
                assert(parent[to] == -1);
                parent[to] = v;
                order.push_back(to);
            }
        }
        assert((int)order.size() == n);

        // Reverse traversal order puts every child before its parent.
        std::vector<T> subtree_weight = weights;
        for (int i = n - 1; i > 0; i--) {
            int v = order[i];
            subtree_weight[parent[v]] += subtree_weight[v];
        }
        T total = subtree_weight[0];

        std::vector<int> result;
        for (int v = 0; v < n; v++) {
            T largest = total - subtree_weight[v];
            for (int to : graph[v]) {
                if (to != parent[v]) largest = std::max(largest, subtree_weight[to]);
            }
            // Avoid overflow from doubling largest.
            if (largest <= total - largest) result.push_back(v);
        }
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_TREE_CENTROID_HPP
