#ifndef KYOPRO_TOPOLOGICAL_SORT_HPP
#define KYOPRO_TOPOLOGICAL_SORT_HPP

#include <queue>
#include <utility>
#include <vector>

namespace kyopro {

template <class Graph>
std::vector<int> topological_sort(const Graph& graph) {
    int n = (int)graph.size();
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

inline std::vector<int> topological_sort(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> graph(n);
    for (auto [from, to] : edges) {
        graph[from].push_back(to);
    }
    return topological_sort(graph);
}

}  // namespace kyopro

#endif  // KYOPRO_TOPOLOGICAL_SORT_HPP
