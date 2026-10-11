#ifndef KYOPRO_HEAVY_LIGHT_DECOMPOSITION_HPP
#define KYOPRO_HEAVY_LIGHT_DECOMPOSITION_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace kyopro {

struct hld_path_segment {
    int l;
    int r;
    bool reversed;
};

class heavy_light_decomposition {
    int n_;
    bool built_ = false;
    std::vector<std::vector<int>> graph_;
    std::vector<int> parent_, depth_, component_, subtree_size_, heavy_, head_, in_, order_;

    void check_vertex(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        (void)v;
    }

public:
    explicit heavy_light_decomposition(int n = 0) : n_(n) {
        assert(0 <= n);
        graph_.resize(n);
    }

    explicit heavy_light_decomposition(const std::vector<std::vector<int>>& graph)
        : n_((int)graph.size()), graph_(graph) {}

    heavy_light_decomposition(int n, const std::vector<std::pair<int, int>>& edges)
        : heavy_light_decomposition(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }

    void add_edge(int u, int v) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        assert(u != v);
        graph_[u].push_back(v);
        graph_[v].push_back(u);
        built_ = false;
    }

    void build(int root = 0) {
        if (n_ == 0) {
            assert(root == 0);
            build(std::vector<int>{});
        } else {
            build(std::vector<int>{root});
        }
    }

    void build(const std::vector<int>& roots) {
        built_ = false;
        parent_.assign(n_, -1);
        depth_.assign(n_, 0);
        component_.assign(n_, -1);
        subtree_size_.assign(n_, 1);
        heavy_.assign(n_, -1);
        head_.assign(n_, -1);
        in_.assign(n_, -1);
        order_.clear();
        order_.reserve(n_);
        std::vector<int> traversal, forest_roots;
        traversal.reserve(n_);
        auto visit = [&](int root) {
            assert(0 <= root && root < n_);
            if (component_[root] != -1) return;
            int comp = (int)forest_roots.size();
            forest_roots.push_back(root);
            int start = (int)traversal.size();
            component_[root] = comp;
            traversal.push_back(root);
            for (int i = start; i < (int)traversal.size(); i++) {
                int v = traversal[i];
                for (int to : graph_[v]) {
                    assert(0 <= to && to < n_);
                    if (to == parent_[v]) continue;
                    assert(component_[to] == -1);
                    component_[to] = comp;
                    parent_[to] = v;
                    depth_[to] = depth_[v] + 1;
                    traversal.push_back(to);
                }
            }
        };
        for (int root : roots) visit(root);
        for (int v = 0; v < n_; v++) {
            if (component_[v] == -1) visit(v);
        }

        for (int i = n_ - 1; i >= 0; i--) {
            int v = traversal[i];
            for (int to : graph_[v]) {
                if (to == parent_[v]) continue;
                subtree_size_[v] += subtree_size_[to];
                if (heavy_[v] == -1 || subtree_size_[heavy_[v]] < subtree_size_[to]) heavy_[v] = to;
            }
        }

        // A heavy-first DFS keeps both chains and subtrees contiguous.
        std::vector<std::pair<int, int>> stack;
        for (auto it = forest_roots.rbegin(); it != forest_roots.rend(); ++it) stack.push_back({*it, *it});
        while (!stack.empty()) {
            int v = stack.back().first, head = stack.back().second;
            stack.pop_back();
            head_[v] = head;
            in_[v] = (int)order_.size();
            order_.push_back(v);
            for (auto it = graph_[v].rbegin(); it != graph_[v].rend(); ++it) {
                int to = *it;
                if (to != parent_[v] && to != heavy_[v]) stack.push_back({to, to});
            }
            if (heavy_[v] != -1) stack.push_back({heavy_[v], head});
        }
        built_ = true;
    }

    int size() const { return n_; }
    bool empty() const { return n_ == 0; }
    int in(int v) const { check_vertex(v); return in_[v]; }
    int out(int v) const { check_vertex(v); return in_[v] + subtree_size_[v]; }
    int parent(int v) const { check_vertex(v); return parent_[v]; }
    int depth(int v) const { check_vertex(v); return depth_[v]; }
    int component(int v) const { check_vertex(v); return component_[v]; }
    int head(int v) const { check_vertex(v); return head_[v]; }

    int get(int k) const {
        assert(built_);
        assert(0 <= k && k < n_);
        return order_[k];
    }
    int operator[](int k) const { return get(k); }
    const std::vector<int>& order() const { assert(built_); return order_; }

    bool same_tree(int u, int v) const {
        check_vertex(u);
        check_vertex(v);
        return component_[u] == component_[v];
    }

    bool is_ancestor(int ancestor, int v) const {
        return same_tree(ancestor, v) && in_[ancestor] <= in_[v] && in_[v] < out(ancestor);
    }

    std::pair<int, int> subtree(int v, bool edge = false) const {
        check_vertex(v);
        return {in_[v] + (edge ? 1 : 0), out(v)};
    }

    int lca(int u, int v) const {
        if (!same_tree(u, v)) return -1;
        while (head_[u] != head_[v]) {
            if (depth_[head_[u]] < depth_[head_[v]]) std::swap(u, v);
            u = parent_[head_[u]];
        }
        return depth_[u] < depth_[v] ? u : v;
    }

    int dist(int u, int v) const {
        int w = lca(u, v);
        return w == -1 ? -1 : (depth_[u] - depth_[w]) + (depth_[v] - depth_[w]);
    }

    int kth_ancestor(int v, long long k) const {
        check_vertex(v);
        assert(0 <= k);
        if (k > depth_[v]) return -1;
        while (true) {
            int length = depth_[v] - depth_[head_[v]];
            if (k <= length) return order_[in_[v] - (int)k];
            k -= length + 1;
            v = parent_[head_[v]];
        }
    }

    int kth_on_path(int u, int v, long long k) const {
        assert(0 <= k);
        int w = lca(u, v);
        if (w == -1) return -1;
        int up = depth_[u] - depth_[w], down = depth_[v] - depth_[w];
        if (k > up + down) return -1;
        return k <= up ? kth_ancestor(u, k) : kth_ancestor(v, up + down - k);
    }

    std::vector<hld_path_segment> path(int u, int v, bool edge = false) const {
        check_vertex(u);
        check_vertex(v);
        assert(component_[u] == component_[v]);
        std::vector<hld_path_segment> left, right;
        while (head_[u] != head_[v]) {
            if (depth_[head_[u]] >= depth_[head_[v]]) {
                left.push_back({in_[head_[u]], in_[u] + 1, true});
                u = parent_[head_[u]];
            } else {
                right.push_back({in_[head_[v]], in_[v] + 1, false});
                v = parent_[head_[v]];
            }
        }
        int skip = edge ? 1 : 0;
        if (depth_[u] >= depth_[v]) {
            if (in_[v] + skip < in_[u] + 1) left.push_back({in_[v] + skip, in_[u] + 1, true});
        } else {
            if (in_[u] + skip < in_[v] + 1) left.push_back({in_[u] + skip, in_[v] + 1, false});
        }
        left.insert(left.end(), right.rbegin(), right.rend());
        return left;
    }
};

using hld = heavy_light_decomposition;

}  // namespace kyopro

#endif  // KYOPRO_HEAVY_LIGHT_DECOMPOSITION_HPP
