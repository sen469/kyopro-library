#include <bits/stdc++.h>

#include "kyopro/dijkstra.hpp"

using namespace std;

int main() {
    {
        vector<vector<kyopro::dijkstra_edge<long long>>> graph(5);
        graph[0].push_back({1, 2});
        graph[0].push_back({2, 5});
        graph[1].push_back({2, 1});
        graph[1].push_back({3, 2});
        graph[2].push_back({3, 1});

        auto res = kyopro::dijkstra(graph, 0);
        vector<long long> expected = {0, 2, 3, 4, res.inf};
        assert(res.dist == expected);
        assert(res.reachable(3));
        assert(!res.reachable(4));
        assert(res.path(3) == vector<int>({0, 1, 3}));
        assert(res.path(4).empty());
    }

    {
        int n = 4;
        vector<tuple<int, int, int>> edges = {
            {0, 1, 10},
            {0, 2, 1},
            {2, 1, 2},
            {1, 3, 3},
        };

        auto res = kyopro::dijkstra(n, edges, 0);
        vector<int> expected = {0, 3, 1, 6};
        assert(res.dist == expected);
        assert(res.path(3) == vector<int>({0, 2, 1, 3}));
    }

    {
        int n = 3;
        vector<tuple<int, int, int>> edges = {
            {0, 1, 4},
            {1, 2, 7},
        };

        auto res = kyopro::dijkstra(n, edges, 2, false);
        vector<int> expected = {11, 7, 0};
        assert(res.dist == expected);
        assert(res.path(0) == vector<int>({2, 1, 0}));
    }

    {
        int n = 1;
        vector<tuple<int, int, long long>> edges;
        auto res = kyopro::dijkstra(n, edges, 0);
        assert(res.dist == vector<long long>({0}));
        assert(res.path(0) == vector<int>({0}));
    }

    return 0;
}
