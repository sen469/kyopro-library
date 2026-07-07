#include <bits/stdc++.h>

#include "kyopro/rerooting_dp.hpp"

using namespace std;

vector<long long> edge_cost;

long long max_ll(long long a, long long b) { return max(a, b); }
long long zero_ll() { return 0; }
long long add_cost(long long vertex_dp, int edge_id) {
    return vertex_dp + edge_cost[edge_id];
}
long long identity_ll(long long merged, int) { return merged; }

struct distance_sum_dp {
    long long count;
    long long sum;
};

distance_sum_dp add_distance_sum(distance_sum_dp a, distance_sum_dp b) {
    return distance_sum_dp{a.count + b.count, a.sum + b.sum};
}
distance_sum_dp distance_sum_e() { return distance_sum_dp{0, 0}; }
distance_sum_dp distance_sum_f_ve(distance_sum_dp vertex_dp, int edge_id) {
    return distance_sum_dp{vertex_dp.count, vertex_dp.sum + vertex_dp.count * edge_cost[edge_id]};
}
distance_sum_dp distance_sum_f_ev(distance_sum_dp merged, int) {
    return distance_sum_dp{merged.count + 1, merged.sum};
}

int add_int(int a, int b) { return a + b; }
int zero_int() { return 0; }
int identity_int(int vertex_dp, int) { return vertex_dp; }
int add_vertex(int merged, int) { return merged + 1; }

int main() {
    {
        int n = 4;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {1, 3},
        };
        edge_cost = {2, 3, 4};

        auto ans = kyopro::rerooting_dp<long long, long long, max_ll, zero_ll, add_cost, identity_ll>(n, edges);

        assert(ans == vector<long long>({6, 4, 7, 7}));
    }

    {
        int n = 5;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {1, 3},
            {3, 4},
        };
        edge_cost = {1, 2, 3, 4};

        auto ans = kyopro::rerooting_dp<distance_sum_dp, distance_sum_dp, add_distance_sum, distance_sum_e,
                                        distance_sum_f_ve, distance_sum_f_ev>(n, edges);

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

        auto ans = kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex>(graph);

        assert(ans == vector<int>({3, 3, 3}));
    }

    {
        vector<pair<int, int>> edges;
        auto ans = kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex>(1, edges);
        assert(ans == vector<int>({1}));
    }

    {
        vector<pair<int, int>> edges;
        auto ans = kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex>(0, edges);
        assert(ans.empty());
    }

    return 0;
}
