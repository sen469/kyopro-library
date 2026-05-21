#ifndef KYOPRO_SIEVE_HPP
#define KYOPRO_SIEVE_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class T = int>
class sieve {
private:
    static_assert(std::is_integral<T>::value, "sieve<T>: T must be an integral type");

    int n_;
    std::vector<int> spf_;
    std::vector<int> primes_;

    static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
        return (uint64_t)((__uint128_t)a * b % mod);
    }

    static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
        uint64_t res = 1;
        while (e > 0) {
            if (e & 1) res = mod_mul(res, a, mod);
            a = mod_mul(a, a, mod);
            e >>= 1;
        }
        return res;
    }

    static bool is_prime_u64(uint64_t n) {
        if (n < 2) return false;
        for (uint64_t p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
            if (n % p == 0) return n == p;
        }

        uint64_t d = n - 1;
        int s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            s++;
        }

        for (uint64_t a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
            if (a % n == 0) continue;
            uint64_t x = mod_pow(a % n, d, n);
            if (x == 1 || x == n - 1) continue;

            bool probably_prime = false;
            for (int r = 1; r < s; r++) {
                x = mod_mul(x, x, n);
                if (x == n - 1) {
                    probably_prime = true;
                    break;
                }
            }
            if (!probably_prime) return false;
        }

        return true;
    }

    static uint64_t pollard_rho(uint64_t n) {
        if (n % 2 == 0) return 2;
        if (n % 3 == 0) return 3;

        for (uint64_t c = 1;; c++) {
            uint64_t x = 2;
            uint64_t y = 2;
            uint64_t d = 1;

            auto f = [&](uint64_t v) {
                return (mod_mul(v, v, n) + c) % n;
            };

            while (d == 1) {
                x = f(x);
                y = f(f(y));
                uint64_t diff = x > y ? x - y : y - x;
                d = std::gcd(diff, n);
            }

            if (d != n) return d;
        }
    }

    static void factor_u64(uint64_t x, std::vector<uint64_t>& res) {
        if (x == 1) return;
        if (is_prime_u64(x)) {
            res.push_back(x);
            return;
        }

        uint64_t d = pollard_rho(x);
        factor_u64(d, res);
        factor_u64(x / d, res);
    }

public:
    using value_type = T;

    sieve() : n_(0), spf_(1, 0) {}

    explicit sieve(int n) : sieve() { build(n); }

    void build(int n) {
        assert(0 <= n);
        n_ = n;
        spf_.assign(n_ + 1, 0);
        primes_.clear();

        for (int i = 2; i <= n_; i++) {
            if (spf_[i] == 0) {
                spf_[i] = i;
                primes_.push_back(i);
            }

            for (int p : primes_) {
                long long x = 1LL * p * i;
                if (x > n_ || p > spf_[i]) break;
                spf_[(int)x] = p;
            }
        }
    }

    int max_n() const { return n_; }

    const std::vector<int>& primes() const { return primes_; }

    bool is_prime(T x) const {
        assert(0 <= x);
        if (x <= n_) return x >= 2 && spf_[(int)x] == x;
        return is_prime_u64((uint64_t)x);
    }

    int spf(T x) const {
        assert(2 <= x && x <= n_);
        return spf_[(int)x];
    }

    std::vector<std::pair<T, int>> factorize(T x) const {
        assert(1 <= x);
        std::vector<std::pair<T, int>> res;

        if (x <= n_) {
            while (x > 1) {
                T p = spf_[(int)x];
                int count = 0;
                while (x % p == 0) {
                    x /= p;
                    count++;
                }
                res.emplace_back(p, count);
            }
            return res;
        }

        std::vector<uint64_t> factors;
        factor_u64((uint64_t)x, factors);
        std::sort(factors.begin(), factors.end());

        for (uint64_t p : factors) {
            if (res.empty() || res.back().first != (T)p) {
                res.emplace_back((T)p, 1);
            } else {
                res.back().second++;
            }
        }

        return res;
    }

    std::vector<T> divisors(T x) const {
        assert(1 <= x);
        std::vector<T> res = {1};

        for (auto [p, e] : factorize(x)) {
            int size = (int)res.size();
            T power = 1;
            for (int i = 1; i <= e; i++) {
                power *= p;
                for (int j = 0; j < size; j++) {
                    res.push_back(res[j] * power);
                }
            }
        }

        std::sort(res.begin(), res.end());
        return res;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_SIEVE_HPP
