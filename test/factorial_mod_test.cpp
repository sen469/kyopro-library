#include <bits/stdc++.h>

#include "kyopro/factorial_mod.hpp"

using namespace std;

int naive_factorial(long long n, int mod) {
    long long res = 1 % mod;
    for (long long i = 1; i <= n; i++) {
        res = res * (i % mod) % mod;
    }
    return (int)res;
}

int main() {
    {
        constexpr int MOD = 998244353;
        assert(kyopro::factorial_mod_prime<MOD>(0) == 1);
        assert(kyopro::factorial_mod_prime<MOD>(10) == naive_factorial(10, MOD));
        assert(kyopro::factorial_mod_prime<MOD>(MOD - 1) == MOD - 1);
        assert(kyopro::factorial_mod_prime<MOD>(MOD - 2) == 1);
        assert(kyopro::factorial_mod_prime<MOD>(MOD - 3) == 499122176);
        assert(kyopro::factorial_mod_prime<MOD>(MOD) == 0);
        assert(kyopro::factorial_mod_prime<MOD>((long long)MOD + 100) == 0);
    }

    {
        vector<int> primes = {2, 3, 5, 7, 11, 97};
        for (int mod : primes) {
            for (int n = 0; n <= mod + 3; n++) {
                assert(kyopro::factorial_mod_prime(n, mod) == naive_factorial(n, mod));
            }
        }
    }

    {
        constexpr int MOD = 101;
        for (int n = 0; n <= MOD + 3; n++) {
            assert(kyopro::factorial_mod_prime<MOD>(n) == naive_factorial(n, MOD));
        }
    }

    {
        constexpr int MOD = 101;
        vector<long long> ns = {0, 1, 10, 50, 98, 99, 100, 101, 150, 3, 97};
        auto ans = kyopro::factorial_mod_prime_batch<MOD>(ns);
        auto dyn = kyopro::factorial_mod_prime_batch(ns, MOD);
        vector<int> ns_int = {0, 1, 10, 50, 98, 99, 100, 101, 150, 3, 97};
        auto ans_int = kyopro::factorial_mod_prime_batch<MOD>(ns_int);
        auto dyn_int = kyopro::factorial_mod_prime_batch(ns_int, MOD);
        for (int i = 0; i < (int)ns.size(); i++) {
            int expected = naive_factorial(ns[i], MOD);
            assert(ans[i] == expected);
            assert(dyn[i] == expected);
            assert(ans_int[i] == expected);
            assert(dyn_int[i] == expected);
        }
    }

    return 0;
}
