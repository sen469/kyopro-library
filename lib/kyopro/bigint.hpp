#ifndef KYOPRO_BIGINT_HPP
#define KYOPRO_BIGINT_HPP

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "atcoder/convolution"

namespace kyopro {

namespace internal {

template <class T>
struct is_bigint_integral : std::is_integral<T> {};

template <>
struct is_bigint_integral<__int128> : std::true_type {};

template <>
struct is_bigint_integral<unsigned __int128> : std::true_type {};

template <class T>
struct is_bigint_signed : std::is_signed<T> {};

template <>
struct is_bigint_signed<__int128> : std::true_type {};

template <>
struct is_bigint_signed<unsigned __int128> : std::false_type {};

}  // namespace internal

class bigint {
private:
    static constexpr int base = 1000000000;
    static constexpr int base_digits = 9;
    static constexpr int fft_base = 1000;
    static constexpr int fft_base_digits = 3;
    static constexpr std::size_t naive_mul_threshold = 80;
    static constexpr std::size_t naive_div_threshold = 120;

    int sign_;
    std::vector<int> digits_;

    void trim() {
        while (!digits_.empty() && digits_.back() == 0) digits_.pop_back();
        if (digits_.empty()) sign_ = 0;
    }

    static int abs_compare(const bigint& a, const bigint& b) {
        if (a.digits_.size() != b.digits_.size()) {
            return a.digits_.size() < b.digits_.size() ? -1 : 1;
        }
        for (int i = (int)a.digits_.size() - 1; i >= 0; i--) {
            if (a.digits_[i] != b.digits_[i]) return a.digits_[i] < b.digits_[i] ? -1 : 1;
        }
        return 0;
    }

    void add_abs(const bigint& other) {
        int carry = 0;
        if (digits_.size() < other.digits_.size()) digits_.resize(other.digits_.size(), 0);
        for (std::size_t i = 0; i < other.digits_.size() || carry; i++) {
            if (i == digits_.size()) digits_.push_back(0);
            long long cur = (long long)digits_[i] + carry;
            if (i < other.digits_.size()) cur += other.digits_[i];
            digits_[i] = (int)(cur % base);
            carry = (int)(cur / base);
        }
    }

    void sub_abs(const bigint& other) {
        int carry = 0;
        for (std::size_t i = 0; i < other.digits_.size() || carry; i++) {
            long long cur = digits_[i] - carry - (i < other.digits_.size() ? other.digits_[i] : 0);
            carry = 0;
            if (cur < 0) {
                cur += base;
                carry = 1;
            }
            digits_[i] = (int)cur;
        }
        trim();
    }

    static std::vector<int> multiply_abs_naive(const std::vector<int>& a, const std::vector<int>& b) {
        std::vector<int> res(a.size() + b.size());
        for (std::size_t i = 0; i < a.size(); i++) {
            long long carry = 0;
            for (std::size_t j = 0; j < b.size(); j++) {
                long long cur = res[i + j] + carry + (long long)a[i] * b[j];
                res[i + j] = (int)(cur % base);
                carry = cur / base;
            }
            for (std::size_t j = b.size(); carry > 0; j++) {
                if (i + j == res.size()) res.push_back(0);
                long long cur = res[i + j] + carry;
                res[i + j] = (int)(cur % base);
                carry = cur / base;
            }
        }
        while (!res.empty() && res.back() == 0) res.pop_back();
        return res;
    }

    static std::vector<long long> split_to_fft_digits(const std::vector<int>& a) {
        std::vector<long long> res;
        res.reserve(a.size() * (base_digits / fft_base_digits));
        for (int x : a) {
            for (int i = 0; i < base_digits; i += fft_base_digits) {
                res.push_back(x % fft_base);
                x /= fft_base;
            }
        }
        while (!res.empty() && res.back() == 0) res.pop_back();
        return res;
    }

