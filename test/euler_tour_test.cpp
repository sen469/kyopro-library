#include <bits/stdc++.h>

#include "kyopro/euler_tour.hpp"

using namespace std;

vector<int> sorted_subtree(const kyopro::euler_tour& tour, int v) {
    auto p = tour.subtree(v);
    vector<int> res;
    for (int i = p.first; i < p.second; i++) {
        res.push_back(tour[i]);
    }
    sort(res.begin(), res.end());
    return res;
}

int main() {
    {
        int n = 5;
        vector<pair<int, int>> edges = {
            {0, 1},
            {0, 2},
            {1, 3},
            {1, 4},
        };
        kyopro::euler_tour tour(n, edges);
        tour.build(0);

        assert(tour.size() == n);
        assert(!tour.empty());
        assert(tour.order() == vector<int>({0, 1, 3, 4, 2}));

        assert(tour.in(0) == 0);
        assert(tour.out(0) == 5);
        assert(tour.subtree(1) == make_pair(1, 4));
        assert(sorted_subtree(tour, 0) == vector<int>({0, 1, 2, 3, 4}));
        assert(sorted_subtree(tour, 1) == vector<int>({1, 3, 4}));
        assert(sorted_subtree(tour, 2) == vector<int>({2}));

        assert(tour.parent(0) == -1);
        assert(tour.parent(1) == 0);
        assert(tour.parent(3) == 1);
        assert(tour.parent_edge(0) == -1);
        assert(tour.parent_edge(1) == 0);
        assert(tour.parent_edge(3) == 2);
        assert(tour.depth(0) == 0);
        assert(tour.depth(3) == 2);

        assert(tour.is_ancestor(0, 4));
        assert(tour.is_ancestor(1, 4));
        assert(!tour.is_ancestor(2, 4));
        assert(!tour.is_ancestor(4, 1));
        assert(tour.same_tree(2, 4));
    }

    {
        vector<vector<int>> graph(6);
        auto add_edge = [&](int u, int v) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        };
        add_edge(0, 1);
        add_edge(1, 2);
        add_edge(3, 4);

        kyopro::euler_tour tour(graph);
        tour.build(3);

        assert(tour.order() == vector<int>({3, 4, 0, 1, 2, 5}));
        assert(tour.parent(3) == -1);
        assert(tour.parent(4) == 3);
        assert(tour.parent(0) == -1);
        assert(tour.parent(5) == -1);
        assert(tour.same_tree(3, 4));
        assert(tour.same_tree(0, 2));
        assert(!tour.same_tree(2, 3));
        assert(!tour.is_ancestor(3, 2));
        assert(sorted_subtree(tour, 0) == vector<int>({0, 1, 2}));
        assert(sorted_subtree(tour, 5) == vector<int>({5}));
    }

    {
        vector<vector<kyopro::euler_tour_edge>> graph(3);
        graph[0].push_back({1, 10});
        graph[1].push_back({0, 10});
        graph[1].push_back({2, 20});
        graph[2].push_back({1, 20});

        kyopro::euler_tour tour(graph);
        tour.build();
        assert(tour.parent_edge(1) == 10);
        assert(tour.parent_edge(2) == 20);
        assert(tour.subtree(1) == make_pair(1, 3));
    }

    {
        kyopro::euler_tour tour(0);
        tour.build();
        assert(tour.size() == 0);
        assert(tour.empty());
        assert(tour.order().empty());
    }

    return 0;
}
