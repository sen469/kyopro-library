#include <bits/stdc++.h>

#include "kyopro/segtree_nd.hpp"

using namespace std;

long long op_ll(long long a, long long b) { return a + b; }
long long e_ll() { return 0; }

template <int D>
vector<vector<int>> points(const vector<int>& sizes) {
    vector<vector<int>> res;
    vector<int> p(D, 0);
    while (true) {
        res.push_back(p);
        int i = D - 1;
        while (0 <= i) {
            p[i]++;
            if (p[i] <= sizes[i]) break;
            p[i] = 0;
            i--;
        }
        if (i < 0) break;
    }
    return res;
}

bool leq(const vector<int>& l, const vector<int>& r) {
    assert(l.size() == r.size());
    for (int i = 0; i < (int)l.size(); i++) {
        if (l[i] > r[i]) return false;
    }
    return true;
}

long long naive_prod(const vector<int>& sizes, const vector<long long>& v,
                     const vector<int>& l, const vector<int>& r) {
    long long res = 0;
    vector<int> p(sizes.size(), 0);
    for (int pos = 0; pos < (int)v.size(); pos++) {
        bool inside = true;
        for (int i = 0; i < (int)sizes.size(); i++) {
            inside &= l[i] <= p[i] && p[i] < r[i];
        }
        if (inside) res += v[pos];

        for (int i = (int)sizes.size() - 1; i >= 0; i--) {
            p[i]++;
            if (p[i] < sizes[i]) break;
            p[i] = 0;
        }
    }
    return res;
}

vector<long long> make_values(const vector<int>& sizes) {
    int total = 1;
    for (int size : sizes) total *= size;
    vector<long long> v(total);
    for (int i = 0; i < total; i++) v[i] = i % 11 - 5;
    return v;
}

template <int D>
void test_all_ranges(const vector<int>& sizes, const vector<long long>& v) {
    kyopro::segtree_nd<long long, D, op_ll, e_ll> seg(sizes, v);
    assert(seg.dimension() == D);
    assert(seg.sizes() == sizes);
    assert(seg.total_size() == v.size());
    for (int i = 0; i < D; i++) assert(seg.size(i) == sizes[i]);

    vector<int> zero(D, 0);
    for (const auto& l : points<D>(sizes)) {
        for (const auto& r : points<D>(sizes)) {
            if (!leq(l, r)) continue;
            assert(seg.prod(l, r) == naive_prod(sizes, v, l, r));
        }
    }
    assert(seg.all_prod() == naive_prod(sizes, v, zero, sizes));
}

int main() {
    {
        kyopro::segtree_nd<long long, 3, op_ll, e_ll> seg;
        assert(seg.dimension() == 3);
        assert(seg.sizes() == (vector<int>{0, 0, 0}));
        assert(seg.total_size() == 0);
        assert(seg.empty());
        assert(seg.all_prod() == 0);
        assert(seg.prod({0, 0, 0}, {0, 0, 0}) == 0);
    }

    test_all_ranges<1>({6}, {3, -1, 4, 1, -5, 9});
    test_all_ranges<2>({3, 4}, make_values({3, 4}));
    test_all_ranges<3>({2, 2, 3}, make_values({2, 2, 3}));
    test_all_ranges<4>({2, 3, 2, 4}, make_values({2, 3, 2, 4}));

    {
        vector<vector<long long>> v = {
            {1, 2, 3},
            {4, 5, 6},
        };
        kyopro::segtree_nd<long long, 2, op_ll, e_ll> seg(v);
        assert(seg.sizes() == (vector<int>{2, 3}));
        assert(seg.get({1, 2}) == 6);
        assert(seg.prod({0, 1}, {2, 3}) == 16);
        seg.set({1, 1}, 50);
        assert(seg.get({1, 1}) == 50);
        assert(seg.prod({0, 1}, {2, 3}) == 61);
        assert(seg.all_prod() == 66);
    }

    {
        vector<vector<vector<long long>>> v = {
            {
                {1, 2},
                {3, 4},
            },
            {
                {5, 6},
                {7, 8},
            },
        };
        kyopro::segtree_nd<long long, 3, op_ll, e_ll> seg(v);
        assert(seg.sizes() == (vector<int>{2, 2, 2}));
        assert(seg.get({1, 0, 1}) == 6);
        assert(seg.prod({0, 0, 0}, {2, 2, 2}) == 36);
        assert(seg.prod({1, 0, 0}, {2, 2, 1}) == 12);
    }

    {
        const int h = 12;
        const int w = 15;
        vector<int> sizes = {h, w};
        vector<long long> v(h * w, 0);
        kyopro::segtree_nd<long long, 2, op_ll, e_ll> seg(sizes);
        mt19937 rng(0);

        for (int step = 0; step < 5000; step++) {
            int kind = uniform_int_distribution<int>(0, 2)(rng);
            if (kind == 0) {
                int y = uniform_int_distribution<int>(0, h - 1)(rng);
                int x = uniform_int_distribution<int>(0, w - 1)(rng);
                long long value = uniform_int_distribution<int>(-100, 100)(rng);
                seg.set({y, x}, value);
                v[y * w + x] = value;
            } else if (kind == 1) {
                int y = uniform_int_distribution<int>(0, h - 1)(rng);
                int x = uniform_int_distribution<int>(0, w - 1)(rng);
                assert(seg.get({y, x}) == v[y * w + x]);
            } else {
                int y1 = uniform_int_distribution<int>(0, h)(rng);
                int y2 = uniform_int_distribution<int>(y1, h)(rng);
                int x1 = uniform_int_distribution<int>(0, w)(rng);
                int x2 = uniform_int_distribution<int>(x1, w)(rng);
                assert(seg.prod({y1, x1}, {y2, x2}) ==
                       naive_prod(sizes, v, {y1, x1}, {y2, x2}));
            }
            assert(seg.all_prod() == naive_prod(sizes, v, {0, 0}, sizes));
        }
    }

    return 0;
}