    static std::vector<int> multiply_abs_fft(const std::vector<int>& a, const std::vector<int>& b) {
        auto x = split_to_fft_digits(a);
        auto y = split_to_fft_digits(b);
        auto z = atcoder::convolution_ll(x, y);

        long long carry = 0;
        for (std::size_t i = 0; i < z.size(); i++) {
            z[i] += carry;
            carry = z[i] / fft_base;
            z[i] %= fft_base;
        }
        while (carry > 0) {
            z.push_back(carry % fft_base);
            carry /= fft_base;
        }
        while (!z.empty() && z.back() == 0) z.pop_back();

        std::vector<int> res;
        res.reserve((z.size() + base_digits / fft_base_digits - 1) / (base_digits / fft_base_digits));
        for (std::size_t i = 0; i < z.size(); i += base_digits / fft_base_digits) {
            long long cur = 0;
            long long pow = 1;
            for (int j = 0; j < base_digits / fft_base_digits && i + j < z.size(); j++) {
                cur += z[i + j] * pow;
                pow *= fft_base;
            }
            res.push_back((int)cur);
        }
        while (!res.empty() && res.back() == 0) res.pop_back();
        return res;
    }

    static void trim_digits(std::vector<int>& v) {
        while (!v.empty() && v.back() == 0) v.pop_back();
    }

    static std::vector<int> multiply_digits_by_int(const std::vector<int>& v, int m) {
        if (v.empty() || m == 0) return {};
        std::vector<int> res;
        res.reserve(v.size() + 1);
        long long carry = 0;
        for (int x : v) {
            long long cur = carry + (long long)x * m;
            res.push_back((int)(cur % base));
            carry = cur / base;
        }
        if (carry > 0) res.push_back((int)carry);
        return res;
    }

    static int div_digits_by_int(std::vector<int>& v, int d) {
        assert(d > 0);
        long long rem = 0;
        for (int i = (int)v.size() - 1; i >= 0; i--) {
            long long cur = v[i] + rem * base;
            v[i] = (int)(cur / d);
            rem = cur % d;
        }
        trim_digits(v);
        return (int)rem;
    }

