#include <bits/stdc++.h>

#include "kyopro/binomial.hpp"

using namespace std;

long long naive_comb(int n, int k) {
    if (k < 0 || n < k) return 0;
    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1));
    dp[0][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= k; j++) {
            dp[i + 1][j] += dp[i][j];
            if (j + 1 <= k) dp[i + 1][j + 1] += dp[i][j];
        }
    }
    return dp[n][k];
}

long long naive_comb_mod(int n, int k, int mod) {
    if (k < 0 || n < k) return 0;
    if (mod == 1) return 0;
    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1));
    dp[0][0] = 1 % mod;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= k; j++) {
            dp[i + 1][j] = (dp[i + 1][j] + dp[i][j]) % mod;
            if (j + 1 <= k) dp[i + 1][j + 1] = (dp[i + 1][j + 1] + dp[i][j]) % mod;
        }
    }
    return dp[n][k];
}

long long naive_perm_mod(int n, int k, int mod) {
    if (k < 0 || n < k) return 0;
    if (mod == 1) return 0;
    long long res = 1 % mod;
    for (int i = 0; i < k; i++) res = res * (n - i) % mod;
    return res;
}

long long naive_fact_mod(int n, int mod) {
    if (mod == 1) return 0;
    long long res = 1 % mod;
    for (int i = 1; i <= n; i++) res = res * i % mod;
    return res;
}

int main() {
    kyopro::binomial<1000000007> comb;

    assert(comb.mod() == 1000000007);
    assert(comb(0, 0) == 1);
    assert(comb(5, -1) == 0);
    assert(comb(5, 6) == 0);
    assert(comb(5, 0) == 1);
    assert(comb(5, 2) == 10);
    assert(comb(5, 5) == 1);
    assert(comb.perm(5, 2) == 20);
    assert(comb.fact(5) == 120);
    assert((long long)comb.fact(5) * comb.inv_fact(5) % comb.mod() == 1);

    for (int n = 0; n <= 30; n++) {
        for (int k = -1; k <= n + 1; k++) {
            assert(comb(n, k) == naive_comb(n, k));
        }
    }

    kyopro::binomial<998244353> comb2(1000);
    assert(comb2(1000, 0) == 1);
    assert(comb2(1000, 1000) == 1);
    assert(comb2(1000, 1) == 1000);
    assert(comb2(1000, 999) == 1000);

    kyopro::dynamic_binomial dynamic_comb(1000000007);
    assert(dynamic_comb.mod() == 1000000007);
    assert(dynamic_comb(0, 0) == 1);
    assert(dynamic_comb(5, -1) == 0);
    assert(dynamic_comb(5, 6) == 0);
    assert(dynamic_comb(5, 0) == 1);
    assert(dynamic_comb(5, 2) == 10);
    assert(dynamic_comb(5, 5) == 1);
    assert(dynamic_comb.perm(5, 2) == 20);
    assert(dynamic_comb.fact(5) == 120);
    assert((long long)dynamic_comb.fact(5) * dynamic_comb.inv_fact(5) % dynamic_comb.mod() == 1);

    for (int n = 0; n <= 30; n++) {
        for (int k = -1; k <= n + 1; k++) {
            assert(dynamic_comb(n, k) == naive_comb(n, k));
        }
    }

    int runtime_mod = 998244353;
    kyopro::dynamic_binomial dynamic_comb2(runtime_mod, 1000);
    assert(dynamic_comb2(1000, 0) == 1);
    assert(dynamic_comb2(1000, 1000) == 1);
    assert(dynamic_comb2(1000, 1) == 1000);
    assert(dynamic_comb2(1000, 999) == 1000);

    kyopro::arbitrary_mod_binomial<12> arbitrary_comb(60);
    kyopro::dynamic_arbitrary_mod_binomial dynamic_arbitrary_comb(12, 60);
    assert(arbitrary_comb.mod() == 12);
    assert(dynamic_arbitrary_comb.mod() == 12);
    for (int n = 0; n <= 60; n++) {
        assert(arbitrary_comb.fact(n) == naive_fact_mod(n, 12));
        assert(dynamic_arbitrary_comb.fact(n) == naive_fact_mod(n, 12));
        for (int k = -1; k <= n + 1; k++) {
            assert(arbitrary_comb.comb(n, k) == naive_comb_mod(n, k, 12));
            assert(dynamic_arbitrary_comb.comb(n, k) == naive_comb_mod(n, k, 12));
            assert(arbitrary_comb.perm(n, k) == naive_perm_mod(n, k, 12));
            assert(dynamic_arbitrary_comb.perm(n, k) == naive_perm_mod(n, k, 12));
        }
    }

    kyopro::dynamic_arbitrary_mod_binomial dynamic_arbitrary_comb2(1000);
    for (int n = 0; n <= 60; n++) {
        assert(dynamic_arbitrary_comb2.fact(n) == naive_fact_mod(n, 1000));
        for (int k = -1; k <= n + 1; k++) {
            assert(dynamic_arbitrary_comb2.comb(n, k) == naive_comb_mod(n, k, 1000));
            assert(dynamic_arbitrary_comb2.perm(n, k) == naive_perm_mod(n, k, 1000));
        }
    }

    kyopro::dynamic_arbitrary_mod_binomial dynamic_arbitrary_comb3(1, 10);
    assert(dynamic_arbitrary_comb3.fact(10) == 0);
    assert(dynamic_arbitrary_comb3.comb(10, 5) == 0);
    assert(dynamic_arbitrary_comb3.perm(10, 5) == 0);

    kyopro::dynamic_arbitrary_mod_binomial yosupo_case(10007);
    assert(yosupo_case.comb(4, 2) == 6);
    assert(yosupo_case.comb(0, 0) == 1);
    assert(yosupo_case.comb(1000000007LL, 998244353LL) == 0);

    kyopro::dynamic_arbitrary_mod_binomial yosupo_many_factors(510510);
    assert(yosupo_many_factors.comb(1000000007LL, 998244353LL) ==
           yosupo_many_factors.comb(1000000007LL, 1000000007LL - 998244353LL));
    assert(yosupo_many_factors.comb(1000000000000000000LL, 0LL) == 1);
    assert(yosupo_many_factors.comb(1000000000000000000LL, 1000000000000000000LL) == 1);

    return 0;
}
