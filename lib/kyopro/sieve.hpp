#ifndef KYOPRO_SIEVE_HPP
#define KYOPRO_SIEVE_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <unordered_map>
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

    static bool is_nonnegative(T x) {
        if constexpr (std::is_signed<T>::value) {
            return 0 <= x;
        } else {
            (void)x;
            return true;
        }
    }

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
                return (uint64_t)(((__uint128_t)mod_mul(v, v, n) + c) % n);
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
        assert(is_nonnegative(x));
        if ((uint64_t)x <= (uint64_t)n_) return x >= 2 && spf_[(int)x] == x;
        return is_prime_u64((uint64_t)x);
    }

    int spf(T x) const {
        assert(2 <= x && x <= n_);
        return spf_[(int)x];
    }

    std::vector<std::pair<T, int>> factorize(T x) const {
        assert(1 <= x);
        std::vector<std::pair<T, int>> res;

        if ((uint64_t)x <= (uint64_t)n_) {
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

template <class T>
std::vector<std::pair<T, int>> factorize(T x) {
    return sieve<T>().factorize(x);
}

namespace internal {

class prime_counter {
private:
    static constexpr int table_size = 5000000;
    static constexpr int phi_x = 100000;
    static constexpr int phi_s = 100;

    std::vector<int> primes_;
    std::vector<int> pi_;
    std::vector<long long> phi_cache_;
    std::unordered_map<uint64_t, long long> lehmer_cache_;

    static uint64_t isqrt(uint64_t x) {
        uint64_t r = (uint64_t)std::sqrt((long double)x);
        while ((__uint128_t)(r + 1) * (r + 1) <= x) r++;
        while ((__uint128_t)r * r > x) r--;
        return r;
    }

    static uint64_t icbrt(uint64_t x) {
        uint64_t r = (uint64_t)std::cbrt((long double)x);
        while ((__uint128_t)(r + 1) * (r + 1) * (r + 1) <= x) r++;
        while ((__uint128_t)r * r * r > x) r--;
        return r;
    }

    static uint64_t iroot4(uint64_t x) {
        uint64_t r = (uint64_t)std::sqrt((long double)isqrt(x));
        while ((__uint128_t)(r + 1) * (r + 1) * (r + 1) * (r + 1) <= x) r++;
        while ((__uint128_t)r * r * r * r > x) r--;
        return r;
    }

    long long phi(uint64_t x, int s) {
        if (s == 0) return (long long)x;
        if (s < phi_s && x < phi_x) {
            long long& res = phi_cache_[s * phi_x + (int)x];
            if (res != -1) return res;
            res = phi(x, s - 1) - phi(x / primes_[s - 1], s - 1);
            return res;
        }
        return phi(x, s - 1) - phi(x / primes_[s - 1], s - 1);
    }

public:
    prime_counter() : pi_(table_size + 1, 0), phi_cache_(phi_s * phi_x, -1) {
        std::vector<bool> is_prime(table_size + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i <= table_size; i++) {
            if (is_prime[i]) {
                primes_.push_back(i);
                if (1LL * i * i <= table_size) {
                    for (long long j = 1LL * i * i; j <= table_size; j += i) {
                        is_prime[(int)j] = false;
                    }
                }
            }
            pi_[i] = (int)primes_.size();
        }
    }

    long long lehmer_pi(uint64_t x) {
        if (x <= table_size) return pi_[(int)x];
        auto it = lehmer_cache_.find(x);
        if (it != lehmer_cache_.end()) return it->second;

        long long a = lehmer_pi(iroot4(x));
        long long b = lehmer_pi(isqrt(x));
        long long c = lehmer_pi(icbrt(x));
        long long res = phi(x, (int)a) + (b + a - 2) * (b - a + 1) / 2;

        for (long long i = a; i < b; i++) {
            uint64_t w = x / primes_[(int)i];
            res -= lehmer_pi(w);
            if (i < c) {
                long long lim = lehmer_pi(isqrt(w));
                for (long long j = i; j < lim; j++) {
                    res -= lehmer_pi(w / primes_[(int)j]) - j;
                }
            }
        }

        lehmer_cache_[x] = res;
        return res;
    }
};

}  // namespace internal

template <class T>
long long prime_count_less(T n) {
    static_assert(std::is_integral<T>::value, "prime_count_less<T>: T must be an integral type");
    if constexpr (std::is_signed<T>::value) {
        assert(0 <= n);
    }
    if (n <= 2) return 0;
    static internal::prime_counter counter;
    return counter.lehmer_pi((uint64_t)n - 1);
}

}  // namespace kyopro

#endif  // KYOPRO_SIEVE_HPP