    static std::pair<std::vector<int>, std::vector<int>> divmod_abs_digits_naive(const std::vector<int>& a_digits,
                                                                                 const std::vector<int>& b_digits) {
        assert(!b_digits.empty());
        if (a_digits.size() < b_digits.size()) return {{}, a_digits};
        if (a_digits.size() == b_digits.size()) {
            for (int i = (int)a_digits.size() - 1; i >= 0; i--) {
                if (a_digits[i] == b_digits[i]) continue;
                if (a_digits[i] < b_digits[i]) return {{}, a_digits};
                break;
            }
        }
        if (b_digits.size() == 1) {
            std::vector<int> q = a_digits;
            int rem = div_digits_by_int(q, b_digits[0]);
            std::vector<int> r;
            if (rem != 0) r.push_back(rem);
            return {q, r};
        }

        int norm = base / (b_digits.back() + 1);
        std::vector<int> a = multiply_digits_by_int(a_digits, norm);
        std::vector<int> b = multiply_digits_by_int(b_digits, norm);
        a.push_back(0);

        int n = (int)a_digits.size();
        int m = (int)b_digits.size();
        std::vector<int> q(n - m + 1);

        for (int i = n - m; i >= 0; i--) {
            long long numerator = (long long)a[i + m] * base + a[i + m - 1];
            long long d = numerator / b[m - 1];
            long long rem = numerator % b[m - 1];
            if (d >= base) {
                d = base - 1;
                rem = numerator - d * b[m - 1];
            }
            while ((__int128)d * b[m - 2] > (__int128)base * rem + a[i + m - 2]) {
                d--;
                rem += b[m - 1];
                if (rem >= base) break;
            }

            long long carry = 0;
            long long borrow = 0;
            for (int j = 0; j < m; j++) {
                long long cur = carry + d * b[j];
                carry = cur / base;
                cur %= base;
                long long sub = (long long)a[i + j] - cur - borrow;
                if (sub < 0) {
                    sub += base;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                a[i + j] = (int)sub;
            }
            long long sub = (long long)a[i + m] - carry - borrow;
            if (sub < 0) {
                d--;
                long long add_carry = 0;
                for (int j = 0; j < m; j++) {
                    long long cur = (long long)a[i + j] + b[j] + add_carry;
                    if (cur >= base) {
                        cur -= base;
                        add_carry = 1;
                    } else {
                        add_carry = 0;
                    }
                    a[i + j] = (int)cur;
                }
                a[i + m] += (int)add_carry;
            } else {
                a[i + m] = (int)sub;
            }
            q[i] = (int)d;
        }

        trim_digits(q);
        std::vector<int> r(a.begin(), a.begin() + m);
        div_digits_by_int(r, norm);
        return {q, r};
    }

    static std::pair<std::vector<int>, std::vector<int>> divmod_abs_digits(const std::vector<int>& a_digits,
                                                                           const std::vector<int>& b_digits) {
        return divmod_abs_digits_naive(a_digits, b_digits);
    }

    static bigint pad_digits(bigint x, std::size_t k) {
        if (!x.is_zero() && k > 0) x.digits_.insert(x.digits_.begin(), k, 0);
        return x;
    }

    static bigint drop_digits(bigint x, std::size_t k) {
        if (k >= x.digits_.size()) return bigint(0);
        x.digits_.erase(x.digits_.begin(), x.digits_.begin() + k);
        x.trim();
        return x;
    }

    static bigint top_digits(bigint x, std::size_t keep) {
        std::size_t size = x.digits_.size();
        if (keep >= size) return pad_digits(std::move(x), keep - size);
        return drop_digits(std::move(x), size - keep);
    }

    static std::pair<bigint, long long> decimal_trunc(std::pair<bigint, long long> x, std::size_t keep) {
        std::size_t old = x.first.digits_.size();
        std::size_t take = std::min(keep, old);
        x.first = top_digits(std::move(x.first), take);
        x.second += (long long)old - (long long)take;
        return x;
    }

    static long long decimal_magnitude(const std::pair<bigint, long long>& x) {
        if (x.first.is_zero()) return -(1LL << 60);
        return (long long)x.first.digits_.size() + x.second;
    }

    static std::pair<bigint, long long> decimal_add(std::pair<bigint, long long> x,
                                                    std::pair<bigint, long long> y) {
        if (x.second < y.second) {
            x.first += pad_digits(std::move(y.first), y.second - x.second);
        } else {
            x.first = pad_digits(std::move(x.first), x.second - y.second) + y.first;
            x.second = y.second;
        }
        return x;
    }

    static std::pair<bigint, long long> decimal_sub(std::pair<bigint, long long> x,
                                                    std::pair<bigint, long long> y) {
        y.first = -y.first;
        return decimal_add(std::move(x), std::move(y));
    }

    static std::pair<bigint, long long> decimal_mul(std::pair<bigint, long long> x,
                                                    std::pair<bigint, long long> y) {
        x.first *= y.first;
        x.second += y.second;
        return x;
    }

    static bigint decimal_round(const std::pair<bigint, long long>& x) {
        if (x.second >= 0) return pad_digits(x.first, x.second);
        std::size_t drop = (std::size_t)(-x.second);
        if (drop >= x.first.digits_.size()) return bigint(0);
        bigint res = top_digits(x.first, x.first.digits_.size() - drop);
        if (x.first.digits_[drop - 1] >= base / 2) res += 1;
        return res;
    }

    static std::pair<bigint, long long> decimal_inv(std::pair<bigint, long long> x, long long precision) {
        long long high = x.first.digits_.back();
        long long low = x.first.digits_.size() >= 2 ? x.first.digits_[x.first.digits_.size() - 2] : 0;
        __int128 leading = (__int128)high * base + low;
        long long lead = (long long)(((__int128)base * base - 1) / leading);
        if (lead == 0) lead = 1;
        std::pair<bigint, long long> d(bigint(lead), -(long long)x.first.digits_.size() - x.second);
        std::pair<bigint, long long> one(bigint(1), 0);
        long long cur = 2;
        auto amend = decimal_sub(one, decimal_mul(decimal_trunc(x, cur), d));
        amend = decimal_trunc(amend, precision + 2);
        while (-decimal_magnitude(amend) < precision) {
            d = decimal_add(d, decimal_mul(d, amend));
            cur = 2 * (1 - decimal_magnitude(amend));
            cur = std::min<long long>(cur, precision + 2);
            d = decimal_trunc(d, cur);
            amend = decimal_sub(one, decimal_mul(decimal_trunc(x, cur), d));
            amend = decimal_trunc(amend, precision + 2);
        }
        return d;
    }

    static std::pair<bigint, bigint> divmod_abs_fast(bigint a, bigint b) {
        long long precision = (long long)a.digits_.size() - (long long)b.digits_.size() + 1;
        auto inv = decimal_inv({b, 0}, precision);
        auto prod_dec = decimal_mul(std::pair<bigint, long long>(a, 0), inv);
        bigint q = decimal_round(prod_dec);
        bigint r = a - q * b;
        if (r.sign_ < 0) {
            q -= 1;
            r += b;
        }
        if (abs_compare(r, b) >= 0) {
            q += 1;
            r -= b;
        }
        return {q, r};
    }

    static std::pair<bigint, bigint> divmod_abs(bigint a, bigint b) {
        assert(!b.is_zero());
        if (a.digits_.size() < b.digits_.size()) return {bigint(0), a};
        if (b.digits_.size() > naive_div_threshold && a.digits_.size() - b.digits_.size() > naive_div_threshold) {
            return divmod_abs_fast(std::move(a), std::move(b));
        }
        auto [q_digits, r_digits] = divmod_abs_digits(a.digits_, b.digits_);
        bigint q, r;
        q.digits_ = std::move(q_digits);
        r.digits_ = std::move(r_digits);
        q.sign_ = q.digits_.empty() ? 0 : 1;
        r.sign_ = r.digits_.empty() ? 0 : 1;
        q.trim();
        r.trim();
        return {q, r};
    }

public:
    bigint() : sign_(0) {}

    bigint(const char* s) { read(std::string(s)); }

    bigint(const std::string& s) { read(s); }

    template <class T, class = typename std::enable_if<internal::is_bigint_integral<T>::value>::type>
    bigint(T value) : sign_(0) {
        *this = value;
    }

    template <class T, class = typename std::enable_if<internal::is_bigint_integral<T>::value>::type>
    bigint& operator=(T value) {
        sign_ = 0;
        digits_.clear();
        if (value == 0) return *this;
        sign_ = 1;
        if constexpr (internal::is_bigint_signed<T>::value) {
            if (value < 0) sign_ = -1;
        }
        while (value != 0) {
            T rem = value % base;
            if constexpr (internal::is_bigint_signed<T>::value) {
                if (rem < 0) rem = -rem;
            }
            digits_.push_back((int)rem);
            value /= base;
        }
        return *this;
    }

    bigint& read(const std::string& s) {
        sign_ = 1;
        digits_.clear();

        std::size_t pos = 0;
        while (pos < s.size() && std::isspace((unsigned char)s[pos])) pos++;
        if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') sign_ = -1;
            pos++;
        }

        std::size_t end = s.size();
        while (pos < end && std::isspace((unsigned char)s[end - 1])) end--;
        assert(pos < end);

        for (std::size_t i = end; i > pos;) {
            std::size_t l = (i >= pos + base_digits ? i - base_digits : pos);
            int x = 0;
            for (std::size_t j = l; j < i; j++) {
                assert('0' <= s[j] && s[j] <= '9');
                x = x * 10 + (s[j] - '0');
            }
            digits_.push_back(x);
            i = l;
        }
        trim();
        return *this;
    }

