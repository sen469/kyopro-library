#include <bits/stdc++.h>

#include "atcoder/segtree.hpp"
#include "kyopro/heavy_light_decomposition.hpp"

using namespace std;

vector<int> expand(const kyopro::hld& h, int u, int v, bool edge = false) {
    vector<int> result;
    auto segments = h.path(u, v, edge);
    for (auto s : segments) {
        assert(0 <= s.l && s.l < s.r && s.r <= h.size());
        if (s.reversed) {
            for (int i = s.r - 1; i >= s.l; i--) result.push_back(h.get(i));
        } else {
            for (int i = s.l; i < s.r; i++) result.push_back(h[i]);
        }
    }
    return result;
}

void verify(const vector<vector<int>>& graph, const vector<int>& roots) {
    int n = (int)graph.size();
    kyopro::hld h(graph);
    h.build(roots);
    vector<int> parent(n, -1), depth(n, 0), component(n, -1);
    vector<int> starts = roots;
    for (int v = 0; v < n; v++) starts.push_back(v);
    int comp = 0;
    for (int root : starts) {
        if (component[root] != -1) continue;
        queue<int> q;
        q.push(root);
        component[root] = comp++;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : graph[v]) {
                if (component[to] != -1) continue;
                parent[to] = v;
                depth[to] = depth[v] + 1;
                component[to] = component[v];
                q.push(to);
            }
        }
    }
    auto is_ancestor = [&](int a, int v) {
        while (v != -1 && v != a) v = parent[v];
        return v == a;
    };
    vector<int> sorted = h.order();
    sort(sorted.begin(), sorted.end());
    for (int v = 0; v < n; v++) {
        assert(sorted[v] == v);
        assert(h.get(h.in(v)) == v);
        assert(h.parent(v) == parent[v]);
        assert(h.depth(v) == depth[v]);
        assert(h.component(v) == component[v]);
        assert(is_ancestor(h.head(v), v));
        assert(h.in(v) - h.in(h.head(v)) == depth[v] - depth[h.head(v)]);
        auto interval = h.subtree(v);
        assert(interval == make_pair(h.in(v), h.out(v)));
        assert(h.subtree(v, true) == make_pair(interval.first + 1, interval.second));
        for (int u = 0; u < n; u++) {
            bool inside = interval.first <= h.in(u) && h.in(u) < interval.second;
            assert(inside == is_ancestor(v, u));
            assert(h.is_ancestor(v, u) == inside);
        }
        int a = v;
        for (int k = 0; k <= depth[v] + 1; k++) {
            assert(h.kth_ancestor(v, k) == a);
            if (a != -1) a = parent[a];
        }
        assert(h.kth_ancestor(v, LLONG_MAX) == -1);
    }
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            assert(h.same_tree(u, v) == (component[u] == component[v]));
            if (component[u] != component[v]) {
                assert(h.lca(u, v) == -1 && h.dist(u, v) == -1);
                assert(h.kth_on_path(u, v, 0) == -1);
                continue;
            }
            int w = u;
            while (!is_ancestor(w, v)) w = parent[w];
            vector<int> expected, tail;
            for (int x = u; x != w; x = parent[x]) expected.push_back(x);
            expected.push_back(w);
            for (int x = v; x != w; x = parent[x]) tail.push_back(x);
            expected.insert(expected.end(), tail.rbegin(), tail.rend());
            assert(h.lca(u, v) == w);
            assert(h.dist(u, v) == (int)expected.size() - 1);
            assert(expand(h, u, v) == expected);
            for (int k = 0; k < (int)expected.size(); k++) {
                assert(h.kth_on_path(u, v, k) == expected[k]);
            }
            assert(h.kth_on_path(u, v, expected.size()) == -1);
            assert(h.kth_on_path(u, v, LLONG_MAX) == -1);
            expected.erase(find(expected.begin(), expected.end(), w));
            assert(expand(h, u, v, true) == expected);
        }
    }
}

long long sum_op(long long a, long long b) { return a + b; }
long long sum_e() { return 0; }

int main() {
    kyopro::hld empty;
    empty.build();
    assert(empty.empty() && empty.size() == 0 && empty.order().empty());
    empty.build(vector<int>{});
    verify({}, {});
    verify({{}}, {0});
    verify({{1, 2, 3}, {0}, {0}, {0}}, {2});
    verify({{1}, {0}, {}, {4}, {3}}, {4, 1, 4});

    mt19937 rng(469);
    for (int n = 1; n <= 30; n++) {
        for (int trial = 0; trial < 15; trial++) {
            vector<vector<int>> graph(n);
            for (int v = 1; v < n; v++) {
                if (trial % 2 && rng() % 3 == 0) continue;
                int u = rng() % v;
                graph[u].push_back(v);
                graph[v].push_back(u);
            }
            for (auto& adj : graph) shuffle(adj.begin(), adj.end(), rng);
            verify(graph, {int(rng() % n)});
        }
    }
    {
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {1, 3}, {3, 4}};
        kyopro::heavy_light_decomposition h(5, edges);
        h.build();
        vector<long long> values(5);
        for (int v = 0; v < 5; v++) values[h.in(v)] = v + 1;
        atcoder::segtree<long long, sum_op, sum_e> seg(values);
        long long sum = 0;
        for (auto s : h.path(2, 4)) sum += seg.prod(s.l, s.r);
        assert(sum == 14);
        auto interval = h.subtree(3);
        assert(seg.prod(interval.first, interval.second) == 9);
        seg.set(h.in(3), 10);
        sum = 0;
        for (auto s : h.path(2, 4)) sum += seg.prod(s.l, s.r);
        assert(sum == 20);
        h.build(4);
        assert(h.parent(4) == -1 && h.lca(0, 2) == 1);
        assert(expand(h, 2, 4) == vector<int>({2, 1, 3, 4}));
    }
    {
        kyopro::hld h(3);
        h.add_edge(0, 1);
        h.build();
        assert(!h.same_tree(0, 2));
        h.add_edge(1, 2);
        h.build(2);
        assert(h.same_tree(0, 2) && h.parent(0) == 1);
    }
    {
        int n = 200000;
        kyopro::hld h(n);
        for (int v = 1; v < n; v++) h.add_edge(v - 1, v);
        h.build();
        assert(h.path(0, n - 1).size() == 1);
        assert(h.dist(0, n - 1) == n - 1);
        assert(h.kth_ancestor(n - 1, n - 1) == 0);
        assert(h.subtree(0) == make_pair(0, n));
        h.build(n - 1);
        assert(h.kth_on_path(0, n - 1, n - 1) == n - 1);
    }
}
