#include <algorithm>
#include <cassert>
#include <functional>
#include <random>
#include <utility>
#include <vector>

#include "kyopro/eulerian.hpp"

using edge_list = std::vector<std::pair<int, int>>;

bool brute_force(int n, const edge_list& edges, bool directed, bool closed, int start) {
    if (edges.empty()) return true;
    int all = (1 << edges.size()) - 1;
    std::function<bool(int, int, int)> dfs = [&](int v, int used, int first) {
        if (used == all) return !closed || v == first;
        for (int i = 0; i < static_cast<int>(edges.size()); i++) {
            if (used & (1 << i)) continue;
            auto [u, to] = edges[i];
            if (u == v && dfs(to, used | (1 << i), first)) return true;
            if (!directed && to == v && u != v && dfs(u, used | (1 << i), first)) return true;
        }
        return false;
    };
    for (int v = 0; v < n; v++) {
        if ((start == -1 || start == v) && dfs(v, 0, v)) return true;
    }
    return false;
}

void validate(const kyopro::eulerian_result& result, int n, const edge_list& edges,
              bool directed, bool closed, int start) {
    if (!result.exists) {
        assert(result.vertices.empty() && result.edge_ids.empty());
        return;
    }
    assert(result.edge_ids.size() == edges.size());
    if (n == 0) {
        assert(result.vertices.empty());
        return;
    }
    assert(result.vertices.size() == edges.size() + 1);
    for (int v : result.vertices) assert(0 <= v && v < n);
    if (start != -1) assert(result.vertices.front() == start);
    if (closed) assert(result.vertices.front() == result.vertices.back());
    std::vector<bool> used(edges.size(), false);
    for (std::size_t i = 0; i < result.edge_ids.size(); i++) {
        int id = result.edge_ids[i];
        assert(0 <= id && id < static_cast<int>(edges.size()) && !used[id]);
        used[id] = true;
        int u = result.vertices[i], v = result.vertices[i + 1];
        auto edge = edges[id];
        bool correct = edge == std::make_pair(u, v);
        if (!directed) correct = correct || edge == std::make_pair(v, u);
        assert(correct);
    }
}

template <bool Directed>
void random_test() {
    std::mt19937 rng(469 + Directed);
    for (int n = 0; n <= 5; n++) {
        for (int trial = 0; trial < 100; trial++) {
            int m = n == 0 ? 0 : rng() % 8;
            edge_list edges;
            std::vector<std::vector<int>> neighbors(n);
            kyopro::eulerian<Directed> graph(n);
            for (int id = 0; id < m; id++) {
                int u = rng() % n, v = rng() % n;
                assert(graph.add_edge(u, v) == id);
                edges.emplace_back(u, v);
                neighbors[u].push_back(v);
                if (!Directed && u != v) neighbors[v].push_back(u);
            }
            const kyopro::eulerian<Directed> from_list(n, edges);
            const kyopro::eulerian<Directed> from_neighbors(neighbors);
            edge_list neighbor_edges;
            for (int v = 0; v < n; v++) {
                for (int to : neighbors[v]) {
                    if (Directed || v <= to) neighbor_edges.emplace_back(v, to);
                }
            }
            assert(graph.size() == n && graph.edge_count() == m);
            assert(from_neighbors.edge_count() == m);
            for (int start = -1; start < n; start++) {
                for (bool closed : {false, true}) {
                    auto result = closed ? graph.cycle(start) : graph.path(start);
                    assert(result.exists == brute_force(n, edges, Directed, closed, start));
                    validate(result, n, edges, Directed, closed, start);
                    auto list_result = closed ? from_list.cycle(start) : from_list.path(start);
                    assert(list_result.exists == result.exists);
                    assert(list_result.vertices == result.vertices);
                    assert(list_result.edge_ids == result.edge_ids);
                    auto neighbor_result = closed ? from_neighbors.cycle(start) : from_neighbors.path(start);
                    assert(neighbor_result.exists == result.exists);
                    validate(neighbor_result, n, neighbor_edges, Directed, closed, start);
                }
            }
        }
    }
}

int main() {
    random_test<true>();
    random_test<false>();
    {
        kyopro::eulerian<> graph(4, {{1, 2}, {2, 1}, {1, 3}});
        auto path = graph.path();
        assert(path.exists);
        assert(path.vertices == std::vector<int>({1, 2, 1, 3}));
        assert(!graph.path(0).exists);
        assert(!graph.path(3).exists);
        assert(!graph.cycle().exists);
        graph.add_edge(3, 1);
        assert(graph.cycle().exists);
        assert(graph.cycle(2).exists);
        assert(!graph.cycle(0).exists);
        assert(path.vertices == std::vector<int>({1, 2, 1, 3}));
    }
    {
        kyopro::eulerian<false> graph(4, {{0, 1}, {2, 3}});
        assert(!graph.path().exists);
        kyopro::eulerian<false> disconnected(4, {{0, 1}, {0, 1}, {2, 3}, {2, 3}});
        assert(!disconnected.path().exists);
        assert(!disconnected.cycle().exists);
        kyopro::eulerian<> directed(4, {{0, 1}, {1, 0}, {2, 3}, {3, 2}});
        assert(!directed.cycle().exists);
    }
    {
        kyopro::eulerian<false> graph(3, {{1, 2}, {1, 2}, {1, 1}});
        const auto& view = graph;
        auto cycle = view.cycle();
        assert(cycle.exists);
        validate(cycle, 3, {{1, 2}, {1, 2}, {1, 1}}, false, true, -1);
        auto repeated = view.cycle();
        assert(cycle.vertices == repeated.vertices);
        assert(cycle.edge_ids == repeated.edge_ids);
        auto copy = graph;
        copy.add_edge(0, 1);
        assert(graph.edge_count() == 3);
    }
    {
        kyopro::eulerian<> empty;
        assert(empty.path().exists && empty.path().vertices.empty());
        assert(empty.cycle().exists);
        kyopro::eulerian<false> no_edges(3);
        assert(no_edges.path().vertices == std::vector<int>({0}));
        assert(no_edges.cycle(2).vertices == std::vector<int>({2}));
    }
    {
        // Long walks use an explicit stack, not recursion.
        int n = 200000;
        kyopro::eulerian<> chain(n);
        for (int v = 1; v < n; v++) chain.add_edge(v - 1, v);
        auto result = chain.path();
        assert(result.exists && result.vertices.size() == static_cast<std::size_t>(n));
        for (int v = 0; v < n; v++) assert(result.vertices[v] == v);
    }
}
