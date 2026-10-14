#ifndef KYOPRO_TOPOLOGICAL_SORT_HPP
#define KYOPRO_TOPOLOGICAL_SORT_HPP

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

namespace kyopro {

class topological_sort {
    std::vector<std::vector<int>> graph_;

public:
    explicit topological_sort(int n = 0) {
        assert(0 <= n);
        graph_.resize(n);
    }
    explicit topological_sort(const std::vector<std::vector<int>>& graph) : graph_(graph) {
        for (const auto& adj : graph_) for (int to : adj) {
            assert(0 <= to && to < size());
            (void)to;
        }
    }
    topological_sort(int n, const std::vector<std::pair<int, int>>& edges) : topological_sort(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }
    void add_edge(int from, int to) {
        assert(0 <= from && from < size());
        assert(0 <= to && to < size());
        graph_[from].push_back(to);
    }
    int size() const { return (int)graph_.size(); }

    std::vector<int> build() const {
        const auto& graph = graph_;
        int n = size();
        std::vector<int> indegree(n);
        for (int v = 0; v < n; v++) {
            for (int to : graph[v]) {
                indegree[to]++;
            }
        }

        std::queue<int> que;
        for (int v = 0; v < n; v++) {
            if (indegree[v] == 0) que.push(v);
        }

        std::vector<int> order;
        order.reserve(n);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            order.push_back(v);

            for (int to : graph[v]) {
                indegree[to]--;
                if (indegree[to] == 0) que.push(to);
            }
        }

        if ((int)order.size() != n) return {};
        return order;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_TOPOLOGICAL_SORT_HPP
