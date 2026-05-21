#ifndef KYOPRO_BINOMIAL_HPP
#define KYOPRO_BINOMIAL_HPP

#include <cassert>
#include <cstdint>
#include <utility>
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

class dynamic_binomial {
private:
    int mod_;
    std::vector<int> fact_;
    std::vector<int> inv_fact_;

    long long mod_pow(long long a, long long e) const {
        long long res = 1;
        while (e > 0) {
            if (e & 1) res = res * a % mod_;
            a = a * a % mod_;
            e >>= 1;
        }
        return res;
    }

public:
    explicit dynamic_binomial(int mod) : mod_(mod), fact_(1, 1), inv_fact_(1, 1) {
        assert(2 <= mod_);
    }

    dynamic_binomial(int mod, int max_n) : dynamic_binomial(mod) { ensure(max_n); }

    int mod() const { return mod_; }

    void ensure(int n) {
        assert(0 <= n);
        assert(n < mod_);
        int old = (int)fact_.size() - 1;
        if (n <= old) return;

        fact_.resize(n + 1);
        inv_fact_.resize(n + 1);

        for (int i = old + 1; i <= n; i++) {
            fact_[i] = (long long)fact_[i - 1] * i % mod_;
        }

        inv_fact_[n] = mod_pow(fact_[n], mod_ - 2);
        for (int i = n; i > old + 1; i--) {
            inv_fact_[i - 1] = (long long)inv_fact_[i] * i % mod_;
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
        return (long long)fact_[n] * inv_fact_[n - k] % mod_;
    }

    int comb(int n, int k) {
        if (k < 0 || n < k) return 0;
        ensure(n);
        return (long long)fact_[n] * inv_fact_[k] % mod_ * inv_fact_[n - k] % mod_;
    }

    int operator()(int n, int k) { return comb(n, k); }
};

namespace internal {

inline long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

inline long long inv_mod(long long a, long long mod) {
    long long x, y;
    long long g = ext_gcd(a, mod, x, y);
    assert(g == 1);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

inline long long mod_pow(long long a, long long e, long long mod) {
    long long res = 1 % mod;
    while (e > 0) {
        if (e & 1) res = (__int128)res * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

inline std::vector<std::pair<int, int>> factorize_mod(int mod) {
    std::vector<std::pair<int, int>> res;
    for (long long p = 2; p * p <= mod; p++) {
        if (mod % p != 0) continue;
        int q = 1;
        while (mod % p == 0) {
            mod /= (int)p;
            q *= (int)p;
        }
        res.emplace_back((int)p, q);
    }
    if (mod > 1) res.emplace_back(mod, mod);
    return res;
}

class prime_power_binomial {
private:
    int p_;
    int q_;
    int exponent_;
    bool periodic_;
    std::vector<int> fact_;

    long long count_p(long long n) const {
        long long res = 0;
        while (n > 0) {
            n /= p_;
            res += n;
        }
        return res;
    }

    void build_periodic() {
        if (periodic_) return;
        periodic_ = true;
        fact_.assign(q_, 1 % q_);
        for (int i = 1; i < q_; i++) {
            fact_[i] = fact_[i - 1];
            if (i % p_ != 0) fact_[i] = (long long)fact_[i] * i % q_;
        }
    }

    int fact_without_p(long long n) const {
        if (!periodic_) return fact_[n];
        if (n == 0) return 1 % q_;
        long long res = mod_pow(fact_[q_ - 1], n / q_, q_);
        res = res * fact_[n % q_] % q_;
        res = res * fact_without_p(n / p_) % q_;
        return (int)res;
    }

public:
    prime_power_binomial(int p, int q) : p_(p), q_(q), exponent_(0), periodic_(false), fact_(1, 1 % q) {
        for (int x = q_; x > 1; x /= p_) exponent_++;
    }

    int mod() const { return q_; }

    void ensure(long long n) {
        assert(0 <= n);
        if (periodic_) return;
        if (q_ <= n) {
            build_periodic();
            return;
        }

        int old = (int)fact_.size() - 1;
        if (n <= old) return;

        fact_.resize((int)n + 1);

        for (int i = old + 1; i <= n; i++) {
            int x = i;
            while (x % p_ == 0) x /= p_;
            fact_[i] = (long long)fact_[i - 1] * (x % q_) % q_;
        }
    }

    int perm(long long n, long long k) {
        if (k < 0 || n < k) return 0;
        ensure(n);
        long long e = count_p(n) - count_p(n - k);
        if (e >= exponent_) return 0;

        long long res = fact_without_p(n);
        res = res * inv_mod(fact_without_p(n - k), q_) % q_;
        res = res * mod_pow(p_, e, q_) % q_;
        return (int)res;
    }

    int comb(long long n, long long k) {
        if (k < 0 || n < k) return 0;
        ensure(n);
        long long e = count_p(n) - count_p(k) - count_p(n - k);
        if (e >= exponent_) return 0;

        long long res = fact_without_p(n);
        res = res * inv_mod(fact_without_p(k), q_) % q_;
        res = res * inv_mod(fact_without_p(n - k), q_) % q_;
        res = res * mod_pow(p_, e, q_) % q_;
        return (int)res;
    }
};

}  // namespace internal

class dynamic_arbitrary_mod_binomial {
private:
    int mod_;
    std::vector<int> fact_;
    std::vector<internal::prime_power_binomial> bins_;

    int crt(const std::vector<int>& residues) const {
        if (mod_ == 1) return 0;

        long long x = 0;
        long long m = 1;
        for (int i = 0; i < (int)bins_.size(); i++) {
            long long q = bins_[i].mod();
            long long t = (residues[i] - x) % q;
            if (t < 0) t += q;
            t = t * internal::inv_mod(m % q, q) % q;
            x += m * t;
            m *= q;
            x %= m;
        }
        return (int)(x % mod_);
    }

    void ensure_bins(long long n) {
        assert(0 <= n);
        for (auto& bin : bins_) bin.ensure(n);
    }

    void ensure_fact(int n) {
        assert(0 <= n);
        int old = (int)fact_.size() - 1;
        if (n <= old) return;

        fact_.resize(n + 1);
        if (mod_ == 1) {
            for (int i = old + 1; i <= n; i++) fact_[i] = 0;
            return;
        }
        for (int i = old + 1; i <= n; i++) {
            fact_[i] = (long long)fact_[i - 1] * (i % mod_) % mod_;
        }
    }

public:
    explicit dynamic_arbitrary_mod_binomial(int mod) : mod_(mod), fact_(1, mod == 1 ? 0 : 1 % mod) {
        assert(1 <= mod_);
        for (auto [p, q] : internal::factorize_mod(mod_)) {
            bins_.emplace_back(p, q);
        }
    }

    dynamic_arbitrary_mod_binomial(int mod, int max_n) : dynamic_arbitrary_mod_binomial(mod) {
        ensure(max_n);
    }

    int mod() const { return mod_; }

    void ensure(long long n) {
        ensure_bins(n);
    }

    int fact(int n) {
        ensure_fact(n);
        return fact_[n];
    }

    int perm(long long n, long long k) {
        if (k < 0 || n < k) return 0;
        ensure_bins(n);
        std::vector<int> residues;
        residues.reserve(bins_.size());
        for (auto& bin : bins_) residues.push_back(bin.perm(n, k));
        return crt(residues);
    }

    int comb(long long n, long long k) {
        if (k < 0 || n < k) return 0;
        ensure_bins(n);
        std::vector<int> residues;
        residues.reserve(bins_.size());
        for (auto& bin : bins_) residues.push_back(bin.comb(n, k));
        return crt(residues);
    }

    int operator()(long long n, long long k) { return comb(n, k); }
};

template <int MOD>
class arbitrary_mod_binomial {
private:
    dynamic_arbitrary_mod_binomial impl_;

public:
    arbitrary_mod_binomial() : impl_(MOD) {}

    explicit arbitrary_mod_binomial(int max_n) : impl_(MOD, max_n) {}

    static constexpr int mod() { return MOD; }

    void ensure(long long n) { impl_.ensure(n); }

    int fact(int n) { return impl_.fact(n); }

    int perm(long long n, long long k) { return impl_.perm(n, k); }

    int comb(long long n, long long k) { return impl_.comb(n, k); }

    int operator()(long long n, long long k) { return comb(n, k); }
};

}  // namespace kyopro

#endif  // KYOPRO_BINOMIAL_HPP
