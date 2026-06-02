#include <bits/stdc++.h>

#include "kyopro/tree_diameter.hpp"

using namespace std;

int main() {
    {
        vector<vector<kyopro::tree_diameter_edge<long long>>> graph(6);
        auto add_edge = [&](int u, int v, long long cost) {
            graph[u].push_back({v, cost});
            graph[v].push_back({u, cost});
        };
        add_edge(0, 1, 3);
        add_edge(1, 2, 4);
        add_edge(1, 3, 2);
        add_edge(3, 4, 6);
        add_edge(3, 5, 1);

        auto res = kyopro::tree_diameter(graph);
        assert(res.diameter == 12);
        assert(res.from == 4);
        assert(res.to == 2);
        assert(res.path == vector<int>({4, 3, 1, 2}));
    }

    {
        int n = 5;
        vector<tuple<int, int, int>> edges = {
            {0, 1, 1},
            {1, 2, 2},
            {1, 3, 10},
            {3, 4, 3},
        };

        auto res = kyopro::tree_diameter(n, edges);
        assert(res.diameter == 15);
        assert(res.from == 4);
        assert(res.to == 2);
        assert(res.path == vector<int>({4, 3, 1, 2}));
    }

    {
        vector<vector<kyopro::tree_diameter_edge<int>>> graph(1);
        auto res = kyopro::tree_diameter(graph);
        assert(res.diameter == 0);
        assert(res.from == 0);
        assert(res.to == 0);
        assert(res.path == vector<int>({0}));
    }

    {
        vector<vector<kyopro::tree_diameter_edge<int>>> graph;
        auto res = kyopro::tree_diameter(graph);
        assert(res.diameter == 0);
        assert(res.from == -1);
        assert(res.to == -1);
        assert(res.path.empty());
    }

    return 0;
}
