#include <bits/stdc++.h>

#include "kyopro/arbitrary_mod_convolution.hpp"

using namespace std;

template <class T>
vector<int> naive_convolution(const vector<T>& a, const vector<T>& b, int mod) {
    if (a.empty() || b.empty()) return {};
    vector<int> res(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++) {
        long long x = a[i] % mod;
        if (x < 0) x += mod;
        for (int j = 0; j < (int)b.size(); j++) {
            long long y = b[j] % mod;
            if (y < 0) y += mod;
            res[i + j] = (res[i + j] + (__int128)x * y) % mod;
        }
    }
    return res;
}

int main() {
    {
        vector<int> a = {1, 2, 3};
        vector<int> b = {4, 5};
        vector<int> expected = {4, 13, 22, 15};
        assert(kyopro::arbitrary_mod_convolution<1000000007>(a, b) == expected);
        assert(kyopro::arbitrary_mod_convolution(a, b, 1000000007) == expected);
    }

    {
        vector<long long> a = {1000000008LL, -2, 3, 4};
        vector<long long> b = {-5, 6, 1000000009LL};
        int mod = 1000000007;
        assert(kyopro::arbitrary_mod_convolution(a, b, mod) == naive_convolution(a, b, mod));
    }

    {
        vector<int> a = {3, 4, 5, 6};
        vector<int> b = {7, 8, 9};
        for (int mod : {1, 2, 12, 998244353, 1000000007}) {
            assert(kyopro::arbitrary_mod_convolution(a, b, mod) == naive_convolution(a, b, mod));
        }
    }

    {
        vector<int> a;
        vector<int> b = {1, 2, 3};
        assert(kyopro::arbitrary_mod_convolution(a, b, 12).empty());
        assert(kyopro::arbitrary_mod_convolution(b, a, 12).empty());
    }

    {
        vector<int> a(80), b(70);
        for (int i = 0; i < (int)a.size(); i++) a[i] = i * i - 100;
        for (int i = 0; i < (int)b.size(); i++) b[i] = 200 - i * 3;
        int mod = 123456789;
        assert(kyopro::arbitrary_mod_convolution(a, b, mod) == naive_convolution(a, b, mod));
    }

    return 0;
}