    bool is_zero() const { return sign_ == 0; }

    int sign() const { return sign_; }

    bigint abs() const {
        bigint res = *this;
        if (res.sign_ < 0) res.sign_ = 1;
        return res;
    }

    std::string to_string() const {
        if (sign_ == 0) return "0";
        std::string res = sign_ < 0 ? "-" : "";
        res += std::to_string(digits_.back());
        for (int i = (int)digits_.size() - 2; i >= 0; i--) {
            std::string part = std::to_string(digits_[i]);
            res += std::string(base_digits - part.size(), '0') + part;
        }
        return res;
    }

    template <class T, class = typename std::enable_if<internal::is_bigint_integral<T>::value>::type>
    T to_integral() const {
        T res = 0;
        for (int i = (int)digits_.size() - 1; i >= 0; i--) {
            res = res * (T)base + (T)digits_[i];
        }
        return sign_ < 0 ? -res : res;
    }

    bigint operator-() const {
        bigint res = *this;
        res.sign_ = -res.sign_;
        return res;
    }

    bigint& operator+=(const bigint& other) {
        if (other.sign_ == 0) return *this;
        if (sign_ == 0) {
            *this = other;
            return *this;
        }
        if (sign_ == other.sign_) {
            add_abs(other);
            return *this;
        }
        if (abs_compare(*this, other) >= 0) {
            sub_abs(other);
        } else {
            bigint res = other;
            res.sub_abs(*this);
            *this = res;
        }
        return *this;
    }

