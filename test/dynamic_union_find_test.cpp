#include <bits/stdc++.h>

#include "kyopro/dynamic_union_find.hpp"

using namespace std;

struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        return (uint64_t)(uint32_t)p.first << 32 ^ (uint32_t)p.second;
    }
};

int main() {
    {
        kyopro::dynamic_union_find<> uf;
        assert(uf.merge(1, 2));
        assert(uf.same(1, 2));
        assert(uf.size(1) == 2);
    }

    {
        kyopro::dynamic_union_find<long long> uf;
        assert(uf.size() == 0);
        assert(uf.group_count() == 0);
        assert(!uf.contains(10));

        assert(uf.add(10) == 0);
        assert(uf.contains(10));
        assert(uf.size() == 1);
        assert(uf.size(10) == 1);
        assert(uf.group_count() == 1);
        assert(uf.index(10) == 0);
        assert(uf.key(0) == 10);
        assert(uf.leader_id(10) == uf.index(10));

        assert(uf.merge(10, 20));
        assert(uf.same(10, 20));
        assert(!uf.same(10, 30));
        assert(uf.size() == 3);
        assert(uf.group_count() == 2);
        assert(uf.size(10) == 2);
        assert(uf.size(20) == 2);
        assert(uf.size(30) == 1);

        assert(uf.merge(20, 30));
        assert(uf.same(10, 30));
        assert(!uf.merge(10, 30));
        assert(uf.size(10) == 3);
        assert(uf.group_count() == 1);
        assert(uf.leader(10) == uf.leader(30));
        assert(uf.key(uf.leader_id(10)) == uf.leader(10));
        assert(uf.leader_id(10) == uf.leader_id(30));

        int before_size = uf.size();
        int new_leader = uf.leader_id(40);
        assert(uf.size() == before_size + 1);
        assert(uf.index(40) == new_leader);
        assert(uf.key(new_leader) == 40);
        assert(uf.size(40) == 1);
    }

    {
        kyopro::dynamic_union_find<string> uf;
        assert(uf.merge("alice", "bob"));
        assert(uf.merge("carol", "dave"));
        assert(!uf.same("alice", "carol"));
        assert(uf.merge("bob", "carol"));
        assert(uf.same("alice", "dave"));
        assert(uf.size("alice") == 4);
        assert(uf.size() == 4);
        assert(uf.group_count() == 1);
    }

    {
        kyopro::dynamic_union_find<int> uf(10);
        vector<pair<int, int>> edges = {
            {0, 1}, {1, 2}, {4, 5}, {10, 11}, {11, 12},
        };
        for (auto edge : edges) {
            assert(uf.merge(edge.first, edge.second));
        }

        assert(uf.size() == 8);
        assert(uf.group_count() == 3);
        assert(uf.same(0, 2));
        assert(!uf.same(0, 4));
        assert(uf.size(0) == 3);
        assert(uf.size(4) == 2);
        assert(uf.size(10) == 3);

        auto groups = uf.groups();
        for (auto& g : groups) sort(g.begin(), g.end());
        sort(groups.begin(), groups.end());
        assert(groups == vector<vector<int>>({{0, 1, 2}, {4, 5}, {10, 11, 12}}));
    }

    {
        kyopro::dynamic_union_find<pair<int, int>, PairHash> uf;
        assert(uf.merge({0, 0}, {1, 0}));
        assert(uf.merge({1, 0}, {1, 1}));
        assert(uf.same({0, 0}, {1, 1}));
        assert(!uf.same({0, 0}, {2, 2}));
        assert(uf.size({0, 0}) == 3);
        assert(uf.size({2, 2}) == 1);
    }

    return 0;
}
