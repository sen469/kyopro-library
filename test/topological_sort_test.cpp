#include <bits/stdc++.h>

#include "kyopro/topological_sort.hpp"

using namespace std;

bool is_topological_order(int n, const vector<pair<int, int>>& edges, const vector<int>& order) {
    if ((int)order.size() != n) return false;

    vector<int> position(n, -1);
    for (int i = 0; i < n; i++) {
        int v = order[i];
        if (v < 0 || n <= v) return false;
        if (position[v] != -1) return false;
        position[v] = i;
    }

    for (auto [from, to] : edges) {
        if (position[from] >= position[to]) return false;
    }
    return true;
}

int main() {
    {
        int n = 6;
        vector<pair<int, int>> edges = {
            {0, 2},
            {1, 2},
            {1, 3},
            {2, 4},
            {3, 4},
            {4, 5},
        };

        auto order = kyopro::topological_sort(n, edges);
        assert(is_topological_order(n, edges, order));
    }

    {
        vector<vector<int>> graph = {
            {1, 2},
            {3},
            {3},
            {},
        };
        vector<pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};

        auto order = kyopro::topological_sort(graph);
        assert(is_topological_order((int)graph.size(), edges, order));
    }

    {
        int n = 3;
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}};
        auto order = kyopro::topological_sort(n, edges);
        assert(order.empty());
    }

    {
        vector<vector<int>> graph(0);
        auto order = kyopro::topological_sort(graph);
        assert(order.empty());
    }

    {
        int n = 4;
        vector<pair<int, int>> edges;
        auto order = kyopro::topological_sort(n, edges);
        assert(is_topological_order(n, edges, order));
    }

    return 0;
}
