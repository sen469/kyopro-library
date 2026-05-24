#ifndef KYOPRO_DIJKSTRA_HPP
#define KYOPRO_DIJKSTRA_HPP

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
struct dijkstra_edge {
    int to;
    T cost;
};

template <class T>
struct dijkstra_result {
    std::vector<T> dist;
    std::vector<int> prev;
    T inf;

    bool reachable(int v) const {
        return dist[v] != inf;
    }

    std::vector<int> path(int goal) const {
        if (!reachable(goal)) return {};

        std::vector<int> res;
        for (int v = goal; v != -1; v = prev[v]) {
            res.push_back(v);
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};

template <class T>
dijkstra_result<T> dijkstra(const std::vector<std::vector<dijkstra_edge<T>>>& graph, int start,
                            T inf = std::numeric_limits<T>::max() / 4) {
    int n = (int)graph.size();
    std::vector<T> dist(n, inf);
    std::vector<int> prev(n, -1);
    std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> que;

    dist[start] = 0;
    que.emplace(0, start);

    while (!que.empty()) {
        auto [d, v] = que.top();
        que.pop();
        if (dist[v] != d) continue;

        for (auto e : graph[v]) {
            T nd = d + e.cost;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                prev[e.to] = v;
                que.emplace(nd, e.to);
            }
        }
    }

    return {dist, prev, inf};
}

template <class T>
dijkstra_result<T> dijkstra(int n, const std::vector<std::tuple<int, int, T>>& edges, int start, bool directed = true,
                            T inf = std::numeric_limits<T>::max() / 4) {
    std::vector<std::vector<dijkstra_edge<T>>> graph(n);
    for (auto [from, to, cost] : edges) {
        graph[from].push_back({to, cost});
        if (!directed) graph[to].push_back({from, cost});
    }
    return dijkstra(graph, start, inf);
}

}  // namespace kyopro

#endif  // KYOPRO_DIJKSTRA_HPP
