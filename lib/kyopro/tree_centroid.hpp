#ifndef KYOPRO_TREE_CENTROID_HPP
#define KYOPRO_TREE_CENTROID_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
std::vector<int> tree_centroid(const std::vector<std::vector<int>>& graph,
                               const std::vector<T>& weights) {
    static_assert(std::numeric_limits<T>::is_integer && !std::is_same<T, bool>::value,
                  "weights must have an integer type other than bool");
    int n = (int)graph.size();
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

inline std::vector<int> tree_centroid(const std::vector<std::vector<int>>& graph) {
    return tree_centroid(graph, std::vector<int>(graph.size(), 1));
}

template <class T>
std::vector<int> tree_centroid(int n, const std::vector<std::pair<int, int>>& edges,
                               const std::vector<T>& weights) {
    assert(0 <= n);
    assert(weights.size() == (std::size_t)n);
    assert(edges.size() == (std::size_t)(n == 0 ? 0 : n - 1));
    std::vector<std::vector<int>> graph(n);
    for (auto [u, v] : edges) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    return tree_centroid(graph, weights);
}

inline std::vector<int> tree_centroid(int n, const std::vector<std::pair<int, int>>& edges) {
    assert(0 <= n);
    return tree_centroid(n, edges, std::vector<int>(n, 1));
}

}  // namespace kyopro

#endif  // KYOPRO_TREE_CENTROID_HPP
