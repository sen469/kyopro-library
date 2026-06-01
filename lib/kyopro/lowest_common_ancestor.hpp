#ifndef KYOPRO_LOWEST_COMMON_ANCESTOR_HPP
#define KYOPRO_LOWEST_COMMON_ANCESTOR_HPP

#include <algorithm>
#include <utility>
#include <vector>

namespace kyopro {

class lowest_common_ancestor {
   private:
    int n_;
    int log_;
    std::vector<std::vector<int>> graph_;
    std::vector<std::vector<int>> parent_;
    std::vector<int> depth_;
    std::vector<int> component_;
    bool built_;

    void init_lift() {
        log_ = 1;
        while ((1 << log_) <= std::max(1, n_)) log_++;
        parent_.assign(log_, std::vector<int>(n_, -1));
        depth_.assign(n_, 0);
        component_.assign(n_, -1);
        built_ = false;
    }

   public:
    lowest_common_ancestor() : n_(0), log_(1), built_(false) {}

    explicit lowest_common_ancestor(int n) : n_(n), graph_(n), built_(false) {
        init_lift();
    }

    explicit lowest_common_ancestor(const std::vector<std::vector<int>>& graph) : n_((int)graph.size()), graph_(graph) {
        init_lift();
    }

    lowest_common_ancestor(int n, const std::vector<std::pair<int, int>>& edges) : n_(n), graph_(n), built_(false) {
        init_lift();
        for (auto [u, v] : edges) {
            add_edge(u, v);
        }
    }

    void add_edge(int u, int v) {
        graph_[u].push_back(v);
        graph_[v].push_back(u);
        built_ = false;
    }

    void build(int root = 0) {
        build(std::vector<int>{root});
    }

    void build(const std::vector<int>& roots) {
        init_lift();

        int comp = 0;
        for (int root : roots) {
            if (component_[root] != -1) continue;

            std::vector<int> stack = {root};
            component_[root] = comp;
            parent_[0][root] = -1;
            depth_[root] = 0;

            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                for (int to : graph_[v]) {
                    if (to == parent_[0][v]) continue;
                    if (component_[to] != -1) continue;
                    component_[to] = comp;
                    parent_[0][to] = v;
                    depth_[to] = depth_[v] + 1;
                    stack.push_back(to);
                }
            }
            comp++;
        }

        for (int root = 0; root < n_; root++) {
            if (component_[root] != -1) continue;

            std::vector<int> stack = {root};
            component_[root] = comp;
            parent_[0][root] = -1;
            depth_[root] = 0;

            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                for (int to : graph_[v]) {
                    if (to == parent_[0][v]) continue;
                    if (component_[to] != -1) continue;
                    component_[to] = comp;
                    parent_[0][to] = v;
                    depth_[to] = depth_[v] + 1;
                    stack.push_back(to);
                }
            }
            comp++;
        }

        for (int k = 0; k + 1 < log_; k++) {
            for (int v = 0; v < n_; v++) {
                if (parent_[k][v] == -1) {
                    parent_[k + 1][v] = -1;
                } else {
                    parent_[k + 1][v] = parent_[k][parent_[k][v]];
                }
            }
        }
        built_ = true;
    }

    int kth_ancestor(int v, long long k) const {
        for (int i = 0; i < log_ && v != -1; i++) {
            if ((k >> i) & 1LL) v = parent_[i][v];
        }
        if (k >> log_) return -1;
        return v;
    }

    int lca(int u, int v) const {
        if (!built_ || component_[u] != component_[v]) return -1;
        if (depth_[u] < depth_[v]) std::swap(u, v);

        u = kth_ancestor(u, depth_[u] - depth_[v]);
        if (u == v) return u;

        for (int k = log_ - 1; k >= 0; k--) {
            if (parent_[k][u] != parent_[k][v]) {
                u = parent_[k][u];
                v = parent_[k][v];
            }
        }
        return parent_[0][u];
    }

    int dist(int u, int v) const {
        int w = lca(u, v);
        if (w == -1) return -1;
        return depth_[u] + depth_[v] - 2 * depth_[w];
    }

    int depth(int v) const {
        return depth_[v];
    }

    int parent(int v) const {
        return parent_[0][v];
    }

    bool same_tree(int u, int v) const {
        return built_ && component_[u] == component_[v];
    }
};

}  // namespace kyopro

#endif  // KYOPRO_LOWEST_COMMON_ANCESTOR_HPP
