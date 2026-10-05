#include <bits/stdc++.h>

#include "kyopro/tree_centroid.hpp"

using namespace std;

vector<int> brute_centroids(const vector<vector<int>>& graph, const vector<long long>& weights) {
    int n = (int)graph.size();
    long long total = accumulate(weights.begin(), weights.end(), 0LL);
    vector<int> result;
    for (int removed = 0; removed < n; removed++) {
        vector<bool> seen(n, false);
        seen[removed] = true;
        long long largest = 0;
        for (int start = 0; start < n; start++) {
            if (seen[start]) continue;
            vector<int> stack = {start};
            seen[start] = true;
            long long count = 0;
            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                count += weights[v];
                for (int to : graph[v]) {
                    if (seen[to]) continue;
                    seen[to] = true;
                    stack.push_back(to);
                }
            }
            largest = max(largest, count);
        }
        if (largest <= total / 2) result.push_back(removed);
    }
    return result;
}

void check(int n, const vector<pair<int, int>>& edges, const vector<int>& expected) {
    vector<vector<int>> graph(n);
    for (auto [u, v] : edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    auto before = graph;
    assert(kyopro::tree_centroid(graph) == expected);
    assert(graph == before);
    assert(kyopro::tree_centroid(n, edges) == expected);
    assert(kyopro::tree_centroid(graph, vector<int>(n, 1)) == expected);
    assert(kyopro::tree_centroid(n, edges, vector<int>(n, 1)) == expected);
}

template <class T>
void check_weighted(int n, const vector<pair<int, int>>& edges,
                    const vector<T>& weights, const vector<int>& expected) {
    vector<vector<int>> graph(n);
    for (auto [u, v] : edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    auto before_graph = graph;
    auto before_weights = weights;
    assert(kyopro::tree_centroid(graph, weights) == expected);
    assert(kyopro::tree_centroid(n, edges, weights) == expected);
    assert(graph == before_graph);
    assert(weights == before_weights);
}

int main() {
    check(0, {}, {});
    check(1, {}, {0});
    check(2, {{0, 1}}, {0, 1});
    check(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}, {2});
    check(4, {{0, 1}, {1, 2}, {2, 3}}, {1, 2});
    check(6, {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 5}}, {4});
    check(7, {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}}, {0});

    mt19937 rng(469);
    check_weighted<long long>(0, {}, {}, {});
    check_weighted<long long>(1, {}, {0}, {0});
    check_weighted<long long>(1, {}, {7}, {0});
    check_weighted<long long>(3, {{0, 1}, {1, 2}}, {10, 1, 1}, {0});
    check_weighted<long long>(2, {{0, 1}}, {5, 5}, {0, 1});
    check_weighted<long long>(3, {{0, 1}, {1, 2}}, {0, 0, 0}, {0, 1, 2});
    check_weighted<long long>(4, {{0, 1}, {1, 2}, {2, 3}}, {5, 0, 0, 5}, {0, 1, 2, 3});
    check_weighted<long long>(3, {{0, 1}, {1, 2}}, {0, 0, LLONG_MAX}, {2});
    check_weighted<unsigned long long>(2, {{0, 1}}, {ULLONG_MAX, 0}, {0});
    for (int n = 1; n <= 60; n++) {
        for (int trial = 0; trial < 30; trial++) {
            vector<int> labels(n);
            iota(labels.begin(), labels.end(), 0);
            shuffle(labels.begin(), labels.end(), rng);
            vector<pair<int, int>> edges;
            vector<vector<int>> graph(n);
            for (int v = 1; v < n; v++) {
                int u = labels[rng() % v], w = labels[v];
                edges.emplace_back(u, w);
                graph[u].push_back(w);
                graph[w].push_back(u);
            }
            shuffle(edges.begin(), edges.end(), rng);
            for (auto& [u, v] : edges) {
                if (rng() % 2) swap(u, v);
            }
            check(n, edges, brute_centroids(graph, vector<long long>(n, 1)));
            vector<long long> weights(n);
            for (auto& weight : weights) weight = rng() % 11;
            check_weighted(n, edges, weights, brute_centroids(graph, weights));
        }
    }

    int n = 200000;
    vector<pair<int, int>> path;
    path.reserve(n - 1);
    for (int v = 1; v < n; v++) path.emplace_back(v - 1, v);
    check(n, path, {n / 2 - 1, n / 2});
    vector<long long> weights(n, 0);
    weights.back() = 100;
    check_weighted(n, path, weights, {n - 1});
    return 0;
}
