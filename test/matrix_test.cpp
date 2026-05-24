#include <bits/stdc++.h>

#include <atcoder/modint>

#include "kyopro/matrix.hpp"

using namespace std;
using mint = atcoder::modint998244353;

int main() {
    {
        kyopro::matrix<long long> a({
            {1, 1},
            {1, 0},
        });

        auto b = a.pow(10);
        assert(b[0][0] == 89);
        assert(b[0][1] == 55);
        assert(b[1][0] == 55);
        assert(b[1][1] == 34);

        auto c = kyopro::matrix_power(a, 0);
        assert(c[0][0] == 1);
        assert(c[0][1] == 0);
        assert(c[1][0] == 0);
        assert(c[1][1] == 1);
    }

    {
        kyopro::matrix<int> a({
            {1, 2, 3},
            {4, 5, 6},
        });
        kyopro::matrix<int> b({
            {7, 8},
            {9, 10},
            {11, 12},
        });

        auto c = a * b;
        assert(c.height() == 2);
        assert(c.width() == 2);
        assert(c[0][0] == 58);
        assert(c[0][1] == 64);
        assert(c[1][0] == 139);
        assert(c[1][1] == 154);
    }

    {
        kyopro::matrix<int> a(2, 2);
        a[0][0] = 1;
        a[0][1] = 2;
        a[1][0] = 3;
        a[1][1] = 4;

        kyopro::matrix<int> b({
            {5, 6},
            {7, 8},
        });

        auto sum = a + b;
        assert(sum[0][0] == 6);
        assert(sum[0][1] == 8);
        assert(sum[1][0] == 10);
        assert(sum[1][1] == 12);

        auto diff = sum - b;
        assert(diff.data() == a.data());
    }

    {
        kyopro::matrix<mint> a({
            {mint(1), mint(1)},
            {mint(1), mint(0)},
        });

        auto b = a.pow(100);
        assert(b[0][1].val() == 494958974);
    }

    return 0;
}
