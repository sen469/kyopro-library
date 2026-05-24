#include <bits/stdc++.h>

#include "kyopro/compress.hpp"

using namespace std;

int main() {
    {
        vector<int> a = {10, -5, 10, 7, -5};
        auto res = kyopro::compress(a);

        assert(res.compressed == vector<int>({2, 0, 2, 1, 0}));
        assert(res.values == vector<int>({-5, 7, 10}));
        assert(res.size() == 3);
    }

    {
        vector<long long> a = {10000000000LL, -3, 4, 4};
        kyopro::compressor<long long> comp(a);

        assert(comp.size() == 3);
        assert(comp.values() == vector<long long>({-3, 4, 10000000000LL}));
        assert(comp.get(-3) == 0);
        assert(comp.get(4) == 1);
        assert(comp.get(10000000000LL) == 2);
        assert(comp.lower_bound(0) == 1);
        assert(comp.upper_bound(4) == 2);
        assert(comp.contains(4));
        assert(!comp.contains(5));
        assert(comp[2] == 10000000000LL);
    }

    {
        kyopro::compressor<string> comp;
        comp.add("banana");
        comp.add("apple");
        comp.add("banana");
        comp.build();

        assert(comp.size() == 2);
        assert(comp.get("apple") == 0);
        assert(comp.get("banana") == 1);
        assert(comp.values() == vector<string>({"apple", "banana"}));
    }

    {
        vector<int> a;
        auto res = kyopro::compress(a);
        assert(res.compressed.empty());
        assert(res.values.empty());
    }

    return 0;
}
