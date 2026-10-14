#include <bits/stdc++.h>

#include "kyopro/minimum_spanning_tree.hpp"

using namespace std;
using edge = tuple<int, int, long long>;

int components(int n, const vector<edge>& edges, const vector<int>& ids) {
    vector<vector<int>> graph(n);
    for (int id : ids) {
        auto [u, v, w] = edges[id];
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<bool> seen(n);
    int count = 0;
    for (int root = 0; root < n; root++) {
        if (seen[root]) continue;
        count++;
        vector<int> stack = {root};
        seen[root] = true;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            for (int to : graph[v]) {
                if (seen[to]) continue;
                seen[to] = true;
                stack.push_back(to);
            }
        }
    }
    return count;
}

void verify(int n, const vector<edge>& edges) {
    auto before = edges;
    auto result = kyopro::minimum_spanning_tree(n, edges).build();
    assert(edges == before);
    int m = (int)edges.size();
    vector<int> all(m);
    iota(all.begin(), all.end(), 0);
    int count = components(n, edges, all);
    assert(result.component_count == count);
    assert(result.is_connected() == (count <= 1));
    assert((int)result.edge_ids.size() == n - count);
    assert(components(n, edges, result.edge_ids) == count);
    vector<bool> used(m);
    long long actual = 0;
    for (int id : result.edge_ids) {
        assert(0 <= id && id < m && !used[id]);
        used[id] = true;
        actual += get<2>(edges[id]);
    }
    assert(actual == result.cost);
    for (int i = 1; i < (int)result.edge_ids.size(); i++) {
        int a = result.edge_ids[i - 1], b = result.edge_ids[i];
        assert(make_pair(get<2>(edges[a]), a) < make_pair(get<2>(edges[b]), b));
    }

    long long best = LLONG_MAX;
    for (int mask = 0; mask < (1 << m); mask++) {
        if (__builtin_popcount((unsigned)mask) != n - count) continue;
        vector<int> ids;
        long long cost = 0;
        for (int i = 0; i < m; i++) {
            if (mask >> i & 1) {
                ids.push_back(i);
                cost += get<2>(edges[i]);
            }
        }
        // A spanning subgraph with n-c edges and c components is a forest.
        if (components(n, edges, ids) == count) best = min(best, cost);
    }
    assert(result.cost == best);
    kyopro::minimum_spanning_tree<long long> graph(n);
    assert(graph.size() == n && graph.edge_count() == 0);
    for (int i = 0; i < m; i++) {
        auto [u, v, cost] = edges[i];
        int id = graph.add_edge(u, v, cost);
        assert(id == i);
        assert(graph.get_edge(id) == edges[i]);
    }
    assert(graph.edge_count() == m);
    const auto& const_graph = graph;
    for (int repeat = 0; repeat < 2; repeat++) {
        auto rebuilt = const_graph.build();
        assert(rebuilt.cost == result.cost);
        assert(rebuilt.edge_ids == result.edge_ids);
        assert(rebuilt.component_count == result.component_count);
    }
}

int main() {
    {
        kyopro::minimum_spanning_tree<long long> graph;
        assert(graph.size() == 0 && graph.build().component_count == 0);
    }
    {
        kyopro::minimum_spanning_tree<long long> graph(3);
        auto empty = graph.build();
        assert(empty.component_count == 3 && empty.cost == 0);
        graph.add_edge(0, 1, 5);
        auto first = graph.build();
        assert(first.cost == 5 && first.component_count == 2);
        graph.add_edge(1, 2, 3);
        auto second = graph.build();
        assert(second.cost == 8 && second.is_connected());
        graph.add_edge(0, 2, 1);
        auto third = graph.build();
        assert(third.cost == 4 && third.edge_ids == vector<int>({2, 1}));
        assert(first.cost == 5 && first.edge_ids == vector<int>({0}));
        assert(second.cost == 8 && second.edge_ids == vector<int>({1, 0}));
        graph.add_edge(0, 0, -100);
        assert(graph.build().cost == 4);
    }
    verify(0, {});
    verify(1, {});
    verify(4, {});
    verify(1, {{0, 0, -10}, {0, 0, 1}});
    verify(4, {{0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 1}, {2, 3, 5}});
    verify(5, {{0, 1, -2}, {0, 1, -4}, {2, 3, 0}, {4, 4, -100}});
    verify(3, {{0, 1, 0}, {1, 2, 0}, {0, 2, 0}});
    {
        vector<edge> edges = {{1, 2, 1}, {0, 2, 1}, {0, 1, 1}, {0, 1, 1}};
        auto result = kyopro::minimum_spanning_tree(3, edges).build();
        assert(result.edge_ids == vector<int>({0, 1}));
        assert(result.cost == 2 && result.is_connected());
    }
    {
        vector<tuple<int, int, int>> edges = {{0, 1, -3}, {1, 2, 5}};
        assert(kyopro::minimum_spanning_tree(3, edges).build().cost == 2);
        vector<tuple<int, int, double>> real_edges = {{0, 1, 0.5}, {1, 2, 1.25}, {0, 2, 2.0}};
        assert(kyopro::minimum_spanning_tree(3, real_edges).build().cost == 1.75);
        vector<edge> large_edges = {{0, 1, 3000000000LL}, {1, 2, 4000000000LL}};
        assert(kyopro::minimum_spanning_tree(3, large_edges).build().cost == 7000000000LL);
    }
    mt19937 rng(469);
    for (int n = 1; n <= 6; n++) {
        for (int trial = 0; trial < 150; trial++) {
            int m = rng() % 11;
            vector<edge> edges;
            for (int i = 0; i < m; i++) {
                edges.emplace_back(rng() % n, rng() % n, (int)(rng() % 21) - 10);
            }
            verify(n, edges);
        }
    }
    {
        int n = 200000;
        vector<edge> edges;
        for (int v = 1; v < n; v++) edges.emplace_back(v - 1, v, 1);
        auto result = kyopro::minimum_spanning_tree(n, edges).build();
        assert(result.cost == n - 1 && result.is_connected());
        assert((int)result.edge_ids.size() == n - 1);
    }
}
