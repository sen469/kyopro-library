#include <bits/stdc++.h>

#include "kyopro/weighted_union_find.hpp"

using namespace std;

int main() {
    {
        kyopro::weighted_union_find<long long> uf(5);
        assert(uf.merge(0, 1, 3));
        assert(uf.merge(1, 2, 5));
        assert(uf.same(0, 2));
        assert(!uf.same(0, 3));
        assert(uf.diff(0, 1) == 3);
        assert(uf.diff(1, 0) == -3);
        assert(uf.diff(0, 2) == 8);
        assert(uf.diff(2, 0) == -8);
        assert(uf.size(0) == 3);
        assert(uf.size(3) == 1);
    }

    {
        kyopro::weighted_union_find<int> uf(4);
        assert(uf.merge(0, 1, 10));
        assert(uf.merge(2, 3, 7));
        assert(uf.merge(1, 3, 5));
        assert(uf.same(0, 2));
        assert(uf.diff(0, 2) == 8);
        assert(uf.diff(2, 0) == -8);
        assert(uf.consistent(0, 3, 15));
        assert(!uf.consistent(0, 3, 16));
        assert(uf.merge(0, 3, 15));
        assert(!uf.merge(0, 3, 16));
    }

    {
        kyopro::weighted_union_find<long long> uf(6);
        assert(uf.merge(0, 1, 1));
        assert(uf.merge(2, 3, 1));
        assert(uf.merge(4, 5, 1));
        auto groups = uf.groups();
        sort(groups.begin(), groups.end());
        assert(groups == vector<vector<int>>({{0, 1}, {2, 3}, {4, 5}}));
    }

    return 0;
}
