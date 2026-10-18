#ifndef KYOPRO_SEGTREE_HPP
#define KYOPRO_SEGTREE_HPP

#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class S, auto op, auto e>
class segtree {
    static_assert(std::is_invocable_r<S, decltype(op), S, S>::value,
                  "op must work as S(S, S)");
    static_assert(std::is_invocable_r<S, decltype(e)>::value,
                  "e must work as S()");

    int n_ = 0;
    std::size_t base_ = 1;
    std::vector<S> data_;

    void initialize() {
        while (base_ < static_cast<std::size_t>(n_)) base_ *= 2;
        data_.assign(2 * base_, e());
    }

    void pull(std::size_t v) { data_[v] = op(data_[v * 2], data_[v * 2 + 1]); }

    template <class F>
    int search_right(std::size_t v, std::size_t left, std::size_t right,
                     int first, S& aggregate, F& predicate) const {
        if (right <= static_cast<std::size_t>(first) ||
            left >= static_cast<std::size_t>(n_)) return n_;
        if (static_cast<std::size_t>(first) <= left) {
            S next = op(aggregate, data_[v]);
            if (predicate(next)) {
                aggregate = std::move(next);
                return n_;
            }
            if (right - left == 1) return static_cast<int>(left);
        }
        std::size_t middle = left + (right - left) / 2;
        int boundary = search_right(v * 2, left, middle, first, aggregate, predicate);
        if (boundary != n_) return boundary;
        return search_right(v * 2 + 1, middle, right, first, aggregate, predicate);
    }

    template <class F>
    int search_left(std::size_t v, std::size_t left, std::size_t right,
                    int last, S& aggregate, F& predicate) const {
        if (static_cast<std::size_t>(last) <= left) return 0;
        if (right <= static_cast<std::size_t>(last)) {
            S next = op(data_[v], aggregate);
            if (predicate(next)) {
                aggregate = std::move(next);
                return 0;
            }
            if (right - left == 1) return static_cast<int>(right);
        }
        std::size_t middle = left + (right - left) / 2;
        int boundary = search_left(v * 2 + 1, middle, right, last, aggregate, predicate);
        if (boundary != 0) return boundary;
        return search_left(v * 2, left, middle, last, aggregate, predicate);
    }

public:
    segtree() : segtree(0) {}

    explicit segtree(int n) : n_(n) {
        assert(0 <= n);
        initialize();
    }

    explicit segtree(const std::vector<S>& values) {
        assert(values.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        n_ = static_cast<int>(values.size());
        initialize();
        for (int i = 0; i < n_; i++) data_[base_ + i] = values[i];
        for (std::size_t v = base_ - 1; v > 0; v--) pull(v);
    }

    int size() const { return n_; }

    std::vector<S> to_vec() const {
        return std::vector<S>(data_.begin() + base_, data_.begin() + base_ + n_);
    }

    void set(int p, S x) {
        assert(0 <= p && p < n_);
        std::size_t v = base_ + p;
        data_[v] = std::move(x);
        while (v > 1) {
            v /= 2;
            pull(v);
        }
    }

    S get(int p) const {
        assert(0 <= p && p < n_);
        return data_[base_ + p];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        S prefix = e(), suffix = e();
        std::size_t left = base_ + l, right = base_ + r;
        while (left < right) {
            if (left % 2 != 0) prefix = op(prefix, data_[left++]);
            if (right % 2 != 0) suffix = op(data_[--right], suffix);
            left /= 2;
            right /= 2;
        }
        return op(prefix, suffix);
    }

    S all_prod() const { return data_[1]; }

    template <bool (*f)(S)>
    int max_right(int l) const { return max_right(l, f); }

    template <class F>
    int max_right(int l, F predicate) const {
        assert(0 <= l && l <= n_);
        assert(predicate(e()));
        if (l == n_) return n_;
        S aggregate = e();
        return search_right(1, 0, base_, l, aggregate, predicate);
    }

    template <bool (*f)(S)>
    int min_left(int r) const { return min_left(r, f); }

    template <class F>
    int min_left(int r, F predicate) const {
        assert(0 <= r && r <= n_);
        assert(predicate(e()));
        if (r == 0) return 0;
        S aggregate = e();
        return search_left(1, 0, base_, r, aggregate, predicate);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_SEGTREE_HPP
