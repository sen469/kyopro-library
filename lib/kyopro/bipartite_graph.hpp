#ifndef KYOPRO_BIPARTITE_GRAPH_HPP
#define KYOPRO_BIPARTITE_GRAPH_HPP

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

namespace kyopro {

class bipartite_graph {
private:
    int n_;
    std::vector<std::vector<int>> graph_;
    mutable std::vector<int> colors_;
    mutable bool checked_;
    mutable bool bipartite_;

    void build() const {
        if (checked_) return;

        colors_.assign(n_, -1);
        bipartite_ = true;
        std::queue<int> que;

        for (int root = 0; root < n_ && bipartite_; root++) {
            if (colors_[root] != -1) continue;
            colors_[root] = 0;
            que.push(root);

            while (!que.empty() && bipartite_) {
                int v = que.front();
                que.pop();
                for (int to : graph_[v]) {
                    if (colors_[to] == -1) {
                        colors_[to] = colors_[v] ^ 1;
                        que.push(to);
                    } else if (colors_[to] == colors_[v]) {
                        bipartite_ = false;
                        break;
                    }
                }
            }
        }
        checked_ = true;
    }

public:
    bipartite_graph() : n_(0), checked_(false), bipartite_(true) {}

    explicit bipartite_graph(int n) : n_(n), graph_(n), checked_(false), bipartite_(true) {
        assert(0 <= n);
    }

    explicit bipartite_graph(const std::vector<std::vector<int>>& graph)
        : n_((int)graph.size()), graph_(graph), checked_(false), bipartite_(true) {
        for (int v = 0; v < n_; v++) {
            for (int to : graph_[v]) {
                assert(0 <= to && to < n_);
            }
        }
    }

    bipartite_graph(int n, const std::vector<std::pair<int, int>>& edges)
        : bipartite_graph(n) {
        for (auto [u, v] : edges) add_edge(u, v);
    }

    void add_edge(int u, int v) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        graph_[u].push_back(v);
        graph_[v].push_back(u);
        checked_ = false;
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    bool is_bipartite() const {
        build();
        return bipartite_;
    }

    int color(int v) const {
        assert(0 <= v && v < n_);
        build();
        assert(bipartite_);
        return colors_[v];
    }

    const std::vector<int>& colors() const {
        build();
        assert(bipartite_);
        return colors_;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_BIPARTITE_GRAPH_HPP
