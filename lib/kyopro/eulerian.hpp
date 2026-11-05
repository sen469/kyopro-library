#ifndef KYOPRO_EULERIAN_HPP
#define KYOPRO_EULERIAN_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

namespace kyopro {

struct eulerian_result {
    bool exists;
    std::vector<int> vertices;
    std::vector<int> edge_ids;
};

template <bool Directed = true>
class eulerian {
    std::vector<std::vector<std::pair<int, int>>> graph_;
    std::vector<int> indegree_;
    int edge_count_ = 0;

    eulerian_result find(int start, bool closed) const {
        assert(-1 <= start && start < size());
        if (size() == 0) return {true, {}, {}};
        if (edge_count_ == 0) return {true, {start == -1 ? 0 : start}, {}};

        int required_start = -1;
        if constexpr (Directed) {
            int positive = 0, negative = 0;
            for (int v = 0; v < size(); v++) {
                long long difference = static_cast<long long>(graph_[v].size()) - indegree_[v];
                if (difference == 1) {
                    positive++;
                    required_start = v;
                } else if (difference == -1) {
                    negative++;
                } else if (difference != 0) {
                    return {false, {}, {}};
                }
            }
            if (!((positive == 0 && negative == 0) ||
                  (!closed && positive == 1 && negative == 1))) return {false, {}, {}};
            if (required_start != -1 && start != -1 && start != required_start) {
                return {false, {}, {}};
            }
        } else {
            int odd = 0;
            for (int v = 0; v < size(); v++) {
                if (graph_[v].size() % 2 != 0) {
                    if (required_start == -1) required_start = v;
                    odd++;
                }
            }
            if (!(odd == 0 || (!closed && odd == 2))) return {false, {}, {}};
            if (odd == 2 && start != -1 && graph_[start].size() % 2 == 0) {
                return {false, {}, {}};
            }
        }

        if (start == -1) {
            start = required_start;
            if (start == -1) {
                start = 0;
                while (graph_[start].empty()) start++;
            }
        }
        if (graph_[start].empty()) return {false, {}, {}};

        std::vector<std::size_t> next(size(), 0);
        std::vector<unsigned char> used(edge_count_, 0);
        // Each stack entry holds a vertex and the edge used to enter it.
        std::vector<std::pair<int, int>> stack = {{start, -1}};
        eulerian_result result = {true, {}, {}};
        while (!stack.empty()) {
            int v = stack.back().first;
            auto& index = next[v];
            while (index < graph_[v].size() && used[graph_[v][index].second]) index++;
            if (index == graph_[v].size()) {
                result.vertices.push_back(v);
                int edge = stack.back().second;
                if (edge != -1) result.edge_ids.push_back(edge);
                stack.pop_back();
            } else {
                auto edge = graph_[v][index++];
                used[edge.second] = 1;
                stack.push_back(edge);
            }
        }
        if (result.edge_ids.size() != static_cast<std::size_t>(edge_count_)) {
            return {false, {}, {}};
        }
        std::reverse(result.vertices.begin(), result.vertices.end());
        std::reverse(result.edge_ids.begin(), result.edge_ids.end());
        return result;
    }

public:
    explicit eulerian(int n = 0) {
        assert(0 <= n);
        graph_.resize(n);
        indegree_.assign(n, 0);
    }

    explicit eulerian(const std::vector<std::vector<int>>& graph) {
        assert(graph.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        graph_.resize(graph.size());
        indegree_.assign(graph.size(), 0);
        for (int v = 0; v < size(); v++) {
            for (int to : graph[v]) {
                assert(0 <= to && to < size());
                if (Directed || v <= to) add_edge(v, to);
            }
        }
    }

    eulerian(int n, const std::vector<std::pair<int, int>>& edges) : eulerian(n) {
        for (auto edge : edges) add_edge(edge.first, edge.second);
    }

    int add_edge(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(edge_count_ < std::numeric_limits<int>::max());
        int id = edge_count_++;
        graph_[u].push_back({v, id});
        if constexpr (Directed) indegree_[v]++;
        else graph_[v].push_back({u, id});
        return id;
    }

    int size() const { return static_cast<int>(graph_.size()); }

    int edge_count() const { return edge_count_; }

    eulerian_result path(int start = -1) const { return find(start, false); }

    eulerian_result cycle(int start = -1) const { return find(start, true); }
};

}  // namespace kyopro

#endif  // KYOPRO_EULERIAN_HPP
