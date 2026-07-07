#ifndef KYOPRO_REROOTING_DP_HPP
#define KYOPRO_REROOTING_DP_HPP

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

struct rerooting_edge {
    int to;
    int id;
};

#if __cplusplus >= 201703L

template <class R, class M, auto op, auto e, auto f_ve, auto f_ev>
std::vector<R> rerooting_dp(const std::vector<std::vector<rerooting_edge>>& graph) {
    static_assert(std::is_convertible_v<decltype(op), std::function<M(M, M)>>, "op must work as M(M, M)");
    static_assert(std::is_convertible_v<decltype(e), std::function<M()>>, "e must work as M()");
    static_assert(std::is_convertible_v<decltype(f_ve), std::function<M(R, int)>>,
                  "f_ve must work as M(R, int)");
    static_assert(std::is_convertible_v<decltype(f_ev), std::function<R(M, int)>>,
                  "f_ev must work as R(M, int)");
#else

template <class R, class M, M (*op)(M, M), M (*e)(), M (*f_ve)(R, int), R (*f_ev)(M, int)>
std::vector<R> rerooting_dp(const std::vector<std::vector<rerooting_edge>>& graph) {
#endif

    int n = (int)graph.size();
    if (n == 0) return {};

    std::vector<int> parent(n, -1), order;
    order.reserve(n);
    std::vector<int> stack = {0};
    parent[0] = -2;
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        order.push_back(v);
        for (auto edge : graph[v]) {
            assert(0 <= edge.to && edge.to < n);
            if (edge.to == parent[v]) continue;
            assert(parent[edge.to] == -1);
            parent[edge.to] = v;
            stack.push_back(edge.to);
        }
    }
    assert((int)order.size() == n);

    std::vector<R> dp(n);
    for (int i = n - 1; i >= 0; i--) {
        int v = order[i];
        M acc = e();
        for (auto edge : graph[v]) {
            if (parent[edge.to] != v) continue;
            acc = op(acc, f_ve(dp[edge.to], edge.id));
        }
        dp[v] = f_ev(acc, v);
    }

    std::vector<R> ans(n);
    std::vector<M> parent_contrib(n, e());
    parent[0] = -1;
    for (int v : order) {
        int deg = (int)graph[v].size();
        std::vector<M> prefix(deg + 1, e()), suffix(deg + 1, e());
        for (int i = 0; i < deg; i++) {
            auto edge = graph[v][i];
            M val = (edge.to == parent[v]) ? parent_contrib[v] : f_ve(dp[edge.to], edge.id);
            prefix[i + 1] = op(prefix[i], val);
        }
        for (int i = deg - 1; i >= 0; i--) {
            auto edge = graph[v][i];
            M val = (edge.to == parent[v]) ? parent_contrib[v] : f_ve(dp[edge.to], edge.id);
            suffix[i] = op(val, suffix[i + 1]);
        }

        ans[v] = f_ev(prefix[deg], v);

        for (int i = 0; i < deg; i++) {
            auto edge = graph[v][i];
            if (edge.to == parent[v]) continue;
            M without_child = op(prefix[i], suffix[i + 1]);
            parent_contrib[edge.to] = f_ve(f_ev(without_child, v), edge.id);
        }
    }

    return ans;
}

#if __cplusplus >= 201703L
template <class R, class M, auto op, auto e, auto f_ve, auto f_ev>
#else
template <class R, class M, M (*op)(M, M), M (*e)(), M (*f_ve)(R, int), R (*f_ev)(M, int)>
#endif
std::vector<R> rerooting_dp(int n, const std::vector<std::pair<int, int>>& edges) {
    assert(0 <= n);
    assert((int)edges.size() == (n == 0 ? 0 : n - 1));
    std::vector<std::vector<rerooting_edge>> graph(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        int u = edges[i].first;
        int v = edges[i].second;
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        graph[u].push_back({v, i});
        graph[v].push_back({u, i});
    }
    return rerooting_dp<R, M, op, e, f_ve, f_ev>(graph);
}

}  // namespace kyopro

#endif  // KYOPRO_REROOTING_DP_HPP
