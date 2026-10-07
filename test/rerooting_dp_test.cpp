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

string concat(string a, string b) { return a + b; }
string empty_string() { return ""; }
string string_f_ve(string value, int id) { return "[" + to_string(id) + ":" + value + "]"; }
string string_f_ev(string value, int v) { return "(" + to_string(v) + value + ")"; }

struct vertex_value {
    int value;
};
int unwrap(vertex_value x, int) { return x.value; }
vertex_value wrap(int x, int) { return {x + 1}; }

int main() {
    {
        int n = 4;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {1, 3},
        };
        edge_cost = {2, 3, 4};

        kyopro::rerooting_dp<long long, long long, max_ll, zero_ll, add_cost, identity_ll> dp(n);
        for (auto [u, v] : edges) dp.add_edge(u, v);
        assert(dp.build() == vector<long long>({6, 4, 0, 0}));
        auto ans = dp.reroot();

        assert(ans == vector<long long>({6, 4, 7, 7}));
        assert(dp.reroot() == ans);
        assert(dp.build(2) == vector<long long>({0, 4, 7, 0}));
        assert(dp.reroot() == ans);
        edge_cost = {1, 1, 1};
        dp.build(3);
        assert(dp.reroot() == vector<long long>({2, 1, 2, 2}));
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

        kyopro::rerooting_dp<distance_sum_dp, distance_sum_dp, add_distance_sum, distance_sum_e,
                            distance_sum_f_ve, distance_sum_f_ev> dp(n);
        for (int i = 0; i < (int)edges.size(); i++) dp.add_edge(edges[i].first, edges[i].second, i);
        auto sub = dp.build();
        assert(sub[0].count == 5 && sub[0].sum == 16);
        assert(sub[1].count == 4 && sub[1].sum == 12);
        assert(sub[3].count == 2 && sub[3].sum == 4);
        auto ans = dp.reroot();

        vector<long long> expected = {16, 13, 19, 16, 28};
        for (int i = 0; i < n; i++) {
            assert(ans[i].count == n);
            assert(ans[i].sum == expected[i]);
        }
    }

    {
        kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex> dp(3);
        assert(dp.add_edge(0, 1) == 0);
        assert(dp.add_edge(1, 2) == 1);
        assert(dp.build() == vector<int>({3, 2, 1}));
        auto ans = dp.reroot();

        assert(ans == vector<int>({3, 3, 3}));
    }

    {
        kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex> dp(1);
        assert(dp.build() == vector<int>({1}));
        auto ans = dp.reroot();
        assert(ans == vector<int>({1}));
    }

    {
        kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex> dp;
        assert(dp.build().empty());
        auto ans = dp.reroot();
        assert(ans.empty());
        assert(dp.build().empty());
        assert(dp.reroot().empty());
    }

    {
        kyopro::rerooting_dp<long long, long long, max_ll, zero_ll, add_cost, identity_ll> dp(2);
        edge_cost = {3, 7};
        dp.add_edge(0, 1, 0, 1);
        assert(dp.build() == vector<long long>({3, 0}));
        assert(dp.reroot() == vector<long long>({3, 7}));
        assert(dp.build(1) == vector<long long>({0, 7}));
        assert(dp.reroot() == vector<long long>({3, 7}));
    }

    {
        kyopro::rerooting_dp<vertex_value, int, add_int, zero_int, unwrap, wrap> dp(2);
        dp.add_edge(0, 1);
        auto sub = dp.build(1);
        assert(sub[0].value == 1 && sub[1].value == 2);
        auto ans = dp.reroot();
        assert(ans[0].value == 2 && ans[1].value == 2);
    }

    mt19937 rng(469);
    for (int n = 1; n <= 12; n++) {
        for (int trial = 0; trial < 20; trial++) {
            kyopro::rerooting_dp<string, string, concat, empty_string, string_f_ve, string_f_ev> dp(n);
            vector<vector<pair<int, int>>> graph(n);
            vector<pair<int, int>> edges;
            for (int v = 1; v < n; v++) edges.push_back({int(rng() % v), v});
            shuffle(edges.begin(), edges.end(), rng);
            for (int i = 0; i < (int)edges.size(); i++) {
                auto [u, v] = edges[i];
                if (rng() % 2) swap(u, v);
                dp.add_edge(u, v, 2 * i, 2 * i + 1);
                graph[u].push_back({v, 2 * i});
                graph[v].push_back({u, 2 * i + 1});
            }
            vector<string> sub(n), expected(n);
            auto dfs = [&](auto&& self, int v, int parent) -> string {
                string acc;
                for (auto [to, id] : graph[v]) {
                    if (to != parent) acc += string_f_ve(self(self, to, v), id);
                }
                return sub[v] = string_f_ev(acc, v);
            };
            for (int root = 0; root < n; root++) expected[root] = dfs(dfs, root, -1);
            for (int root = 0; root < n; root++) {
                dfs(dfs, root, -1);
                assert(dp.build(root) == sub);
                assert(dp.reroot() == expected);
                assert(dp.reroot() == expected);
            }
        }
    }

    {
        int n = 200000;
        kyopro::rerooting_dp<int, int, add_int, zero_int, identity_int, add_vertex> dp(n);
        for (int v = 1; v < n; v++) dp.add_edge(v - 1, v);
        auto sub = dp.build(n - 1);
        assert(sub.front() == 1 && sub.back() == n);
        assert(dp.reroot() == vector<int>(n, n));
    }

    return 0;
}
