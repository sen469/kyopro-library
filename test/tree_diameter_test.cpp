#include <bits/stdc++.h>

#include "kyopro/tree_diameter.hpp"

using namespace std;

int main() {
    {
        vector<vector<kyopro::tree_diameter_edge<long long>>> graph(6);
        auto add_edge = [&](int u, int v, long long cost) {
            graph[u].push_back({v, cost});
            graph[v].push_back({u, cost});
        };
        add_edge(0, 1, 3);
        add_edge(1, 2, 4);
        add_edge(1, 3, 2);
        add_edge(3, 4, 6);
        add_edge(3, 5, 1);

        auto res = kyopro::tree_diameter(graph).build();
        assert(res.diameter == 12);
        assert(res.from == 4);
        assert(res.to == 2);
        assert(res.path == vector<int>({4, 3, 1, 2}));
        assert(res.center == vector<int>({3}));
    }

    {
        int n = 5;
        vector<tuple<int, int, int>> edges = {
            {0, 1, 1},
            {1, 2, 2},
            {1, 3, 10},
            {3, 4, 3},
        };

        auto res = kyopro::tree_diameter(n, edges).build();
        assert(res.diameter == 15);
        assert(res.from == 4);
        assert(res.to == 2);
        assert(res.path == vector<int>({4, 3, 1, 2}));
        assert(res.center == vector<int>({3, 1}));
    }

    {
        vector<vector<kyopro::tree_diameter_edge<int>>> graph(1);
        auto res = kyopro::tree_diameter(graph).build();
        assert(res.diameter == 0);
        assert(res.from == 0);
        assert(res.to == 0);
        assert(res.path == vector<int>({0}));
        assert(res.center == vector<int>({0}));
    }

    {
        vector<vector<kyopro::tree_diameter_edge<int>>> graph;
        auto res = kyopro::tree_diameter(graph).build();
        assert(res.diameter == 0);
        assert(res.from == -1);
        assert(res.to == -1);
        assert(res.path.empty());
        assert(res.center.empty());
    }

    {
        const kyopro::tree_diameter<int> empty;
        assert(empty.build().center.empty());
        const kyopro::tree_diameter<int> single(1);
        assert(single.build().center == vector<int>({0}));
    }

    {
        kyopro::tree_diameter<int> tree(3);
        tree.add_edge(0, 1);
        tree.add_edge(1, 2);
        const auto result = tree.build();
        const auto& center = result.center;
        assert(center == vector<int>({1}));
        // Results own their center information; replacing the tree does not affect it.
        tree = kyopro::tree_diameter<int>(1);
        assert(tree.build().center == vector<int>({0}));
        assert(center == vector<int>({1}));
        assert(result.path == vector<int>({2, 1, 0}));
    }

    {
        kyopro::tree_diameter<int> odd_vertices(5);
        for (int v = 1; v < 5; v++) odd_vertices.add_edge(v - 1, v);
        assert(odd_vertices.build().center == vector<int>({2}));

        kyopro::tree_diameter<int> even_vertices(4);
        for (int v = 1; v < 4; v++) even_vertices.add_edge(v - 1, v);
        auto edge = even_vertices.build().center;
        sort(edge.begin(), edge.end());
        assert(edge == vector<int>({1, 2}));

        kyopro::tree_diameter<int> star(6);
        for (int v = 0; v < 6; v++) if (v != 3) star.add_edge(3, v);
        assert(star.build().center == vector<int>({3}));
    }

    {
        // Weighted midpoint differs from the middle vertex of the path.
        kyopro::tree_diameter<long long> weighted(3);
        weighted.add_edge(0, 1, 2);
        weighted.add_edge(1, 2, 8);
        auto edge = weighted.build().center;
        sort(edge.begin(), edge.end());
        assert(edge == vector<int>({1, 2}));

        kyopro::tree_diameter<int> on_vertex(4);
        on_vertex.add_edge(0, 1, 5);
        on_vertex.add_edge(1, 2, 2);
        on_vertex.add_edge(2, 3, 3);
        assert(on_vertex.build().center == vector<int>({1}));

        kyopro::tree_diameter<int> odd_length(3);
        odd_length.add_edge(0, 1, 2);
        odd_length.add_edge(1, 2, 3);
        edge = odd_length.build().center;
        sort(edge.begin(), edge.end());
        assert(edge == vector<int>({1, 2}));
    }

    {
        kyopro::tree_diameter<int> zero(4);
        zero.add_edge(0, 1, 0);
        zero.add_edge(1, 2, 0);
        zero.add_edge(2, 3, 0);
        assert(zero.build().center == vector<int>({0}));

        kyopro::tree_diameter<int> mixed(4);
        mixed.add_edge(0, 1, 2);
        mixed.add_edge(1, 2, 0);
        mixed.add_edge(2, 3, 2);
        auto center = mixed.build().center;
        assert(center.size() == 1);
        assert(center[0] == 1 || center[0] == 2);
    }

    {
        // Do not overflow by calculating diameter * 2 or distance * 2.
        long long maximum = numeric_limits<long long>::max();
        kyopro::tree_diameter<long long> large(3);
        large.add_edge(0, 1, maximum / 2);
        large.add_edge(1, 2, maximum - maximum / 2);
        auto edge = large.build().center;
        sort(edge.begin(), edge.end());
        assert(edge == vector<int>({1, 2}));

        kyopro::tree_diameter<unsigned long long> unsigned_tree(2);
        unsigned_tree.add_edge(0, 1, numeric_limits<unsigned long long>::max());
        auto unsigned_edge = unsigned_tree.build().center;
        sort(unsigned_edge.begin(), unsigned_edge.end());
        assert(unsigned_edge == vector<int>({0, 1}));

        kyopro::tree_diameter<double> floating(3);
        floating.add_edge(0, 1, 0.5);
        floating.add_edge(1, 2, 0.5);
        assert(floating.build().center == vector<int>({1}));
    }

    {
        // Compare unit-weight centers with all minimum-eccentricity vertices.
        mt19937 rng(469);
        for (int n = 1; n <= 35; n++) {
            for (int trial = 0; trial < 10; trial++) {
                kyopro::tree_diameter<int> tree(n);
                vector<vector<int>> graph(n);
                for (int v = 1; v < n; v++) {
                    int p = rng() % v;
                    tree.add_edge(p, v);
                    graph[p].push_back(v);
                    graph[v].push_back(p);
                }
                vector<int> eccentricity(n, 0);
                for (int s = 0; s < n; s++) {
                    vector<int> distance(n, -1), queue = {s};
                    distance[s] = 0;
                    for (size_t i = 0; i < queue.size(); i++) {
                        int v = queue[i];
                        for (int to : graph[v]) if (distance[to] == -1) {
                            distance[to] = distance[v] + 1;
                            queue.push_back(to);
                        }
                    }
                    eccentricity[s] = *max_element(distance.begin(), distance.end());
                }
                vector<int> expected;
                int radius = *min_element(eccentricity.begin(), eccentricity.end());
                for (int v = 0; v < n; v++) if (eccentricity[v] == radius) expected.push_back(v);
                const auto& view = tree;
                const auto result = view.build();
                auto actual = result.center;
                sort(actual.begin(), actual.end());
                assert(actual == expected);
                auto rebuilt = view.build();
                assert(rebuilt.path == result.path);
                assert(rebuilt.center == result.center);
            }
        }
    }

    return 0;
}
