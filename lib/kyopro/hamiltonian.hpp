#ifndef KYOPRO_HAMILTONIAN_HPP
#define KYOPRO_HAMILTONIAN_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

namespace kyopro {

template <bool Directed = true>
class hamiltonian {
    static constexpr int max_vertices = 24;
    std::vector<std::uint32_t> incoming_;
    // Counts saturated at 2: an undirected two-vertex cycle needs parallel edges.
    std::vector<std::vector<unsigned char>> multiplicity_;

    static std::uint32_t bit(int v) { return std::uint32_t(1) << v; }

    static int first_vertex(std::uint32_t mask) {
        assert(mask != 0);
        return __builtin_ctz(mask);
    }

    std::vector<std::uint32_t> reachable(int start) const {
        std::uint32_t states = bit(size());
        std::vector<std::uint32_t> dp(states, 0);
        for (int v = 0; v < size(); v++) {
            if (start == -1 || start == v) dp[bit(v)] = bit(v);
        }
        for (std::uint32_t mask = 1; mask < states; mask++) {
            if (start != -1 && !(mask & bit(start))) continue;
            std::uint32_t vertices = mask;
            while (vertices != 0) {
                int v = first_vertex(vertices);
                vertices &= vertices - 1;
                if (dp[mask ^ bit(v)] & incoming_[v]) dp[mask] |= bit(v);
            }
        }
        return dp;
    }

    std::vector<int> restore(const std::vector<std::uint32_t>& dp, int last) const {
        std::vector<int> result;
        std::uint32_t mask = bit(size()) - 1;
        while (mask != 0) {
            result.push_back(last);
            mask ^= bit(last);
            if (mask != 0) last = first_vertex(dp[mask] & incoming_[last]);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

public:
    explicit hamiltonian(int n = 0) {
        assert(0 <= n && n <= max_vertices);
        incoming_.assign(n, 0);
        multiplicity_.assign(n, std::vector<unsigned char>(n, 0));
    }

    explicit hamiltonian(const std::vector<std::vector<int>>& graph) {
        assert(graph.size() <= static_cast<std::size_t>(max_vertices));
        int n = static_cast<int>(graph.size());
        incoming_.assign(n, 0);
        multiplicity_.assign(n, std::vector<unsigned char>(n, 0));
        for (int v = 0; v < n; v++) {
            for (int to : graph[v]) {
                assert(0 <= to && to < n);
                if (Directed || v <= to) add_edge(v, to);
            }
        }
    }

    hamiltonian(int n, const std::vector<std::pair<int, int>>& edges) : hamiltonian(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }

    void add_edge(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        incoming_[v] |= bit(u);
        if (multiplicity_[u][v] < 2) multiplicity_[u][v]++;
        if (!Directed && u != v) {
            incoming_[u] |= bit(v);
            if (multiplicity_[v][u] < 2) multiplicity_[v][u]++;
        }
    }

    int size() const { return static_cast<int>(incoming_.size()); }

    std::vector<int> path(int start = -1, int goal = -1) const {
        assert(-1 <= start && start < size());
        assert(-1 <= goal && goal < size());
        if (size() == 0) return {};
        if (size() > 1 && start != -1 && start == goal) return {};
        auto dp = reachable(start);
        std::uint32_t ends = dp.back();
        if (goal != -1) ends &= bit(goal);
        if (ends == 0) return {};
        return restore(dp, first_vertex(ends));
    }

    std::vector<int> cycle(int start = 0) const {
        assert(0 <= start && (start < size() || (size() == 0 && start == 0)));
        if (size() == 0) return {};
        if (!Directed && size() == 2 && multiplicity_[0][1] < 2) return {};
        auto dp = reachable(start);
        std::uint32_t ends = dp.back() & incoming_[start];
        if (ends == 0) return {};
        return restore(dp, first_vertex(ends));
    }
};

}  // namespace kyopro

#endif  // KYOPRO_HAMILTONIAN_HPP
