#ifndef KYOPRO_DIRECTED_MST_HPP
#define KYOPRO_DIRECTED_MST_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
struct directed_mst_result {
    bool exists;
    T cost;
    std::vector<int> edge_ids;
    std::vector<int> parents;

    int parent(int v) const {
        assert(exists);
        assert(0 <= v && v < (int)parents.size());
        return parents[v];
    }
};

template <class T>
class directed_mst {
    struct heap_node {
        T cost, lazy;
        int left, right, rank, id;
    };

    struct incoming_heap {
        std::vector<heap_node> nodes;
        std::vector<int> path;

        explicit incoming_heap(int m) {
            nodes.reserve((std::size_t)m + 1);
            nodes.push_back({T(0), T(0), 0, 0, 0, -1});
        }

        int make(T cost, int id) {
            int index = (int)nodes.size();
            nodes.push_back({cost, T(0), 0, 0, 1, id});
            return index;
        }

        void subtract(int v, T value) {
            if (v == 0) return;
            nodes[v].cost = nodes[v].cost - value;
            nodes[v].lazy = nodes[v].lazy + value;
        }

        void push(int v) {
            subtract(nodes[v].left, nodes[v].lazy);
            subtract(nodes[v].right, nodes[v].lazy);
            nodes[v].lazy = T(0);
        }

        int meld(int a, int b) {
            path.clear();
            while (a != 0 && b != 0) {
                if (nodes[b].cost < nodes[a].cost ||
                    (!(nodes[a].cost < nodes[b].cost) && nodes[b].id < nodes[a].id)) {
                    std::swap(a, b);
                }
                push(a);
                path.push_back(a);
                a = nodes[a].right;
            }
            int result = a == 0 ? b : a;
            for (int i = (int)path.size() - 1; i >= 0; i--) {
                int v = path[i];
                nodes[v].right = result;
                if (nodes[nodes[v].left].rank < nodes[nodes[v].right].rank) {
                    std::swap(nodes[v].left, nodes[v].right);
                }
                nodes[v].rank = nodes[nodes[v].right].rank + 1;
                result = v;
            }
            return result;
        }

        int pop(int v) {
            push(v);
            return meld(nodes[v].left, nodes[v].right);
        }
    };

    int n_;
    std::vector<std::tuple<int, int, T>> edges_;

public:
    explicit directed_mst(int n = 0) : n_(n) {
        assert(0 <= n && n <= std::numeric_limits<int>::max() / 2);
    }

    directed_mst(int n, const std::vector<std::tuple<int, int, T>>& edges)
        : directed_mst(n) {
        assert(edges.size() <= (std::size_t)std::numeric_limits<int>::max());
        edges_.reserve(edges.size());
        for (const auto& edge : edges) {
            add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
    }

    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n_);
        assert(0 <= to && to < n_);
        assert(edges_.size() < (std::size_t)std::numeric_limits<int>::max());
        int id = (int)edges_.size();
        edges_.emplace_back(from, to, cost);
        return id;
    }

    int size() const { return n_; }
    int edge_count() const { return (int)edges_.size(); }
    std::tuple<int, int, T> get_edge(int id) const {
        assert(0 <= id && id < edge_count());
        return edges_[id];
    }

    directed_mst_result<T> build(int root) const {
        assert(n_ == 0 ? root == 0 : (0 <= root && root < n_));
        if (n_ == 0) return {false, T(0), {}, {}};

        int capacity = 2 * n_;
        incoming_heap heap(edge_count());
        std::vector<int> heaps(capacity, 0), uf(capacity), visited(capacity, -1);
        std::vector<int> chosen(capacity, -1), tree_parent(capacity, -1);
        std::vector<int> leaf_count(capacity, 1);
        std::vector<std::vector<int>> children(capacity);
        for (int v = 0; v < capacity; v++) uf[v] = v;
        auto leader = [&](int v) {
            int r = v;
            while (uf[r] != r) r = uf[r];
            while (uf[v] != v) {
                int next = uf[v];
                uf[v] = r;
                v = next;
            }
            return r;
        };
        for (int id = 0; id < edge_count(); id++) {
            int u = std::get<0>(edges_[id]), v = std::get<1>(edges_[id]);
            if (u == v || v == root) continue;
            heaps[v] = heap.meld(heaps[v], heap.make(std::get<2>(edges_[id]), id));
        }

        int count = n_;
        visited[root] = -2;
        for (int start = 0; start < n_; start++) {
            int v = leader(start);
            while (visited[v] == -1) {
                visited[v] = start;
                // Edges internal to a contracted component cannot be incoming edges.
                while (heaps[v] != 0) {
                    int id = heap.nodes[heaps[v]].id;
                    if (leader(std::get<0>(edges_[id])) != v) break;
                    heaps[v] = heap.pop(heaps[v]);
                }
                if (heaps[v] == 0) return {false, T(0), {}, {}};
                int id = heap.nodes[heaps[v]].id;
                T cost = heap.nodes[heaps[v]].cost;
                chosen[v] = id;
                heaps[v] = heap.pop(heaps[v]);
                heap.subtract(heaps[v], cost);
                int from = leader(std::get<0>(edges_[id]));
                if (visited[from] != start) {
                    v = from;
                    continue;
                }

                // Record all cycle members before changing their DSU representatives.
                int cycle = count++;
                leaf_count[cycle] = 0;
                int u = from;
                do {
                    children[cycle].push_back(u);
                    leaf_count[cycle] += leaf_count[u];
                    heaps[cycle] = heap.meld(heaps[cycle], heaps[u]);
                    u = leader(std::get<0>(edges_[chosen[u]]));
                } while (u != from);
                for (int child : children[cycle]) {
                    uf[child] = cycle;
                    tree_parent[child] = cycle;
                }
                v = cycle;
            }
        }

        // Number leaves of the contraction forest to locate the target child in O(1).
        std::vector<int> begin(count), traversal, stack;
        traversal.reserve(count);
        for (int v = count - 1; v >= 0; v--) {
            if (tree_parent[v] == -1) stack.push_back(v);
        }
        int position = 0;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            begin[v] = position;
            traversal.push_back(v);
            if (v < n_) {
                position++;
            } else {
                for (auto it = children[v].rbegin(); it != children[v].rend(); ++it) stack.push_back(*it);
            }
        }

        directed_mst_result<T> result{true, T(0), {}, std::vector<int>(n_, -1)};
        result.edge_ids.reserve(n_ - 1);
        for (int v : traversal) {
            int id = chosen[v];
            if (v < n_) {
                if (id != -1) {
                    result.edge_ids.push_back(id);
                    result.parents[v] = std::get<0>(edges_[id]);
                }
            } else {
                assert(id != -1);
                int target = begin[std::get<1>(edges_[id])];
                for (int child : children[v]) {
                    if (begin[child] <= target && target < begin[child] + leaf_count[child]) {
                        chosen[child] = id;
                        break;
                    }
                }
            }
        }
        std::sort(result.edge_ids.begin(), result.edge_ids.end());
        for (int id : result.edge_ids) result.cost = result.cost + std::get<2>(edges_[id]);
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DIRECTED_MST_HPP
