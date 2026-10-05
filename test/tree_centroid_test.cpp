#include <bits/stdc++.h>

#include "kyopro/tree_centroid.hpp"

using namespace std;

vector<int> brute_centroids(const vector<vector<int>>& graph) {
    int n = (int)graph.size();
    vector<int> result;
    for (int removed = 0; removed < n; removed++) {
        vector<bool> seen(n, false);
        seen[removed] = true;
        int largest = 0;
        for (int start = 0; start < n; start++) {
            if (seen[start]) continue;
            vector<int> stack = {start};
            seen[start] = true;
            int count = 0;
            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                count++;
                for (int to : graph[v]) {
                    if (seen[to]) continue;
                    seen[to] = true;
                    stack.push_back(to);
                }
            }
            largest = max(largest, count);
        }
        if (largest <= n / 2) result.push_back(removed);
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
            check(n, edges, brute_centroids(graph));
        }
    }

    int n = 200000;
    vector<pair<int, int>> path;
    path.reserve(n - 1);
    for (int v = 1; v < n; v++) path.emplace_back(v - 1, v);
    check(n, path, {n / 2 - 1, n / 2});
    return 0;
}
