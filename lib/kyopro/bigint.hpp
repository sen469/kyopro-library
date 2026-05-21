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
        std::vector<int> res(digits_.size() + other.digits_.size());
        for (std::size_t i = 0; i < digits_.size(); i++) {
            long long carry = 0;
            for (std::size_t j = 0; j < other.digits_.size(); j++) {
                long long cur = res[i + j] + carry + (long long)digits_[i] * other.digits_[j];
                res[i + j] = (int)(cur % base);
                carry = cur / base;
            }
            for (std::size_t j = other.digits_.size(); carry > 0; j++) {
                if (i + j == res.size()) res.push_back(0);
                long long cur = res[i + j] + carry;
                res[i + j] = (int)(cur % base);
                carry = cur / base;
            }
        }
        digits_ = res;
        sign_ *= other.sign_;
        trim();
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
