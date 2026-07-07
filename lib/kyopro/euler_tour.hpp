#ifndef KYOPRO_EULER_TOUR_HPP
#define KYOPRO_EULER_TOUR_HPP

#include <cassert>
#include <utility>
#include <vector>

namespace kyopro {

struct euler_tour_edge {
    int to;
    int id;
};

class euler_tour {
private:
    int n_;
    std::vector<std::vector<euler_tour_edge>> graph_;
    std::vector<int> parent_;
    std::vector<int> parent_edge_;
    std::vector<int> depth_;
    std::vector<int> component_;
    std::vector<int> in_;
    std::vector<int> out_;
    std::vector<int> order_;
    bool built_;

    void init_data() {
        parent_.assign(n_, -1);
        parent_edge_.assign(n_, -1);
        depth_.assign(n_, 0);
        component_.assign(n_, -1);
        in_.assign(n_, -1);
        out_.assign(n_, -1);
        order_.clear();
        order_.reserve(n_);
        built_ = false;
    }

    void dfs(int root, int comp) {
        std::vector<std::pair<int, int>> stack;
        stack.push_back({root, 0});
        component_[root] = comp;
        parent_[root] = -1;
        parent_edge_[root] = -1;
        depth_[root] = 0;
        in_[root] = (int)order_.size();
        order_.push_back(root);

        while (!stack.empty()) {
            int v = stack.back().first;
            int& i = stack.back().second;
            if (i == (int)graph_[v].size()) {
                out_[v] = (int)order_.size();
                stack.pop_back();
                continue;
            }

            euler_tour_edge edge = graph_[v][i++];
            assert(0 <= edge.to && edge.to < n_);
            if (edge.to == parent_[v]) continue;
            if (component_[edge.to] != -1) continue;

            component_[edge.to] = comp;
            parent_[edge.to] = v;
            parent_edge_[edge.to] = edge.id;
            depth_[edge.to] = depth_[v] + 1;
            in_[edge.to] = (int)order_.size();
            order_.push_back(edge.to);
            stack.push_back({edge.to, 0});
        }
    }

public:
    euler_tour() : n_(0), built_(false) {}

    explicit euler_tour(int n) : n_(n), graph_(n), built_(false) {
        assert(0 <= n);
        init_data();
    }

    explicit euler_tour(const std::vector<std::vector<int>>& graph)
        : n_((int)graph.size()), graph_(n_), built_(false) {
        init_data();
        for (int v = 0; v < n_; v++) {
            for (int to : graph[v]) {
                assert(0 <= to && to < n_);
                graph_[v].push_back({to, -1});
            }
        }
    }

    explicit euler_tour(const std::vector<std::vector<euler_tour_edge>>& graph)
        : n_((int)graph.size()), graph_(graph), built_(false) {
        init_data();
    }

    euler_tour(int n, const std::vector<std::pair<int, int>>& edges) : n_(n), graph_(n), built_(false) {
        assert(0 <= n);
        init_data();
        for (int i = 0; i < (int)edges.size(); i++) {
            add_edge(edges[i].first, edges[i].second, i);
        }
    }

    void add_edge(int u, int v, int id = -1) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        graph_[u].push_back({v, id});
        graph_[v].push_back({u, id});
        built_ = false;
    }

    void build(int root = 0) {
        if (n_ == 0) {
            init_data();
            built_ = true;
            return;
        }
        build(std::vector<int>{root});
    }

    void build(const std::vector<int>& roots) {
        init_data();

        int comp = 0;
        for (int root : roots) {
            assert(0 <= root && root < n_);
            if (component_[root] != -1) continue;
            dfs(root, comp++);
        }
        for (int root = 0; root < n_; root++) {
            if (component_[root] != -1) continue;
            dfs(root, comp++);
        }
        built_ = true;
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    int in(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return in_[v];
    }

    int out(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return out_[v];
    }

    std::pair<int, int> subtree(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return {in_[v], out_[v]};
    }

    int get(int k) const {
        assert(built_);
        assert(0 <= k && k < n_);
        return order_[k];
    }

    int operator[](int k) const { return get(k); }

    const std::vector<int>& order() const {
        assert(built_);
        return order_;
    }

    int parent(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return parent_[v];
    }

    int parent_edge(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return parent_edge_[v];
    }

    int depth(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return depth_[v];
    }

    int component(int v) const {
        assert(built_);
        assert(0 <= v && v < n_);
        return component_[v];
    }

    bool same_tree(int u, int v) const {
        assert(built_);
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        return component_[u] == component_[v];
    }

    bool is_ancestor(int ancestor, int v) const {
        assert(built_);
        assert(0 <= ancestor && ancestor < n_);
        assert(0 <= v && v < n_);
        return component_[ancestor] == component_[v] && in_[ancestor] <= in_[v] && out_[v] <= out_[ancestor];
    }
};

}  // namespace kyopro

#endif  // KYOPRO_EULER_TOUR_HPP
