#ifndef KYOPRO_BINOMIAL_HPP
#define KYOPRO_BINOMIAL_HPP

#include <cassert>
#include <vector>

namespace kyopro {

template <int MOD>
class binomial {
private:
    std::vector<int> fact_;
    std::vector<int> inv_fact_;

    static long long mod_pow(long long a, long long e) {
        long long res = 1;
        while (e > 0) {
            if (e & 1) res = res * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return res;
    }

public:
    binomial() : fact_(1, 1), inv_fact_(1, 1) {}

    explicit binomial(int max_n) : binomial() { ensure(max_n); }

    static constexpr int mod() { return MOD; }

    void ensure(int n) {
        assert(0 <= n);
        assert(n < MOD);
        int old = (int)fact_.size() - 1;
        if (n <= old) return;

        fact_.resize(n + 1);
        inv_fact_.resize(n + 1);

        for (int i = old + 1; i <= n; i++) {
            fact_[i] = (long long)fact_[i - 1] * i % MOD;
        }

        inv_fact_[n] = mod_pow(fact_[n], MOD - 2);
        for (int i = n; i > old + 1; i--) {
            inv_fact_[i - 1] = (long long)inv_fact_[i] * i % MOD;
        }
    }

    int fact(int n) {
        ensure(n);
        return fact_[n];
    }

    int inv_fact(int n) {
        ensure(n);
        return inv_fact_[n];
    }

    int perm(int n, int k) {
        if (k < 0 || n < k) return 0;
        ensure(n);
        return (long long)fact_[n] * inv_fact_[n - k] % MOD;
    }

    int comb(int n, int k) {
        if (k < 0 || n < k) return 0;
        ensure(n);
        return (long long)fact_[n] * inv_fact_[k] % MOD * inv_fact_[n - k] % MOD;
    }

    int operator()(int n, int k) { return comb(n, k); }
};

}  // namespace kyopro

#endif  // KYOPRO_BINOMIAL_HPP
