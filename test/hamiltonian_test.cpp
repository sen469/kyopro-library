#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

#include "kyopro/hamiltonian.hpp"

using adjacency = std::vector<std::vector<int>>;

bool valid(const std::vector<int>& order, const adjacency& edges, bool cycle,
           bool directed, int start = -1, int goal = -1) {
    int n = static_cast<int>(edges.size());
    if (n == 0 || static_cast<int>(order.size()) != n) return false;
    std::vector<bool> seen(n, false);
    for (int v : order) {
        if (v < 0 || v >= n || seen[v]) return false;
        seen[v] = true;
    }
    if (start != -1 && order.front() != start) return false;
    if (goal != -1 && order.back() != goal) return false;
    for (int i = 1; i < n; i++) if (!edges[order[i - 1]][order[i]]) return false;
    if (cycle) {
        if (!edges[order.back()][order.front()]) return false;
        if (!directed && n == 2 && edges[0][1] < 2) return false;
    }
    return true;
}

bool brute_force(const adjacency& edges, bool cycle, bool directed,
                 int start = -1, int goal = -1) {
    std::vector<int> order(edges.size());
    std::iota(order.begin(), order.end(), 0);
    do {
        if (valid(order, edges, cycle, directed, start, goal)) return true;
    } while (std::next_permutation(order.begin(), order.end()));
    return false;
}

template <bool Directed>
void random_test() {
    std::mt19937 rng(469 + Directed);
    for (int n = 0; n <= 7; n++) {
        for (int trial = 0; trial < 35; trial++) {
            adjacency edges(n, std::vector<int>(n));
            std::vector<std::pair<int, int>> list;
            std::vector<std::vector<int>> neighbors(n);
            kyopro::hamiltonian<Directed> graph(n);
            for (int u = 0; u < n; u++) {
                for (int v = Directed ? 0 : u; v < n; v++) {
                    int copies = rng() % 4 == 0 ? static_cast<int>(rng() % 3) : 0;
                    for (int i = 0; i < copies; i++) {
                        graph.add_edge(u, v);
                        list.emplace_back(u, v);
                        neighbors[u].push_back(v);
                        edges[u][v]++;
                        if (!Directed && u != v) {
                            neighbors[v].push_back(u);
                            edges[v][u]++;
                        }
                    }
                }
            }
            const kyopro::hamiltonian<Directed> from_list(n, list);
            const kyopro::hamiltonian<Directed> from_neighbors(neighbors);
            assert(graph.size() == n);
            for (auto query : std::vector<std::pair<int, int>>{
                     {-1, -1}, {n == 0 ? -1 : 0, -1}, {-1, n - 1},
                     {n == 0 ? -1 : 0, n - 1}}) {
                int start = query.first, goal = query.second;
                auto path = graph.path(start, goal);
                assert(!path.empty() == brute_force(edges, false, Directed, start, goal));
                if (!path.empty()) assert(valid(path, edges, false, Directed, start, goal));
                assert(from_list.path(start, goal) == path);
                assert(from_neighbors.path(start, goal) == path);
            }
            for (int start = 0; start < std::max(n, 1); start++) {
                auto cycle = graph.cycle(start);
                assert(!cycle.empty() == brute_force(edges, true, Directed, start));
                if (!cycle.empty()) assert(valid(cycle, edges, true, Directed, start));
                assert(from_list.cycle(start) == cycle);
                assert(from_neighbors.cycle(start) == cycle);
            }
        }
    }
}

int main() {
    {
        kyopro::hamiltonian<> graph(4);
        graph.add_edge(0, 1);
        graph.add_edge(1, 2);
        graph.add_edge(2, 3);
        assert(graph.path() == std::vector<int>({0, 1, 2, 3}));
        assert(graph.path(3).empty());
        assert(graph.path(-1, 0).empty());
        assert(graph.path(0, 0).empty());
        assert(graph.cycle().empty());
        auto old_path = graph.path();
        graph.add_edge(3, 0);
        assert(graph.cycle() == std::vector<int>({0, 1, 2, 3}));
        assert(graph.cycle(2) == std::vector<int>({2, 3, 0, 1}));
        assert(old_path == std::vector<int>({0, 1, 2, 3}));
    }
    {
        kyopro::hamiltonian<false> graph(2);
        graph.add_edge(0, 1);
        assert(graph.path(1, 0) == std::vector<int>({1, 0}));
        assert(graph.cycle().empty());
        graph.add_edge(0, 1);
        assert(graph.cycle() == std::vector<int>({0, 1}));
        graph.add_edge(1, 0);
        assert(graph.cycle(1) == std::vector<int>({1, 0}));
    }
    {
        kyopro::hamiltonian<> single(1);
        assert(single.path() == std::vector<int>({0}));
        assert(single.cycle().empty());
        single.add_edge(0, 0);
        assert(single.cycle() == std::vector<int>({0}));
        kyopro::hamiltonian<> empty;
        assert(empty.path().empty());
        assert(empty.cycle().empty());
    }
    {
        // A larger state table exercises reconstruction beyond tiny brute-force cases.
        kyopro::hamiltonian<> chain(18);
        for (int v = 1; v < 18; v++) chain.add_edge(v - 1, v);
        auto path = chain.path();
        assert(path.size() == 18);
        for (int v = 0; v < 18; v++) assert(path[v] == v);
    }
    random_test<true>();
    random_test<false>();
}
