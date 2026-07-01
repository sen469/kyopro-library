#include <bits/stdc++.h>

#include "kyopro/persistent_segtree.hpp"

using namespace std;

long long op_ll(long long a, long long b) { return a + b; }
long long e_ll() { return 0; }

using segtree = kyopro::persistent_segtree<long long, op_ll, e_ll>;

string op_string(string a, string b) { return a + b; }
string e_string() { return ""; }

using string_segtree = kyopro::persistent_segtree<string, op_string, e_string>;

long long sum_vector(const vector<long long>& v, int l, int r) {
    long long res = 0;
    for (int i = l; i < r; i++) res += v[i];
    return res;
}

int main() {
    {
        segtree empty;
        assert(empty.size() == 0);
        assert(empty.empty());
        assert(empty.all_prod() == 0);
        assert(empty.prod(0, 0) == 0);
        assert(empty.max_right(0, [](long long x) { return x == 0; }) == 0);
        assert(empty.min_left(0, [](long long x) { return x == 0; }) == 0);
    }

    {
        vector<long long> a = {1, 2, 3, 4, 5};
        segtree seg(a);
        assert(seg.size() == 5);
        assert(!seg.empty());
        assert(seg.all_prod() == 15);
        assert(seg.prod(1, 4) == 9);
        assert(seg.get(2) == 3);
        assert(seg[4] == 5);

        segtree seg2 = seg.set(2, 10);
        assert(seg.prod(0, 5) == 15);
        assert(seg.get(2) == 3);
        assert(seg2.prod(0, 5) == 22);
        assert(seg2.get(2) == 10);

        segtree seg3 = seg2.set(0, -1);
        assert(seg2.get(0) == 1);
        assert(seg3.get(0) == -1);
        assert(seg3.prod(0, 3) == 11);
    }

    {
        const int n = 50;
        vector<segtree> segs;
        vector<vector<long long>> vs;
        segs.emplace_back(n);
        vs.emplace_back(n, 0);
        mt19937 rng(0);

        for (int step = 0; step < 2000; step++) {
            int version = uniform_int_distribution<int>(0, (int)segs.size() - 1)(rng);
            int p = uniform_int_distribution<int>(0, n - 1)(rng);
            long long x = uniform_int_distribution<int>(-100, 100)(rng);

            segs.push_back(segs[version].set(p, x));
            vs.push_back(vs[version]);
            vs.back()[p] = x;

            for (int iter = 0; iter < 5; iter++) {
                int qv = uniform_int_distribution<int>(0, (int)segs.size() - 1)(rng);
                int l = uniform_int_distribution<int>(0, n)(rng);
                int r = uniform_int_distribution<int>(l, n)(rng);
                assert(segs[qv].prod(l, r) == sum_vector(vs[qv], l, r));
                assert(segs[qv].all_prod() == sum_vector(vs[qv], 0, n));
                if (l < n) assert(segs[qv].get(l) == vs[qv][l]);
            }
        }
    }

    {
        const int n = 80;
        vector<long long> v(n);
        mt19937 rng(1);
        for (int i = 0; i < n; i++) {
            v[i] = uniform_int_distribution<int>(0, 10)(rng);
        }
        segtree seg(v);

        for (int l = 0; l <= n; l++) {
            for (long long limit = 0; limit <= 60; limit++) {
                long long s = 0;
                int expected = l;
                while (expected < n && s + v[expected] <= limit) {
                    s += v[expected];
                    expected++;
                }
                assert(seg.max_right(l, [&](long long x) {
                    return x <= limit;
                }) == expected);
            }
        }

        for (int r = 0; r <= n; r++) {
            for (long long limit = 0; limit <= 60; limit++) {
                long long s = 0;
                int expected = r;
                while (0 < expected && s + v[expected - 1] <= limit) {
                    expected--;
                    s += v[expected];
                }
                assert(seg.min_left(r, [&](long long x) {
                    return x <= limit;
                }) == expected);
            }
        }
    }

    {
        string_segtree seg(vector<string>{"a", "b", "c", "d"});
        string_segtree seg2 = seg.set(1, "B");
        assert(seg.prod(0, 4) == "abcd");
        assert(seg2.prod(0, 4) == "aBcd");
        assert(seg2.prod(1, 3) == "Bc");
    }

    return 0;
}
