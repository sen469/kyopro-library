#include <bits/stdc++.h>

#include "kyopro/cycle_detection.hpp"

using namespace std;

bool is_directed_cycle(const vector<pair<int, int>>& edges, const vector<int>& cycle) {
    if (cycle.empty()) return false;

    set<pair<int, int>> edge_set(edges.begin(), edges.end());
    int n = (int)cycle.size();
    for (int i = 0; i < n; i++) {
        int from = cycle[i];
        int to = cycle[(i + 1) % n];
        if (!edge_set.count({from, to})) return false;
    }
    return true;
}

bool is_undirected_cycle(const vector<pair<int, int>>& edges, const vector<int>& cycle) {
    if (cycle.empty()) return false;
    if ((int)cycle.size() == 1) {
        return count(edges.begin(), edges.end(), make_pair(cycle[0], cycle[0])) > 0;
    }

    map<pair<int, int>, int> edge_count;
    for (auto [a, b] : edges) {
        if (a > b) swap(a, b);
        edge_count[{a, b}]++;
    }

    int n = (int)cycle.size();
    for (int i = 0; i < n; i++) {
        int a = cycle[i];
        int b = cycle[(i + 1) % n];
        if (a > b) swap(a, b);
        if (edge_count[{a, b}] == 0) return false;
    }
    return true;
}

int main() {
    {
        int n = 4;
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {2, 3}};
        auto cycle = kyopro::directed_cycle(n, edges);
        assert(is_directed_cycle(edges, cycle));
    }

    {
        vector<vector<int>> graph = {
            {1},
            {2},
            {3},
            {},
        };
        auto cycle = kyopro::directed_cycle(graph);
        assert(cycle.empty());
    }

    {
        int n = 4;
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 1}};
        auto cycle = kyopro::undirected_cycle(n, edges);
        assert(is_undirected_cycle(edges, cycle));
    }

    {
        int n = 2;
        vector<pair<int, int>> edges = {{0, 1}, {0, 1}};
        auto cycle = kyopro::undirected_cycle(n, edges);
        assert(is_undirected_cycle(edges, cycle));
    }

    {
        int n = 3;
        vector<pair<int, int>> edges = {{1, 1}};
        auto cycle = kyopro::undirected_cycle(n, edges);
        assert(cycle == vector<int>({1}));
    }

    {
        vector<vector<int>> graph = {
            {1, 2},
            {0, 2},
            {0, 1},
        };
        auto cycle = kyopro::undirected_cycle(graph);
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}};
        assert(is_undirected_cycle(edges, cycle));
    }

    {
        int n = 5;
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {3, 4}};
        auto cycle = kyopro::undirected_cycle(n, edges);
        assert(cycle.empty());
    }

    return 0;
}
