#include <bits/stdc++.h>

#include "kyopro/directed_mst.hpp"

using namespace std;
using edge = tuple<int, int, long long>;

bool is_tree(int n, int root, const vector<edge>& edges, const vector<int>& ids) {
    if (n == 0 || (int)ids.size() != n - 1) return false;
    vector<int> indegree(n);
    vector<bool> used(edges.size());
    vector<vector<int>> graph(n);
    for (int id : ids) {
        if (id < 0 || id >= (int)edges.size() || used[id]) return false;
        used[id] = true;
        int u = get<0>(edges[id]), v = get<1>(edges[id]);
        indegree[v]++;
        graph[u].push_back(v);
    }
    for (int v = 0; v < n; v++) {
        if (indegree[v] != (v == root ? 0 : 1)) return false;
    }
    vector<bool> seen(n);
    vector<int> order = {root};
    seen[root] = true;
    for (int i = 0; i < (int)order.size(); i++) {
        for (int v : graph[order[i]]) {
            if (!seen[v]) {
                seen[v] = true;
                order.push_back(v);
            }
        }
    }
    return (int)order.size() == n;
}

void verify(int n, const vector<edge>& edges) {
    kyopro::directed_mst<long long> graph(n, edges);
    kyopro::directed_mst<long long> added(n);
    assert(graph.size() == n && graph.edge_count() == (int)edges.size());
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v, cost] = edges[i];
        int id = added.add_edge(u, v, cost);
        assert(id == i && added.get_edge(i) == edges[i]);
    }
    for (int root = 0; root < n; root++) {
        bool exists = false;
        long long best = LLONG_MAX;
        for (int mask = 0; mask < (1 << edges.size()); mask++) {
            if (__builtin_popcount((unsigned)mask) != n - 1) continue;
            vector<int> ids;
            long long cost = 0;
            for (int i = 0; i < (int)edges.size(); i++) {
                if (mask >> i & 1) {
                    ids.push_back(i);
                    cost += get<2>(edges[i]);
                }
            }
            if (is_tree(n, root, edges, ids)) {
                exists = true;
                best = min(best, cost);
            }
        }
        const auto& const_graph = graph;
        auto result = const_graph.build(root);
        assert(result.exists == exists);
        if (exists) {
            assert(result.cost == best);
            assert(is_tree(n, root, edges, result.edge_ids));
            assert(is_sorted(result.edge_ids.begin(), result.edge_ids.end()));
            long long cost = 0;
            for (int id : result.edge_ids) cost += get<2>(edges[id]);
            assert(cost == result.cost);
            vector<int> expected_parents(n, -1);
            for (int id : result.edge_ids) {
                expected_parents[get<1>(edges[id])] = get<0>(edges[id]);
            }
            assert(result.parents == expected_parents);
            assert(result.parent(root) == -1);
            for (int v = 0; v < n; v++) {
                assert(result.parent(v) == expected_parents[v]);
                int x = v, steps = 0;
                while (x != root && steps < n) {
                    assert(0 <= x && x < n);
                    x = result.parent(x);
                    steps++;
                }
                assert(x == root);
            }
        } else {
            assert(result.cost == 0 && result.edge_ids.empty());
            assert(result.parents.empty());
        }
        auto again = graph.build(root), other = added.build(root);
        assert(again.exists == result.exists && other.exists == result.exists);
        assert(again.cost == result.cost && other.cost == result.cost);
        assert(again.edge_ids == result.edge_ids && other.edge_ids == result.edge_ids);
        assert(again.parents == result.parents && other.parents == result.parents);
    }
    for (int i = 0; i < (int)edges.size(); i++) assert(graph.get_edge(i) == edges[i]);
}

int main() {
    kyopro::directed_mst<long long> empty;
    auto absent = empty.build(0);
    assert(!absent.exists && absent.cost == 0 && absent.edge_ids.empty());
    assert(absent.parents.empty());
    verify(1, {});
    verify(1, {{0, 0, -10}});
    verify(3, {});
    verify(3, {{0, 1, 3}, {1, 2, 4}, {2, 0, -100}});
    verify(3, {{1, 2, -2}, {2, 1, -3}}); // Unreachable cycle.
    verify(3, {{0, 1, 5}, {0, 2, 6}, {1, 2, 1}, {2, 1, 1}});
    verify(4, {{2, 1, 0}, {1, 2, 0}, {1, 3, 1}, {3, 1, 1}, {0, 1, 10}, {0, 3, 20}});
    verify(5, {{1, 2, 0}, {2, 1, 0}, {3, 4, 0}, {4, 3, 0},
               {0, 1, 5}, {0, 3, 7}, {2, 3, 1}});
    verify(3, {{0, 0, -100}, {0, 1, -3}, {0, 1, -3}, {1, 2, -2}, {0, 2, 4}});
    {
        kyopro::directed_mst<long long> graph(3);
        graph.add_edge(0, 1, 5);
        assert(!graph.build(0).exists);
        graph.add_edge(1, 2, 4);
        auto old = graph.build(0);
        assert(old.exists && old.cost == 9);
        graph.add_edge(0, 2, 1);
        auto result = graph.build(0);
        assert(result.cost == 6 && result.edge_ids == vector<int>({0, 2}));
        assert(old.cost == 9 && old.edge_ids == vector<int>({0, 1}));
        assert(old.parents == vector<int>({-1, 0, 1}));
        assert(result.parents == vector<int>({-1, 0, 0}));
        assert(!graph.build(2).exists);
    }
    {
        kyopro::directed_mst<int> graph(2);
        graph.add_edge(0, 1, 5);
        graph.add_edge(0, 1, 5);
        assert(graph.build(0).edge_ids == vector<int>({0}));
        kyopro::directed_mst<double> real(3);
        real.add_edge(0, 1, 2.0);
        real.add_edge(0, 2, 3.0);
        real.add_edge(1, 2, 0.5);
        real.add_edge(2, 1, 0.25);
        assert(real.build(0).cost == 2.5);
        kyopro::directed_mst<long long> large(3);
        large.add_edge(0, 1, 3000000000LL);
        large.add_edge(1, 2, 4000000000LL);
        assert(large.build(0).cost == 7000000000LL);
    }
    mt19937 rng(469);
    for (int n = 1; n <= 5; n++) {
        for (int trial = 0; trial < 150; trial++) {
            vector<edge> edges;
            int m = rng() % 11;
            for (int i = 0; i < m; i++) {
                edges.emplace_back(rng() % n, rng() % n, (int)(rng() % 21) - 10);
            }
            verify(n, edges);
        }
    }
    {
        int n = 200000;
        kyopro::directed_mst<long long> graph(n);
        for (int v = 1; v < n; v++) graph.add_edge(v - 1, v, 1);
        auto result = graph.build(0);
        assert(result.exists && result.cost == n - 1);
        assert((int)result.edge_ids.size() == n - 1);
        assert(!graph.build(n - 1).exists);
    }
}
