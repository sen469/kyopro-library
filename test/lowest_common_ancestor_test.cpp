#include <bits/stdc++.h>

#include "kyopro/lowest_common_ancestor.hpp"

using namespace std;

int main() {
    {
        kyopro::lowest_common_ancestor lca(7);
        lca.add_edge(0, 1);
        lca.add_edge(0, 2);
        lca.add_edge(1, 3);
        lca.add_edge(1, 4);
        lca.add_edge(2, 5);
        lca.add_edge(2, 6);
        lca.build(0);

        assert(lca.lca(3, 4) == 1);
        assert(lca.lca(3, 5) == 0);
        assert(lca.lca(5, 6) == 2);
        assert(lca.lca(0, 6) == 0);
        assert(lca.dist(3, 6) == 4);
        assert(lca.dist(4, 1) == 1);
        assert(lca.kth_ancestor(6, 0) == 6);
        assert(lca.kth_ancestor(6, 1) == 2);
        assert(lca.kth_ancestor(6, 2) == 0);
        assert(lca.kth_ancestor(6, 3) == -1);
        assert(lca.depth(4) == 2);
        assert(lca.parent(4) == 1);
    }

    {
        int n = 6;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {3, 4},
        };
        kyopro::lowest_common_ancestor lca(n, edges);
        lca.build({0, 3});

        assert(lca.lca(0, 2) == 0);
        assert(lca.dist(0, 2) == 2);
        assert(lca.lca(3, 4) == 3);
        assert(lca.same_tree(0, 2));
        assert(!lca.same_tree(2, 4));
        assert(lca.lca(2, 4) == -1);
        assert(lca.dist(2, 4) == -1);
        assert(lca.lca(5, 5) == 5);
        assert(lca.parent(5) == -1);
    }

    {
        vector<vector<int>> graph(4);
        graph[0].push_back(1);
        graph[1].push_back(0);
        graph[1].push_back(2);
        graph[2].push_back(1);
        graph[2].push_back(3);
        graph[3].push_back(2);

        kyopro::lowest_common_ancestor lca(graph);
        lca.build(2);

        assert(lca.lca(0, 3) == 2);
        assert(lca.dist(0, 3) == 3);
        assert(lca.kth_ancestor(0, 2) == 2);
        assert(lca.kth_ancestor(0, 100) == -1);
    }

    return 0;
}