    bigint& operator-=(const bigint& other) {
        *this += -other;
        return *this;
    }

    bigint& operator*=(const bigint& other) {
        if (is_zero() || other.is_zero()) {
            *this = 0;
            return *this;
        }
        if (std::min(digits_.size(), other.digits_.size()) <= naive_mul_threshold) {
            digits_ = multiply_abs_naive(digits_, other.digits_);
        } else {
            digits_ = multiply_abs_fft(digits_, other.digits_);
        }
        sign_ *= other.sign_;
        trim();
        return *this;
    }

    bigint& operator/=(const bigint& other) {
        assert(!other.is_zero());
        if (is_zero()) return *this;
        int result_sign = sign_ * other.sign_;
        auto [q, r] = divmod_abs(abs(), other.abs());
        (void)r;
        *this = q;
        if (!is_zero()) sign_ = result_sign;
        return *this;
    }

    bigint& operator%=(const bigint& other) {
        assert(!other.is_zero());
        if (is_zero()) return *this;
        int result_sign = sign_;
        auto [q, r] = divmod_abs(abs(), other.abs());
        (void)q;
        *this = r;
        if (!is_zero()) sign_ = result_sign;
        return *this;
    }

    friend bigint operator+(bigint lhs, const bigint& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend bigint operator-(bigint lhs, const bigint& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend bigint operator*(bigint lhs, const bigint& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend bigint operator/(bigint lhs, const bigint& rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend bigint operator%(bigint lhs, const bigint& rhs) {
        lhs %= rhs;
        return lhs;
    }

    friend std::pair<bigint, bigint> divmod(const bigint& lhs, const bigint& rhs) {
        assert(!rhs.is_zero());
        if (lhs.is_zero()) return {bigint(0), bigint(0)};
        int q_sign = lhs.sign_ * rhs.sign_;
        int r_sign = lhs.sign_;
        auto res = divmod_abs(lhs.abs(), rhs.abs());
        if (!res.first.is_zero()) res.first.sign_ = q_sign;
        if (!res.second.is_zero()) res.second.sign_ = r_sign;
        return res;
    }

    friend bool operator<(const bigint& a, const bigint& b) {
        if (a.sign_ != b.sign_) return a.sign_ < b.sign_;
        if (a.sign_ == 0) return false;
        int cmp = abs_compare(a, b);
        return a.sign_ > 0 ? cmp < 0 : cmp > 0;
    }

    friend bool operator>(const bigint& a, const bigint& b) { return b < a; }

    friend bool operator<=(const bigint& a, const bigint& b) { return !(b < a); }

    friend bool operator>=(const bigint& a, const bigint& b) { return !(a < b); }

    friend bool operator==(const bigint& a, const bigint& b) {
        return a.sign_ == b.sign_ && a.digits_ == b.digits_;
    }

    friend bool operator!=(const bigint& a, const bigint& b) { return !(a == b); }

    friend std::ostream& operator<<(std::ostream& os, const bigint& x) {
        return os << x.to_string();
    }

    friend std::istream& operator>>(std::istream& is, bigint& x) {
        std::string s;
        is >> s;
        x.read(s);
        return is;
    }
};

}  // namespace kyopro

#endif
