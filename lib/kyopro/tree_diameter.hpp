#ifndef KYOPRO_TREE_DIAMETER_HPP
#define KYOPRO_TREE_DIAMETER_HPP

#include <algorithm>
#include <tuple>
#include <vector>

namespace kyopro {

template <class T>
struct tree_diameter_edge {
    int to;
    T cost;
};

template <class T>
struct tree_diameter_result {
    T diameter;
    int from;
    int to;
    std::vector<int> path;
};

namespace internal {

template <class T>
struct tree_diameter_farthest {
    int vertex;
    std::vector<int> parent;
    std::vector<T> dist;
};

template <class T>
tree_diameter_farthest<T> tree_diameter_find_farthest(const std::vector<std::vector<tree_diameter_edge<T>>>& graph,
                                                       int start) {
    int n = (int)graph.size();
    std::vector<int> parent(n, -1);
    std::vector<T> dist(n, T(0));
    std::vector<char> seen(n, false);
    std::vector<int> stack = {start};
    int farthest = start;
    seen[start] = true;

    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        if (dist[farthest] < dist[v]) farthest = v;

        for (auto e : graph[v]) {
            if (seen[e.to]) continue;
            seen[e.to] = true;
            parent[e.to] = v;
            dist[e.to] = dist[v] + e.cost;
            stack.push_back(e.to);
        }
    }

    return {farthest, parent, dist};
}

}  // namespace internal

template <class T>
tree_diameter_result<T> tree_diameter(const std::vector<std::vector<tree_diameter_edge<T>>>& graph) {
    int n = (int)graph.size();
    if (n == 0) return {T(0), -1, -1, {}};

    int s = internal::tree_diameter_find_farthest(graph, 0).vertex;
    auto res = internal::tree_diameter_find_farthest(graph, s);
    int t = res.vertex;

    std::vector<int> path;
    for (int v = t; v != -1; v = res.parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());

    return {res.dist[t], s, t, path};
}

template <class T>
tree_diameter_result<T> tree_diameter(int n, const std::vector<std::tuple<int, int, T>>& edges) {
    std::vector<std::vector<tree_diameter_edge<T>>> graph(n);
    for (auto [u, v, cost] : edges) {
        graph[u].push_back({v, cost});
        graph[v].push_back({u, cost});
    }
    return tree_diameter(graph);
}

}  // namespace kyopro

#endif  // KYOPRO_TREE_DIAMETER_HPP
