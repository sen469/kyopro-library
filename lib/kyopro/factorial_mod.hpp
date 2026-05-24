#ifndef KYOPRO_FACTORIAL_MOD_HPP
#define KYOPRO_FACTORIAL_MOD_HPP

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace kyopro {

namespace internal {

inline long long factorial_mod_pow(long long a, long long e, long long mod) {
    long long res = 1 % mod;
    while (e > 0) {
        if (e & 1) res = (__int128)res * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

}  // namespace internal

template <int MOD>
int factorial_mod_prime(long long n) {
    static_assert(2 <= MOD);
    assert(0 <= n);

    if (n >= MOD) return 0;

    long long right = (long long)MOD - 1 - n;
    if (n <= right) {
        long long res = 1;
        for (long long i = 1; i <= n; i++) {
            res = res * i % MOD;
        }
        return (int)res;
    }

    long long prod = 1;
    for (long long i = n + 1; i < MOD; i++) {
        prod = prod * i % MOD;
    }

    long long inv_prod = internal::factorial_mod_pow(prod, MOD - 2, MOD);
    return (int)((long long)(MOD - 1) * inv_prod % MOD);
}

inline int factorial_mod_prime(long long n, int mod) {
    assert(0 <= n);
    assert(2 <= mod);

    if (n >= mod) return 0;

    long long right = (long long)mod - 1 - n;
    if (n <= right) {
        long long res = 1;
        for (long long i = 1; i <= n; i++) {
            res = res * i % mod;
        }
        return (int)res;
    }

    long long prod = 1;
    for (long long i = n + 1; i < mod; i++) {
        prod = prod * i % mod;
    }

    long long inv_prod = internal::factorial_mod_pow(prod, mod - 2, mod);
    return (int)((long long)(mod - 1) * inv_prod % mod);
}


template <int MOD, class T>
std::vector<int> factorial_mod_prime_batch(const std::vector<T>& ns) {
    static_assert(2 <= MOD);

    std::vector<int> ans(ns.size());
    std::vector<std::pair<long long, int>> left;
    std::vector<std::pair<long long, int>> right;

    for (int i = 0; i < (int)ns.size(); i++) {
        long long n = ns[i];
        assert(0 <= n);
        if (n >= MOD) {
            ans[i] = 0;
            continue;
        }

        long long d = (long long)MOD - 1 - n;
        if (n <= d) {
            left.push_back({n, i});
        } else {
            right.push_back({d, i});
        }
    }

    std::sort(left.begin(), left.end());
    long long cur_n = 0;
    long long fact = 1;
    for (auto [n, idx] : left) {
        while (cur_n < n) {
            cur_n++;
            fact = fact * cur_n % MOD;
        }
        ans[idx] = (int)fact;
    }

    std::sort(right.begin(), right.end());
    long long cur_d = 0;
    long long prod = 1;
    for (auto [d, idx] : right) {
        while (cur_d < d) {
            cur_d++;
            prod = prod * (MOD - cur_d) % MOD;
        }
        long long inv_prod = internal::factorial_mod_pow(prod, MOD - 2, MOD);
        ans[idx] = (int)((long long)(MOD - 1) * inv_prod % MOD);
    }

    return ans;
}

template <class T>
std::vector<int> factorial_mod_prime_batch(const std::vector<T>& ns, int mod) {
    assert(2 <= mod);

    std::vector<int> ans(ns.size());
    std::vector<std::pair<long long, int>> left;
    std::vector<std::pair<long long, int>> right;

    for (int i = 0; i < (int)ns.size(); i++) {
        long long n = ns[i];
        assert(0 <= n);
        if (n >= mod) {
            ans[i] = 0;
            continue;
        }

        long long d = (long long)mod - 1 - n;
        if (n <= d) {
            left.push_back({n, i});
        } else {
            right.push_back({d, i});
        }
    }

    std::sort(left.begin(), left.end());
    long long cur_n = 0;
    long long fact = 1 % mod;
    for (auto [n, idx] : left) {
        while (cur_n < n) {
            cur_n++;
            fact = fact * cur_n % mod;
        }
        ans[idx] = (int)fact;
    }

    std::sort(right.begin(), right.end());
    long long cur_d = 0;
    long long prod = 1 % mod;
    for (auto [d, idx] : right) {
        while (cur_d < d) {
            cur_d++;
            prod = prod * (mod - cur_d) % mod;
        }
        long long inv_prod = internal::factorial_mod_pow(prod, mod - 2, mod);
        ans[idx] = (int)((long long)(mod - 1) * inv_prod % mod);
    }

    return ans;
}

}  // namespace kyopro

#endif  // KYOPRO_FACTORIAL_MOD_HPP
