#ifndef KYOPRO_CYCLE_DETECTION_HPP
#define KYOPRO_CYCLE_DETECTION_HPP

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace kyopro {

template <class Graph>
std::vector<int> directed_cycle(const Graph& graph) {
    int n = (int)graph.size();
    std::vector<int> color(n), parent(n, -1);
    std::vector<int> cycle;

    std::function<bool(int)> dfs = [&](int v) {
        color[v] = 1;
        for (int to : graph[v]) {
            if (color[to] == 0) {
                parent[to] = v;
                if (dfs(to)) return true;
            } else if (color[to] == 1) {
                cycle.push_back(to);
                for (int cur = v; cur != to; cur = parent[cur]) {
                    cycle.push_back(cur);
                }
                std::reverse(cycle.begin(), cycle.end());
                return true;
            }
        }
        color[v] = 2;
        return false;
    };

    for (int v = 0; v < n; v++) {
        if (color[v] == 0 && dfs(v)) return cycle;
    }
    return {};
}

inline std::vector<int> directed_cycle(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> graph(n);
    for (auto [from, to] : edges) {
        graph[from].push_back(to);
    }
    return directed_cycle(graph);
}

template <class Graph>
std::vector<int> undirected_cycle(const Graph& graph) {
    int n = (int)graph.size();
    std::vector<int> visited(n), parent(n, -1);
    std::vector<int> cycle;

    auto build_cycle = [&](int a, int b) {
        std::vector<int> path_a, path_b;
        std::vector<int> mark(n, -1);

        for (int v = a; v != -1; v = parent[v]) {
            mark[v] = (int)path_a.size();
            path_a.push_back(v);
        }

        int lca = -1;
        for (int v = b; v != -1; v = parent[v]) {
            path_b.push_back(v);
            if (mark[v] != -1) {
                lca = v;
                break;
            }
        }

        cycle.assign(path_a.begin(), path_a.begin() + mark[lca] + 1);
        for (int i = (int)path_b.size() - 2; i >= 0; i--) {
            cycle.push_back(path_b[i]);
        }
    };

    std::function<bool(int, int)> dfs = [&](int v, int p) {
        visited[v] = 1;
        for (int to : graph[v]) {
            if (to == v) {
                cycle = {v};
                return true;
            }
            if (to == p) continue;
            if (!visited[to]) {
                parent[to] = v;
                if (dfs(to, v)) return true;
            } else {
                build_cycle(v, to);
                return true;
            }
        }
        return false;
    };

    for (int v = 0; v < n; v++) {
        if (!visited[v] && dfs(v, -1)) return cycle;
    }
    return {};
}

inline std::vector<int> undirected_cycle(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<std::pair<int, int>>> graph(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [a, b] = edges[i];
        graph[a].push_back({b, i});
        graph[b].push_back({a, i});
    }

    std::vector<int> visited(n), parent(n, -1), parent_edge(n, -1);
    std::vector<int> cycle;

    auto build_cycle = [&](int a, int b) {
        std::vector<int> path_a, path_b;
        std::vector<int> mark(n, -1);

        for (int v = a; v != -1; v = parent[v]) {
            mark[v] = (int)path_a.size();
            path_a.push_back(v);
        }

        int lca = -1;
        for (int v = b; v != -1; v = parent[v]) {
            path_b.push_back(v);
            if (mark[v] != -1) {
                lca = v;
                break;
            }
        }

        cycle.assign(path_a.begin(), path_a.begin() + mark[lca] + 1);
        for (int i = (int)path_b.size() - 2; i >= 0; i--) {
            cycle.push_back(path_b[i]);
        }
    };

    std::function<bool(int)> dfs = [&](int v) {
        visited[v] = 1;
        for (auto [to, id] : graph[v]) {
            if (id == parent_edge[v]) continue;
            if (to == v) {
                cycle = {v};
                return true;
            }
            if (!visited[to]) {
                parent[to] = v;
                parent_edge[to] = id;
                if (dfs(to)) return true;
            } else {
                build_cycle(v, to);
                return true;
            }
        }
        return false;
    };

    for (int v = 0; v < n; v++) {
        if (!visited[v] && dfs(v)) return cycle;
    }
    return {};
}

}  // namespace kyopro

#endif  // KYOPRO_CYCLE_DETECTION_HPP
