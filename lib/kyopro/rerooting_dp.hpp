#ifndef KYOPRO_REROOTING_DP_HPP
#define KYOPRO_REROOTING_DP_HPP

#include <cassert>
#include <functional>
#include <type_traits>
#include <vector>

namespace kyopro {

#if __cplusplus >= 201703L
template <class R, class M, auto op, auto e, auto f_ve, auto f_ev>
#else
template <class R, class M, M (*op)(M, M), M (*e)(), M (*f_ve)(R, int), R (*f_ev)(M, int)>
#endif
class rerooting_dp {
#if __cplusplus >= 201703L
    static_assert(std::is_convertible_v<decltype(op), std::function<M(M, M)>>, "op must work as M(M, M)");
    static_assert(std::is_convertible_v<decltype(e), std::function<M()>>, "e must work as M()");
    static_assert(std::is_convertible_v<decltype(f_ve), std::function<M(R, int)>>,
                  "f_ve must work as M(R, int)");
    static_assert(std::is_convertible_v<decltype(f_ev), std::function<R(M, int)>>,
                  "f_ev must work as R(M, int)");
#endif

    struct edge {
        int to;
        int id;
        int rev;
    };

    int n_;
    int edge_count_ = 0;
    bool built_ = false;
    std::vector<std::vector<edge>> graph_;
    std::vector<int> parent_, order_;
    std::vector<R> dp_;

public:
    explicit rerooting_dp(int n = 0) : n_(n) {
        assert(0 <= n);
        graph_.resize(n);
    }

    int add_edge(int u, int v, int idx, int xdi) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        assert(u != v);
        assert(edge_count_ < n_ - 1);
        int u_rev = (int)graph_[u].size();
        int v_rev = (int)graph_[v].size();
        graph_[u].push_back({v, idx, v_rev});
        graph_[v].push_back({u, xdi, u_rev});
        built_ = false;
        return edge_count_++;
    }

    int add_edge(int u, int v, int id) { return add_edge(u, v, id, id); }

    int add_edge(int u, int v) { return add_edge(u, v, edge_count_, edge_count_); }

    std::vector<R> build(int root = 0) {
        assert(edge_count_ == (n_ == 0 ? 0 : n_ - 1));
        assert(n_ == 0 ? root == 0 : (0 <= root && root < n_));
        built_ = false;
        parent_.assign(n_, -1);
        order_.clear();
        order_.reserve(n_);
        dp_.clear();
        dp_.resize(n_);
        if (n_ == 0) {
            built_ = true;
            return {};
        }

        parent_[root] = -2;
        order_.push_back(root);
        for (int i = 0; i < (int)order_.size(); i++) {
            int v = order_[i];
            for (const auto& edge : graph_[v]) {
                if (edge.to == parent_[v]) continue;
                assert(parent_[edge.to] == -1);
                parent_[edge.to] = v;
                order_.push_back(edge.to);
            }
        }
        assert((int)order_.size() == n_);

        for (int i = n_ - 1; i >= 0; i--) {
            int v = order_[i];
            M acc = e();
            for (const auto& edge : graph_[v]) {
                if (edge.to == parent_[v]) continue;
                acc = op(acc, f_ve(dp_[edge.to], edge.id));
            }
            dp_[v] = f_ev(acc, v);
        }
        built_ = true;
        return dp_;
    }

    std::vector<R> reroot() const {
        assert(built_);
        std::vector<R> ans(n_);
        std::vector<M> parent_contrib(n_, e());
        for (int v : order_) {
            int deg = (int)graph_[v].size();
            std::vector<M> values(deg, e()), suffix(deg + 1, e());
            for (int i = 0; i < deg; i++) {
                const auto& edge = graph_[v][i];
                values[i] = edge.to == parent_[v] ? parent_contrib[v] : f_ve(dp_[edge.to], edge.id);
            }
            for (int i = deg - 1; i >= 0; i--) {
                suffix[i] = op(values[i], suffix[i + 1]);
            }
            ans[v] = f_ev(suffix[0], v);

            M prefix = e();
            for (int i = 0; i < deg; i++) {
                const auto& edge = graph_[v][i];
                if (edge.to != parent_[v]) {
                    M without_child = op(prefix, suffix[i + 1]);
                    // Use the outgoing edge ID as seen from the receiving child.
                    int reverse_id = graph_[edge.to][edge.rev].id;
                    parent_contrib[edge.to] = f_ve(f_ev(without_child, v), reverse_id);
                }
                prefix = op(prefix, values[i]);
            }
        }
        return ans;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_REROOTING_DP_HPP
