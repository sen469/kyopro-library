#ifndef KYOPRO_DIRECTED_MST_HPP
#define KYOPRO_DIRECTED_MST_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
struct directed_mst_result {
    bool exists;
    T cost;
    std::vector<int> edge_ids;
    std::vector<int> parents;

    int parent(int v) const {
        assert(exists);
        assert(0 <= v && v < (int)parents.size());
        return parents[v];
    }
};

template <class T>
class directed_mst {
    struct work_edge {
        int from, to;
        T cost;
        int original;
        int previous;
    };
    struct level {
        std::vector<work_edge> edges;
        std::vector<int> incoming;
    };

    int n_;
    std::vector<std::tuple<int, int, T>> edges_;

public:
    explicit directed_mst(int n = 0) : n_(n) { assert(0 <= n); }

    directed_mst(int n, const std::vector<std::tuple<int, int, T>>& edges)
        : directed_mst(n) {
        assert(edges.size() <= (std::size_t)std::numeric_limits<int>::max());
        edges_.reserve(edges.size());
        for (const auto& edge : edges) {
            add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
    }

    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n_);
        assert(0 <= to && to < n_);
        assert(edges_.size() < (std::size_t)std::numeric_limits<int>::max());
        int id = (int)edges_.size();
        edges_.emplace_back(from, to, cost);
        return id;
    }

    int size() const { return n_; }
    int edge_count() const { return (int)edges_.size(); }
    std::tuple<int, int, T> get_edge(int id) const {
        assert(0 <= id && id < edge_count());
        return edges_[id];
    }

    directed_mst_result<T> build(int root) const {
        assert(n_ == 0 ? root == 0 : (0 <= root && root < n_));
        if (n_ == 0) return {false, T(0), {}, {}};

        std::vector<std::vector<int>> graph(n_);
        for (const auto& edge : edges_) graph[std::get<0>(edge)].push_back(std::get<1>(edge));
        std::vector<char> seen(n_, false);
        std::vector<int> order = {root};
        seen[root] = true;
        for (int i = 0; i < (int)order.size(); i++) {
            for (int to : graph[order[i]]) {
                if (seen[to]) continue;
                seen[to] = true;
                order.push_back(to);
            }
        }
        if ((int)order.size() != n_) return {false, T(0), {}, {}};

        std::vector<work_edge> current;
        current.reserve(edges_.size());
        for (int id = 0; id < edge_count(); id++) {
            int u = std::get<0>(edges_[id]), v = std::get<1>(edges_[id]);
            if (u != v && v != root) current.push_back({u, v, std::get<2>(edges_[id]), id, -1});
        }
        std::vector<level> history;
        int n = n_, r = root;
        while (true) {
            std::vector<int> incoming(n, -1);
            for (int i = 0; i < (int)current.size(); i++) {
                const auto& edge = current[i];
                int old = incoming[edge.to];
                if (old == -1 || edge.cost < current[old].cost) incoming[edge.to] = i;
            }
            for (int v = 0; v < n; v++) {
                if (v != r && incoming[v] == -1) return {false, T(0), {}, {}};
            }

            // Follow the chosen parent edges; each vertex is visited once per level.
            std::vector<int> visited(n, -1), component(n, -1);
            int cycles = 0;
            for (int start = 0; start < n; start++) {
                int v = start;
                while (v != r && visited[v] == -1) {
                    visited[v] = start;
                    v = current[incoming[v]].from;
                }
                if (v == r || visited[v] != start) continue;
                int u = v;
                do {
                    component[u] = cycles;
                    u = current[incoming[u]].from;
                } while (u != v);
                cycles++;
            }
            if (cycles == 0) {
                history.push_back({std::move(current), std::move(incoming)});
                break;
            }
            int next_n = cycles;
            for (int v = 0; v < n; v++) {
                if (component[v] == -1) component[v] = next_n++;
            }
            std::vector<work_edge> next;
            next.reserve(current.size());
            for (int i = 0; i < (int)current.size(); i++) {
                const auto& edge = current[i];
                int u = component[edge.from], v = component[edge.to];
                if (u == v) continue;
                T cost = edge.cost - current[incoming[edge.to]].cost;
                next.push_back({u, v, cost, edge.original, i});
            }
            history.push_back({std::move(current), std::move(incoming)});
            current = std::move(next);
            r = component[r];
            n = next_n;
        }

        std::vector<int> chosen = history.back().incoming;
        for (int k = (int)history.size() - 1; k > 0; k--) {
            const auto& previous = history[k - 1];
            std::vector<int> expanded = previous.incoming;
            for (int id : chosen) {
                if (id == -1) continue;
                int old = history[k].edges[id].previous;
                // The entering edge replaces the chosen incoming edge at its target.
                expanded[previous.edges[old].to] = old;
            }
            chosen = std::move(expanded);
        }
        directed_mst_result<T> result{true, T(0), {}, std::vector<int>(n_, -1)};
        result.edge_ids.reserve(n_ - 1);
        for (int id : chosen) {
            if (id != -1) result.edge_ids.push_back(history[0].edges[id].original);
        }
        std::sort(result.edge_ids.begin(), result.edge_ids.end());
        for (int id : result.edge_ids) {
            result.cost = result.cost + std::get<2>(edges_[id]);
            result.parents[std::get<1>(edges_[id])] = std::get<0>(edges_[id]);
        }
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DIRECTED_MST_HPP
