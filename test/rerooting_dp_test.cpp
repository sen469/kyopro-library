#include <bits/stdc++.h>

#include "kyopro/rerooting_dp.hpp"

using namespace std;

int main() {
    {
        int n = 4;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {1, 3},
        };
        vector<long long> cost = {2, 3, 4};

        auto ans = kyopro::rerooting_dp(
            n, edges, 0LL, [](long long a, long long b) { return max(a, b); },
            [&](long long vertex_dp, int edge_id) { return vertex_dp + cost[edge_id]; },
            [](long long merged, int) { return merged; });

        assert(ans == vector<long long>({6, 4, 7, 7}));
    }

    {
        struct dp {
            long long count;
            long long sum;
        };

        int n = 5;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {1, 3},
            {3, 4},
        };
        vector<long long> cost = {1, 2, 3, 4};

        auto ans = kyopro::rerooting_dp(
            n, edges, dp{0, 0},
            [](dp a, dp b) {
                return dp{a.count + b.count, a.sum + b.sum};
            },
            [&](dp vertex_dp, int edge_id) {
                return dp{vertex_dp.count, vertex_dp.sum + vertex_dp.count * cost[edge_id]};
            },
            [](dp merged, int) {
                return dp{merged.count + 1, merged.sum};
            });

        vector<long long> expected = {16, 13, 19, 16, 28};
        for (int i = 0; i < n; i++) {
            assert(ans[i].count == n);
            assert(ans[i].sum == expected[i]);
        }
    }

    {
        vector<vector<kyopro::rerooting_edge>> graph(3);
        auto add_edge = [&](int id, int u, int v) {
            graph[u].push_back({v, id});
            graph[v].push_back({u, id});
        };
        add_edge(0, 0, 1);
        add_edge(1, 1, 2);

        auto ans = kyopro::rerooting_dp(
            graph, 0, [](int a, int b) { return a + b; }, [](int vertex_dp, int) { return vertex_dp; },
            [](int merged, int) { return merged + 1; });

        assert(ans == vector<int>({3, 3, 3}));
    }

    {
        vector<pair<int, int>> edges;
        auto ans = kyopro::rerooting_dp(
            1, edges, 0, [](int a, int b) { return a + b; }, [](int vertex_dp, int) { return vertex_dp; },
            [](int merged, int) { return merged + 1; });
        assert(ans == vector<int>({1}));
    }

    {
        vector<pair<int, int>> edges;
        auto ans = kyopro::rerooting_dp(
            0, edges, 0, [](int a, int b) { return a + b; }, [](int vertex_dp, int) { return vertex_dp; },
            [](int merged, int) { return merged + 1; });
        assert(ans.empty());
    }

    return 0;
}
