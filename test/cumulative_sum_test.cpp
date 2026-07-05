#include <bits/stdc++.h>

#include "kyopro/cumulative_sum.hpp"

using namespace std;

vector<vector<int>> points(const vector<int>& sizes) {
    vector<vector<int>> res;
    vector<int> p(sizes.size(), 0);
    while (true) {
        res.push_back(p);
        int i = (int)sizes.size() - 1;
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

vector<long long> to_long_long(const vector<int>& v) {
    return vector<long long>(v.begin(), v.end());
}

long long naive_sum_nd(const vector<int>& sizes, const vector<long long>& v,
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
    for (int i = 0; i < total; i++) v[i] = i % 7 - 3;
    return v;
}

template <int D>
array<int, D> to_array(const vector<int>& v) {
    assert((int)v.size() == D);
    array<int, D> res{};
    for (int i = 0; i < D; i++) res[i] = v[i];
    return res;
}

template <int D>
void test_all_ranges(const vector<int>& sizes, const vector<long long>& v) {
    kyopro::cumulative_sum_nd<long long, D> acc(to_array<D>(sizes), v);
    assert(acc.dimension() == D);
    assert(acc.sizes() == sizes);
    assert(acc.total_size() == v.size());
    for (int i = 0; i < D; i++) {
        assert(acc.size(i) == sizes[i]);
    }

    vector<int> zero(D, 0);
    for (const auto& p : points(sizes)) {
        vector<int> get_indexes = p;
        bool valid_get = true;
        for (int i = 0; i < D; i++) valid_get &= get_indexes[i] < sizes[i];
        if (valid_get) {
            assert(acc.get(get_indexes) ==
                   naive_sum_nd(sizes, v, get_indexes, [&] {
                       vector<int> r = get_indexes;
                       for (int& x : r) x++;
                       return r;
                   }()));
        }
    }

    for (const auto& l : points(sizes)) {
        for (const auto& r : points(sizes)) {
            if (!leq(l, r)) continue;
            assert(acc.sum(to_long_long(l), to_long_long(r)) ==
                   naive_sum_nd(sizes, v, l, r));
        }
    }
    assert(acc.all_sum() == naive_sum_nd(sizes, v, zero, sizes));
}

int main() {
    {
        kyopro::cumulative_sum_nd<long long, 3> acc;
        assert(acc.dimension() == 3);
        assert(acc.sizes() == (vector<int>{0, 0, 0}));
        assert(acc.total_size() == 0);
        assert(acc.empty());
        assert(acc.all_sum() == 0);
    }

    test_all_ranges<1>({6}, {3, -1, 4, 1, -5, 9});
    test_all_ranges<2>({3, 4}, make_values({3, 4}));
    test_all_ranges<3>({2, 2, 3}, make_values({2, 2, 3}));
    test_all_ranges<4>({2, 3, 2, 4}, make_values({2, 3, 2, 4}));

    {
        vector<long long> v = {1, 2, 3, 4};
        kyopro::cumulative_sum_nd<long long, 1> acc(v);
        assert(acc.sizes() == (vector<int>{4}));
        assert(acc.get({2}) == 3);
        assert(acc.prefix_sum({3}) == 6);
        assert(acc.sum({1}, {3}) == 5);
        acc.set({2}, 10);
        assert(acc.get({2}) == 10);
        assert(acc.prefix_sum({3}) == 13);
        assert(acc.sum({1}, {3}) == 12);
        assert(acc.all_sum() == 17);

        acc.build(array<int, 1>{5});
        assert(acc.sizes() == (vector<int>{5}));
        assert(acc.total_size() == 5);
        assert(acc.prefix_sum({5}) == 0);
        assert(acc.sum({1}, {4}) == 0);
    }

    {
        vector<vector<long long>> v = {
            {1, 2, 3},
            {4, 5, 6},
        };
        kyopro::cumulative_sum_nd<long long, 2> acc(v);
        assert(acc.sizes() == (vector<int>{2, 3}));
        assert(acc.get({1, 2}) == 6);
        assert(acc.prefix_sum({2, 2}) == 12);
        assert(acc.sum({0, 1}, {2, 3}) == 16);
        acc.set({1, 1}, 50);
        assert(acc.get({1, 1}) == 50);
        assert(acc.prefix_sum({2, 2}) == 57);
        assert(acc.sum({0, 1}, {2, 3}) == 61);
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
        kyopro::cumulative_sum_nd<long long, 3> acc(v);
        assert(acc.sizes() == (vector<int>{2, 2, 2}));
        assert(acc.get({1, 0, 1}) == 6);
        assert(acc.prefix_sum({1, 2, 2}) == 10);
        assert(acc.sum({0, 0, 0}, {2, 2, 2}) == 36);
        assert(acc.sum({1, 0, 0}, {2, 2, 1}) == 12);
    }

    {
        kyopro::cumulative_sum_nd<long long, 2> acc(array<int, 2>{2, 3});
        assert(acc.total_size() == 6);
        assert(!acc.empty());
        assert(acc.all_sum() == 0);
        assert(acc.prefix_sum({2, 3}) == 0);
    }

    {
        kyopro::cumulative_sum_nd<long long, 3> acc(array<int, 3>{3, 0, 2}, {});
        assert(acc.total_size() == 0);
        assert(acc.empty());
        assert(acc.all_sum() == 0);
        assert(acc.sum({0, 0, 0}, {3, 0, 2}) == 0);
    }

    {
        kyopro::cumulative_sum_nd<long long, 1> acc(vector<long long>{1, 2});
        assert(acc.sum({0}, {2}) == 3);
    }

    return 0;
}
