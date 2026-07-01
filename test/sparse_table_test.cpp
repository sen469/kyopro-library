#include <bits/stdc++.h>

#include "kyopro/sparse_table.hpp"

using namespace std;

int min_op(int a, int b) { return min(a, b); }
int min_e() { return numeric_limits<int>::max(); }

int max_op(int a, int b) { return max(a, b); }
int max_e() { return numeric_limits<int>::min(); }

int gcd_op(int a, int b) { return gcd(a, b); }
int gcd_e() { return 0; }

using min_table = kyopro::sparse_table<int, min_op, min_e>;
using max_table = kyopro::sparse_table<int, max_op, max_e>;
using gcd_table = kyopro::sparse_table<int, gcd_op, gcd_e>;

int naive_min(const vector<int>& v, int l, int r) {
    int res = min_e();
    for (int i = l; i < r; i++) res = min_op(res, v[i]);
    return res;
}

int naive_max(const vector<int>& v, int l, int r) {
    int res = max_e();
    for (int i = l; i < r; i++) res = max_op(res, v[i]);
    return res;
}

int naive_gcd(const vector<int>& v, int l, int r) {
    int res = gcd_e();
    for (int i = l; i < r; i++) res = gcd_op(res, v[i]);
    return res;
}

int main() {
    {
        min_table st;
        assert(st.size() == 0);
        assert(st.empty());
        assert(st.prod(0, 0) == min_e());
        assert(st.all_prod() == min_e());
    }

    {
        vector<int> v = {5, 2, 7, 3, 9, 1, 4};
        min_table st(v);
        assert(st.size() == (int)v.size());
        assert(!st.empty());
        assert(st.get(0) == 5);
        assert(st[5] == 1);
        assert(st.prod(0, 7) == 1);
        assert(st.prod(1, 4) == 2);
        assert(st.prod(4, 5) == 9);
        assert(st.prod(3, 3) == min_e());
        assert(st.all_prod() == 1);

        st.build(vector<int>{8, 6, 10});
        assert(st.size() == 3);
        assert(st.prod(0, 3) == 6);
        assert(st.prod(2, 3) == 10);
    }

    {
        const int n = 200;
        vector<int> v(n);
        mt19937 rng(0);
        for (int i = 0; i < n; i++) {
            v[i] = uniform_int_distribution<int>(-1000, 1000)(rng);
        }

        min_table mn(v);
        max_table mx(v);

        for (int l = 0; l <= n; l++) {
            for (int r = l; r <= n; r++) {
                assert(mn.prod(l, r) == naive_min(v, l, r));
                assert(mx.prod(l, r) == naive_max(v, l, r));
            }
        }
    }

    {
        const int n = 150;
        vector<int> v(n);
        mt19937 rng(1);
        for (int i = 0; i < n; i++) {
            v[i] = uniform_int_distribution<int>(1, 1000000)(rng);
        }

        gcd_table st(v);
        for (int l = 0; l <= n; l++) {
            for (int r = l; r <= n; r++) {
                assert(st.prod(l, r) == naive_gcd(v, l, r));
            }
        }
    }

    {
        min_table st(10);
        assert(st.size() == 10);
        assert(st.prod(0, 10) == min_e());
        assert(st.prod(4, 9) == min_e());
    }

    return 0;
}
