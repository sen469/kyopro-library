#include <bits/stdc++.h>

#include "kyopro/dsu.hpp"

using namespace std;

int main() {
    {
        kyopro::dsu uf(5);
        assert(uf.group_count() == 5);
        assert(uf.size(0) == 1);
        assert(uf.edge_count(0) == 0);
        assert(uf.unique_edge_count(0) == 0);
        assert(!uf.same(0, 1));

        int r = uf.merge(0, 1);
        assert(uf.same(0, 1));
        assert(uf.leader(0) == r);
        assert(uf.size(0) == 2);
        assert(uf.edge_count(0) == 1);
        assert(uf.unique_edge_count(0) == 1);
        assert(uf.group_count() == 4);

        uf.merge(1, 2);
        assert(uf.size(0) == 3);
        assert(uf.edge_count(0) == 2);
        assert(uf.unique_edge_count(0) == 2);
        assert(uf.edge_count(0) == uf.size(0) - 1);
        assert(uf.group_count() == 3);
    }

    {
        kyopro::dsu uf(4);
        assert(uf.group_count() == 4);
        uf.merge(0, 1);
        assert(uf.group_count() == 3);
        uf.merge(0, 1);
        uf.merge(1, 0);
        assert(uf.size(0) == 2);
        assert(uf.edge_count(0) == 3);
        assert(uf.unique_edge_count(0) == 1);
        assert(uf.group_count() == 3);

        uf.merge(0, 0);
        uf.merge(0, 0);
        assert(uf.edge_count(0) == 5);
        assert(uf.unique_edge_count(0) == 2);
        assert(uf.group_count() == 3);
    }

    {
        kyopro::dsu uf(6);
        uf.merge(0, 1);
        uf.merge(1, 2);
        uf.merge(3, 4);
        uf.merge(3, 4);

        assert(uf.edge_count(0) == 2);
        assert(uf.unique_edge_count(0) == 2);
        assert(uf.edge_count(3) == 2);
        assert(uf.unique_edge_count(3) == 1);

        uf.merge(2, 3);
        assert(uf.size(0) == 5);
        assert(uf.edge_count(0) == 5);
        assert(uf.unique_edge_count(0) == 4);
        assert(uf.same(0, 4));
        assert(!uf.same(0, 5));
        assert(uf.group_count() == 2);
    }

    {
        kyopro::dsu uf(4);
        uf.merge(0, 1);
        uf.merge(2, 3);
        auto groups = uf.groups();
        for (auto& group : groups) sort(group.begin(), group.end());
        sort(groups.begin(), groups.end());
        assert(groups == vector<vector<int>>({{0, 1}, {2, 3}}));
    }

    {
        kyopro::dsu uf;
        assert(uf.group_count() == 0);
        assert(uf.groups().empty());
    }

    return 0;
}
