#ifndef KYOPRO_ARBITRARY_MOD_CONVOLUTION_HPP
#define KYOPRO_ARBITRARY_MOD_CONVOLUTION_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#include "atcoder/convolution"

namespace kyopro {

namespace internal {

inline long long arbitrary_mod_convolution_inv_mod(long long a, long long mod) {
    long long b = mod;
    long long x = 1;
    long long y = 0;
    while (b) {
        long long t = a / b;
        a -= t * b;
        std::swap(a, b);
        x -= t * y;
        std::swap(x, y);
    }
    assert(a == 1);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

}  // namespace internal

template <class T>
std::vector<int> arbitrary_mod_convolution(const std::vector<T>& a, const std::vector<T>& b, int mod) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    assert(1 <= mod);

    int n = (int)a.size();
    int m = (int)b.size();
    if (n == 0 || m == 0) return {};
    if (mod == 1) return std::vector<int>(n + m - 1, 0);

    constexpr long long MOD1 = 754974721;
    constexpr long long MOD2 = 167772161;
    constexpr long long MOD3 = 469762049;
    constexpr long long M1M2 = MOD1 * MOD2;

    static const long long inv_m1_mod_m2 = internal::arbitrary_mod_convolution_inv_mod(MOD1, MOD2);
    static const long long inv_m1m2_mod_m3 = internal::arbitrary_mod_convolution_inv_mod(M1M2 % MOD3, MOD3);

    std::vector<long long> x(n), y(m);
    for (int i = 0; i < n; i++) {
        long long v = (long long)(a[i] % mod);
        if (v < 0) v += mod;
        x[i] = v;
    }
    for (int i = 0; i < m; i++) {
        long long v = (long long)(b[i] % mod);
        if (v < 0) v += mod;
        y[i] = v;
    }

    auto c1 = atcoder::convolution<MOD1>(x, y);
    auto c2 = atcoder::convolution<MOD2>(x, y);
    auto c3 = atcoder::convolution<MOD3>(x, y);

    std::vector<int> res(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        long long r1 = c1[i];
        long long r2 = c2[i];
        long long r3 = c3[i];

        long long t1 = (r2 - r1) % MOD2;
        if (t1 < 0) t1 += MOD2;
        t1 = (__int128)t1 * inv_m1_mod_m2 % MOD2;

        long long x_mod_m3 = (r1 + (__int128)(MOD1 % MOD3) * t1) % MOD3;
        long long t2 = (r3 - x_mod_m3) % MOD3;
        if (t2 < 0) t2 += MOD3;
        t2 = (__int128)t2 * inv_m1m2_mod_m3 % MOD3;

        long long v = 0;
        v = (v + r1 % mod) % mod;
        v = (v + (__int128)(MOD1 % mod) * (t1 % mod) % mod) % mod;
        v = (v + (__int128)(M1M2 % mod) * (t2 % mod) % mod) % mod;
        res[i] = (int)v;
    }

    return res;
}

template <int MOD, class T>
std::vector<int> arbitrary_mod_convolution(const std::vector<T>& a, const std::vector<T>& b) {
    static_assert(1 <= MOD, "MOD must be positive");
    return arbitrary_mod_convolution(a, b, MOD);
}

}  // namespace kyopro

#endif  // KYOPRO_ARBITRARY_MOD_CONVOLUTION_HPP
