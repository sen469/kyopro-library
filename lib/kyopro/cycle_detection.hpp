#ifndef KYOPRO_CYCLE_DETECTION_HPP
#define KYOPRO_CYCLE_DETECTION_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

namespace kyopro {

template <bool Directed = true>
class cycle_detection {
    std::vector<std::vector<std::pair<int, int>>> graph_;
    int edge_count_ = 0;

public:
    explicit cycle_detection(int n = 0) {
        assert(0 <= n);
        graph_.resize(n);
    }
    explicit cycle_detection(const std::vector<std::vector<int>>& graph)
        : cycle_detection((int)graph.size()) {
        for (int v = 0; v < size(); v++) {
            for (int to : graph[v]) {
                assert(0 <= to && to < size());
                if (Directed || v <= to) add_edge(v, to);
            }
        }
    }
    cycle_detection(int n, const std::vector<std::pair<int, int>>& edges) : cycle_detection(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }
    int add_edge(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(edge_count_ < std::numeric_limits<int>::max());
        int id = edge_count_++;
        graph_[u].push_back({v, id});
        if (!Directed) graph_[v].push_back({u, id});
        return id;
    }
    int size() const { return (int)graph_.size(); }

    std::vector<int> build() const {
        int n = size();
        std::vector<int> color(n), parent(n, -1), parent_edge(n, -1);
        std::vector<std::pair<int, std::size_t>> stack;
        for (int root = 0; root < n; root++) {
            if (color[root] != 0) continue;
            color[root] = 1;
            stack.push_back({root, 0});
            while (!stack.empty()) {
                int v = stack.back().first;
                std::size_t& index = stack.back().second;
                if (index == graph_[v].size()) {
                    color[v] = 2;
                    stack.pop_back();
                    continue;
                }
                auto edge = graph_[v][index++];
                int to = edge.first, id = edge.second;
                if (!Directed && id == parent_edge[v]) continue;
                if (color[to] == 0) {
                    parent[to] = v;
                    parent_edge[to] = id;
                    color[to] = 1;
                    stack.push_back({to, 0});
                } else if (color[to] == 1) {
                    std::vector<int> cycle;
                    for (int cur = v; cur != to; cur = parent[cur]) cycle.push_back(cur);
                    cycle.push_back(to);
                    std::reverse(cycle.begin(), cycle.end());
                    return cycle;
                }
            }
        }
        return {};
    }
};

using directed_cycle = cycle_detection<true>;
using undirected_cycle = cycle_detection<false>;

}  // namespace kyopro

#endif  // KYOPRO_CYCLE_DETECTION_HPP
