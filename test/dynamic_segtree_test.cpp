#include <bits/stdc++.h>

#include "kyopro/dynamic_segtree.hpp"

using namespace std;

long long op_ll(long long a, long long b) { return a + b; }
long long e_ll() { return 0; }

using segtree = kyopro::dynamic_segtree<long long, op_ll, e_ll>;

string op_string(string a, string b) { return a + b; }
string e_string() { return ""; }

using string_segtree = kyopro::dynamic_segtree<string, op_string, e_string>;

long long sum_vector(const vector<long long>& v, int l, int r) {
    long long res = 0;
    for (int i = l; i < r; i++) res += v[i];
    return res;
}

int main() {
    {
        const int n = 64;
        segtree seg(n);
        vector<long long> v(n, 0);
        mt19937 rng(0);

        for (int step = 0; step < 20000; step++) {
            int kind = uniform_int_distribution<int>(0, 3)(rng);
            if (kind == 0) {
                int p = uniform_int_distribution<int>(0, n - 1)(rng);
                long long x = uniform_int_distribution<int>(-100, 100)(rng);
                seg.set(p, x);
                v[p] = x;
            } else if (kind == 1) {
                int p = uniform_int_distribution<int>(0, n - 1)(rng);
                assert(seg.get(p) == v[p]);
                assert(seg[p] == v[p]);
            } else {
                int l = uniform_int_distribution<int>(0, n)(rng);
                int r = uniform_int_distribution<int>(l, n)(rng);
                assert(seg.prod(l, r) == sum_vector(v, l, r));
            }
            assert(seg.all_prod() == sum_vector(v, 0, n));
        }

        segtree copied = seg;
        assert(copied.all_prod() == seg.all_prod());
        copied.set(0, copied.get(0) + 1);
        assert(copied.get(0) == seg.get(0) + 1);
    }

    {
        const long long l = -1000000000000LL;
        const long long r = 1000000000000LL;
        segtree seg(l, r);
        assert(seg.left_bound() == l);
        assert(seg.right_bound() == r);
        assert(seg.size() == r - l);
        assert(!seg.empty());

        seg.set(l, 3);
        seg.set(-5, 10);
        seg.set(0, 7);
        seg.set(r - 1, 11);

        assert(seg.get(l) == 3);
        assert(seg.get(l + 1) == 0);
        assert(seg.get(-5) == 10);
        assert(seg.get(r - 1) == 11);
        assert(seg.prod(l, r) == 31);
        assert(seg.prod(-5, 1) == 17);
        assert(seg.prod(1, r - 1) == 0);
    }

    {
        string_segtree seg(-10, 10);
        seg.set(-3, "a");
        seg.set(0, "b");
        seg.set(2, "c");
        seg.set(9, "d");

        assert(seg.prod(-10, 10) == "abcd");
        assert(seg.prod(-3, 3) == "abc");
        assert(seg.prod(-2, 9) == "bc");
        assert(seg.prod(3, 9) == "");
    }

    {
        const int n = 80;
        segtree seg(n);
        vector<long long> v(n, 0);
        mt19937 rng(1);

        for (int i = 0; i < n; i++) {
            v[i] = uniform_int_distribution<int>(0, 10)(rng);
            seg.set(i, v[i]);
        }

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

    return 0;
}
