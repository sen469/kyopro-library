#ifndef KYOPRO_TREE_CENTROID_HPP
#define KYOPRO_TREE_CENTROID_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace kyopro {

inline std::vector<int> tree_centroid(const std::vector<std::vector<int>>& graph) {
    int n = (int)graph.size();
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
    std::vector<int> subtree_size(n, 1);
    for (int i = n - 1; i > 0; i--) {
        int v = order[i];
        subtree_size[parent[v]] += subtree_size[v];
    }

    std::vector<int> result;
    for (int v = 0; v < n; v++) {
        int largest = n - subtree_size[v];
        for (int to : graph[v]) {
            if (to != parent[v]) largest = std::max(largest, subtree_size[to]);
        }
        if (largest <= n / 2) result.push_back(v);
    }
    return result;
}

inline std::vector<int> tree_centroid(int n, const std::vector<std::pair<int, int>>& edges) {
    assert(0 <= n);
    assert(edges.size() == (std::size_t)(n == 0 ? 0 : n - 1));
    std::vector<std::vector<int>> graph(n);
    for (auto [u, v] : edges) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    return tree_centroid(graph);
}

}  // namespace kyopro

#endif  // KYOPRO_TREE_CENTROID_HPP
