#ifndef KYOPRO_ROLLING_HASH_HPP
#define KYOPRO_ROLLING_HASH_HPP

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace kyopro {

class rolling_hash {
public:
    using ull = unsigned long long;

private:
    ull base_;
    std::vector<ull> pow_;
    std::vector<ull> hash_;

    static ull add_mod(ull a, ull b) {
        a += b;
        if (a >= mod() || a < b) a -= mod();
        return a;
    }

    static ull sub_mod(ull a, ull b) {
        if (a < b) a += mod();
        return a - b;
    }

    static ull mul_mod(ull a, ull b) {
        return (ull)((unsigned __int128)a * b % mod());
    }

    static ull char_value(char c) {
        return (ull)(unsigned char)c + 1;
    }

    template <class T>
    static ull value(T x) {
        return (ull)((unsigned __int128)(ull)x % (mod() - 1)) + 1;
    }

public:
    static constexpr ull mod() {
        return (1ULL << 61) - 1;
    }

    static constexpr ull default_base() {
        return 100000007ULL;
    }

    rolling_hash() : base_(default_base()), pow_(1, 1), hash_(1, 0) {}

    explicit rolling_hash(const std::string& s, ull base = default_base()) {
        build(s, base);
    }

    template <class T>
    explicit rolling_hash(const std::vector<T>& v, ull base = default_base()) {
        build(v, base);
    }

    void build(const std::string& s, ull base = default_base()) {
        assert(1 <= base && base < mod());
        base_ = base;
        int n = (int)s.size();
        pow_.assign(n + 1, 1);
        hash_.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            pow_[i + 1] = mul_mod(pow_[i], base_);
            hash_[i + 1] = add_mod(mul_mod(hash_[i], base_), char_value(s[i]));
        }
    }

    template <class T>
    void build(const std::vector<T>& v, ull base = default_base()) {
        assert(1 <= base && base < mod());
        base_ = base;
        int n = (int)v.size();
        pow_.assign(n + 1, 1);
        hash_.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            pow_[i + 1] = mul_mod(pow_[i], base_);
            hash_[i + 1] = add_mod(mul_mod(hash_[i], base_), value(v[i]));
        }
    }

    int size() const {
        return (int)hash_.size() - 1;
    }

    bool empty() const {
        return size() == 0;
    }

    ull base() const {
        return base_;
    }

    ull get(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        return sub_mod(hash_[r], mul_mod(hash_[l], pow_[r - l]));
    }

    ull all() const {
        return hash_.back();
    }

    ull concat(ull left_hash, ull right_hash, int right_length) const {
        assert(0 <= right_length && right_length <= size());
        return add_mod(mul_mod(left_hash, pow_[right_length]), right_hash);
    }

    bool same(int l1, int r1, int l2, int r2) const {
        assert(0 <= l1 && l1 <= r1 && r1 <= size());
        assert(0 <= l2 && l2 <= r2 && r2 <= size());
        return r1 - l1 == r2 - l2 && get(l1, r1) == get(l2, r2);
    }

    int lcp(int a, int b) const {
        assert(0 <= a && a <= size());
        assert(0 <= b && b <= size());
        int ok = 0;
        int ng = size() - std::max(a, b) + 1;
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (get(a, a + mid) == get(b, b + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return ok;
    }

    int lcp(int l1, int r1, int l2, int r2) const {
        assert(0 <= l1 && l1 <= r1 && r1 <= size());
        assert(0 <= l2 && l2 <= r2 && r2 <= size());
        int ok = 0;
        int ng = std::min(r1 - l1, r2 - l2) + 1;
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (get(l1, l1 + mid) == get(l2, l2 + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return ok;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_ROLLING_HASH_HPP
