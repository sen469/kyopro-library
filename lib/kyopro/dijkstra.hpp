#ifndef KYOPRO_DIJKSTRA_HPP
#define KYOPRO_DIJKSTRA_HPP

#include <algorithm>
#include <cassert>
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
class dijkstra {
    std::vector<std::vector<dijkstra_edge<T>>> graph_;
    bool directed_;

public:
    explicit dijkstra(int n = 0, bool directed = true) : directed_(directed) {
        assert(0 <= n);
        graph_.resize(n);
    }

    explicit dijkstra(const std::vector<std::vector<dijkstra_edge<T>>>& graph, bool directed = true)
        : graph_(graph), directed_(directed) {
        for (const auto& adj : graph_) for (const auto& edge : adj) {
            assert(0 <= edge.to && edge.to < size());
            assert(!(edge.cost < T(0)));
            (void)edge;
        }
    }

    dijkstra(int n, const std::vector<std::tuple<int, int, T>>& edges, bool directed = true)
        : dijkstra(n, directed) {
        for (const auto& edge : edges) add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }

    void add_edge(int from, int to, T cost) {
        assert(0 <= from && from < size());
        assert(0 <= to && to < size());
        assert(!(cost < T(0)));
        graph_[from].push_back({to, cost});
        if (!directed_) graph_[to].push_back({from, cost});
    }

    int size() const { return (int)graph_.size(); }

    dijkstra_result<T> build(int start = 0, T inf = std::numeric_limits<T>::max() / 4) const {
        const auto& graph = graph_;
        int n = size();
        assert(n == 0 ? start == 0 : (0 <= start && start < n));
        assert(T(0) < inf);
        if (n == 0) return {{}, {}, inf};
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
                if (!(e.cost < inf - d)) continue;
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
};

}  // namespace kyopro

#endif  // KYOPRO_DIJKSTRA_HPP
