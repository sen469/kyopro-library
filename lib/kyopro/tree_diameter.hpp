#ifndef KYOPRO_TREE_DIAMETER_HPP
#define KYOPRO_TREE_DIAMETER_HPP

#include <algorithm>
#include <cassert>
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
class tree_diameter {
    std::vector<std::vector<tree_diameter_edge<T>>> graph_;
    int edge_count_ = 0;

public:
    explicit tree_diameter(int n = 0) {
        assert(0 <= n);
        graph_.resize(n);
    }
    explicit tree_diameter(const std::vector<std::vector<tree_diameter_edge<T>>>& graph) : graph_(graph) {
        std::size_t arcs = 0;
        for (const auto& adj : graph_) for (const auto& edge : adj) {
            assert(0 <= edge.to && edge.to < size());
            assert(!(edge.cost < T(0)));
            (void)edge;
            arcs++;
        }
        assert(arcs % 2 == 0);
        edge_count_ = (int)(arcs / 2);
    }
    tree_diameter(int n, const std::vector<std::tuple<int, int, T>>& edges) : tree_diameter(n) {
        for (const auto& edge : edges) add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }
    void add_edge(int u, int v, T cost = T(1)) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(u != v);
        assert(!(cost < T(0)));
        graph_[u].push_back({v, cost});
        graph_[v].push_back({u, cost});
        edge_count_++;
    }
    int size() const { return (int)graph_.size(); }

    tree_diameter_result<T> build() const {
        const auto& graph = graph_;
        int n = size();
        assert(edge_count_ == (n == 0 ? 0 : n - 1));
        if (n == 0) return {T(0), -1, -1, {}};

        int s = internal::tree_diameter_find_farthest(graph, 0).vertex;
        auto res = internal::tree_diameter_find_farthest(graph, s);
        int t = res.vertex;
        for (int v = 0; v < n; v++) assert(v == s || res.parent[v] != -1);

        std::vector<int> path;
        for (int v = t; v != -1; v = res.parent[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());

        return {res.dist[t], s, t, path};
    }
};

}  // namespace kyopro

#endif  // KYOPRO_TREE_DIAMETER_HPP
