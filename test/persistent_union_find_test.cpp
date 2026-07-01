#include <bits/stdc++.h>

#include "kyopro/persistent_union_find.hpp"

using namespace std;

int main() {
    {
        kyopro::persistent_union_find uf;
        assert(uf.size() == 0);
        assert(uf.empty());
        assert(uf.group_count() == 0);
    }

    {
        kyopro::persistent_union_find uf(5);
        assert(uf.size() == 5);
        assert(!uf.empty());
        assert(uf.group_count() == 5);
        for (int i = 0; i < 5; i++) {
            assert(uf.leader(i) == i);
            assert(uf.component_size(i) == 1);
        }

        auto [uf1, merged1] = uf.merge(0, 1);
        assert(merged1);
        assert(!uf.same(0, 1));
        assert(uf.group_count() == 5);
        assert(uf1.same(0, 1));
        assert(uf1.component_size(0) == 2);
        assert(uf1.size(0) == 2);
        assert(uf1.group_count() == 4);

        auto [uf2, merged2] = uf1.merge(1, 2);
        assert(merged2);
        assert(uf1.same(0, 1));
        assert(!uf1.same(0, 2));
        assert(uf2.same(0, 2));
        assert(uf2.component_size(2) == 3);
        assert(uf2.group_count() == 3);

        auto [uf3, merged3] = uf2.merge(0, 2);
        assert(!merged3);
        assert(uf3.same(0, 2));
        assert(uf3.group_count() == uf2.group_count());
        assert(uf3.component_size(0) == 3);

        kyopro::persistent_union_find uf4 = uf2.unite(3, 4);
        assert(uf4.same(3, 4));
        assert(!uf2.same(3, 4));
        assert(uf4.group_count() == 2);
    }

    {
        kyopro::persistent_union_find uf(8);
        vector<kyopro::persistent_union_find> versions;
        versions.push_back(uf);

        vector<pair<int, int>> edges = {
            {0, 1}, {2, 3}, {1, 2}, {4, 5}, {6, 7}, {5, 6},
        };
        for (auto [a, b] : edges) {
            versions.push_back(versions.back().unite(a, b));
        }

        assert(!versions[0].same(0, 1));
        assert(versions[1].same(0, 1));
        assert(!versions[2].same(0, 2));
        assert(versions[3].same(0, 3));
        assert(versions[3].component_size(0) == 4);
        assert(!versions[3].same(4, 7));
        assert(versions[6].same(4, 7));
        assert(versions[6].component_size(4) == 4);
        assert(versions[6].group_count() == 2);

        auto groups = versions[6].groups();
        for (auto& g : groups) sort(g.begin(), g.end());
        sort(groups.begin(), groups.end());
        assert(groups == vector<vector<int>>({{0, 1, 2, 3}, {4, 5, 6, 7}}));
    }

    {
        const int n = 40;
        kyopro::persistent_union_find uf(n);
        vector<kyopro::persistent_union_find> versions{uf};
        vector<vector<int>> naive(1, vector<int>(n));
        iota(naive[0].begin(), naive[0].end(), 0);
        mt19937 rng(0);

        auto find_naive = [](vector<int>& parent, int x) {
            while (parent[x] != x) x = parent[x];
            return x;
        };

        for (int step = 0; step < 1000; step++) {
            int version = uniform_int_distribution<int>(0, (int)versions.size() - 1)(rng);
            int a = uniform_int_distribution<int>(0, n - 1)(rng);
            int b = uniform_int_distribution<int>(0, n - 1)(rng);

            auto [next, merged] = versions[version].merge(a, b);
            versions.push_back(next);
            naive.push_back(naive[version]);

            int ra = find_naive(naive.back(), a);
            int rb = find_naive(naive.back(), b);
            assert(merged == (ra != rb));
            if (ra != rb) {
                for (int i = 0; i < n; i++) {
                    if (find_naive(naive.back(), i) == rb) naive.back()[i] = ra;
                }
            }

            for (int iter = 0; iter < 10; iter++) {
                int qv = uniform_int_distribution<int>(0, (int)versions.size() - 1)(rng);
                int x = uniform_int_distribution<int>(0, n - 1)(rng);
                int y = uniform_int_distribution<int>(0, n - 1)(rng);
                int rx = find_naive(naive[qv], x);
                int ry = find_naive(naive[qv], y);
                assert(versions[qv].same(x, y) == (rx == ry));

                int expected_size = 0;
                for (int i = 0; i < n; i++) {
                    if (find_naive(naive[qv], i) == rx) expected_size++;
                }
                assert(versions[qv].component_size(x) == expected_size);
            }
        }
    }

    return 0;
}
