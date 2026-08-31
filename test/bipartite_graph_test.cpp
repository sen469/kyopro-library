#include <bits/stdc++.h>

#include "kyopro/bipartite_graph.hpp"

using namespace std;

void check_coloring(const kyopro::bipartite_graph& graph,
                    const vector<pair<int, int>>& edges) {
    assert(graph.is_bipartite());
    assert((int)graph.colors().size() == graph.size());
    for (auto [u, v] : edges) {
        assert(graph.color(u) != graph.color(v));
    }
}

int main() {
    {
        kyopro::bipartite_graph graph;
        assert(graph.empty());
        assert(graph.is_bipartite());
        assert(graph.colors().empty());
    }

    {
        int n = 6;
        vector<pair<int, int>> edges = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {4, 5},
        };
        kyopro::bipartite_graph graph(n, edges);
        assert(graph.size() == n);
        check_coloring(graph, edges);
    }

    {
        vector<vector<int>> graph = {
            {1, 3},
            {0, 2},
            {1, 3},
            {0, 2},
        };
        kyopro::bipartite_graph bg(graph);
        assert(bg.is_bipartite());
        for (int v = 0; v < (int)graph.size(); v++) {
            for (int to : graph[v]) assert(bg.color(v) != bg.color(to));
        }
    }

    {
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}};
        kyopro::bipartite_graph graph(3, edges);
        assert(!graph.is_bipartite());
    }

    {
        kyopro::bipartite_graph graph(2);
        graph.add_edge(0, 1);
        assert(graph.is_bipartite());
        graph.add_edge(0, 1);
        assert(graph.is_bipartite());
        graph.add_edge(0, 0);
        assert(!graph.is_bipartite());
    }

    {
        kyopro::bipartite_graph graph(3);
        assert(graph.is_bipartite());
        graph.add_edge(0, 1);
        assert(graph.is_bipartite());
        graph.add_edge(1, 2);
        assert(graph.is_bipartite());
        graph.add_edge(2, 0);
        assert(!graph.is_bipartite());
    }

    return 0;
}
