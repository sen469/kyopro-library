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

    return 0;
}
