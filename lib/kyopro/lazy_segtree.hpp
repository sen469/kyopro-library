#ifndef KYOPRO_LAZY_SEGTREE_HPP
#define KYOPRO_LAZY_SEGTREE_HPP

#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class S, auto op, auto e, class F, auto mapping, auto composition, auto id>
class lazy_segtree {
    static_assert(std::is_invocable_r<S, decltype(op), S, S>::value,
                  "op must work as S(S, S)");
    static_assert(std::is_invocable_r<S, decltype(e)>::value,
                  "e must work as S()");
    static_assert(std::is_invocable_r<S, decltype(mapping), F, S>::value,
                  "mapping must work as S(F, S)");
    static_assert(std::is_invocable_r<F, decltype(composition), F, F>::value,
                  "composition must work as F(F, F)");
    static_assert(std::is_invocable_r<F, decltype(id)>::value,
                  "id must work as F()");

    int n_ = 0;
    int height_ = 0;
    std::size_t base_ = 1;
    std::vector<S> data_;
    std::vector<F> lazy_;
    std::vector<unsigned char> pending_;

    void initialize() {
        while (base_ < static_cast<std::size_t>(n_)) {
            base_ *= 2;
            height_++;
        }
        data_.assign(2 * base_, e());
        lazy_.assign(base_, id());
        pending_.assign(base_, 0);
    }

    void pull(std::size_t v) { data_[v] = op(data_[v * 2], data_[v * 2 + 1]); }

    void apply_node(std::size_t v, const F& f) {
        data_[v] = mapping(f, data_[v]);
        if (v < base_) {
            // composition(f, g) means f after g, as in ACL.
            lazy_[v] = pending_[v] ? composition(f, lazy_[v]) : f;
            pending_[v] = 1;
        }
    }

    void push(std::size_t v) {
        if (!pending_[v]) return;
        apply_node(v * 2, lazy_[v]);
        apply_node(v * 2 + 1, lazy_[v]);
        lazy_[v] = id();
        pending_[v] = 0;
    }

    void push_path(std::size_t leaf) {
        for (int shift = height_; shift > 0; shift--) push(leaf >> shift);
    }

    S range_prod(std::size_t v, std::size_t left, std::size_t right,
                 std::size_t first, std::size_t last) {
        if (last <= left || right <= first) return e();
        if (first <= left && right <= last) return data_[v];
        push(v);
        std::size_t middle = left + (right - left) / 2;
        S prefix = range_prod(v * 2, left, middle, first, last);
        S suffix = range_prod(v * 2 + 1, middle, right, first, last);
        return op(prefix, suffix);
    }

    void range_apply(std::size_t v, std::size_t left, std::size_t right,
                     std::size_t first, std::size_t last, const F& f) {
        if (last <= left || right <= first) return;
        if (first <= left && right <= last) {
            apply_node(v, f);
            return;
        }
        push(v);
        std::size_t middle = left + (right - left) / 2;
        range_apply(v * 2, left, middle, first, last, f);
        range_apply(v * 2 + 1, middle, right, first, last, f);
        pull(v);
    }

    template <class G>
    int search_right(std::size_t v, std::size_t left, std::size_t right,
                     int first, S& aggregate, G& predicate) {
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
        push(v);
        std::size_t middle = left + (right - left) / 2;
        int boundary = search_right(v * 2, left, middle, first, aggregate, predicate);
        if (boundary != n_) return boundary;
        return search_right(v * 2 + 1, middle, right, first, aggregate, predicate);
    }

    template <class G>
    int search_left(std::size_t v, std::size_t left, std::size_t right,
                    int last, S& aggregate, G& predicate) {
        if (static_cast<std::size_t>(last) <= left) return 0;
        if (right <= static_cast<std::size_t>(last)) {
            S next = op(data_[v], aggregate);
            if (predicate(next)) {
                aggregate = std::move(next);
                return 0;
            }
            if (right - left == 1) return static_cast<int>(right);
        }
        push(v);
        std::size_t middle = left + (right - left) / 2;
        int boundary = search_left(v * 2 + 1, middle, right, last, aggregate, predicate);
        if (boundary != 0) return boundary;
        return search_left(v * 2, left, middle, last, aggregate, predicate);
    }

public:
    lazy_segtree() : lazy_segtree(0) {}

    explicit lazy_segtree(int n) : n_(n) {
        assert(0 <= n);
        initialize();
    }

    explicit lazy_segtree(const std::vector<S>& values) {
        assert(values.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        n_ = static_cast<int>(values.size());
        initialize();
        for (int i = 0; i < n_; i++) data_[base_ + i] = values[i];
        for (std::size_t v = base_ - 1; v > 0; v--) pull(v);
    }

    int size() const { return n_; }

    std::vector<S> to_vec() {
        // Parents precede children, so each pending action reaches every leaf.
        for (std::size_t v = 1; v < base_; v++) push(v);
        return std::vector<S>(data_.begin() + base_, data_.begin() + base_ + n_);
    }

    void set(int p, S x) {
        assert(0 <= p && p < n_);
        std::size_t v = base_ + p;
        push_path(v);
        data_[v] = std::move(x);
        while (v > 1) {
            v /= 2;
            pull(v);
        }
    }

    S get(int p) {
        assert(0 <= p && p < n_);
        std::size_t v = base_ + p;
        push_path(v);
        return data_[v];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return e();
        return range_prod(1, 0, base_, l, r);
    }

    S all_prod() const { return data_[1]; }

    void apply(int p, F f) {
        assert(0 <= p && p < n_);
        range_apply(1, 0, base_, p, p + 1, f);
    }

    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return;
        range_apply(1, 0, base_, l, r, f);
    }

    template <bool (*g)(S)>
    int max_right(int l) { return max_right(l, g); }

    template <class G>
    int max_right(int l, G predicate) {
        assert(0 <= l && l <= n_);
        assert(predicate(e()));
        if (l == n_) return n_;
        S aggregate = e();
        return search_right(1, 0, base_, l, aggregate, predicate);
    }

    template <bool (*g)(S)>
    int min_left(int r) { return min_left(r, g); }

    template <class G>
    int min_left(int r, G predicate) {
        assert(0 <= r && r <= n_);
        assert(predicate(e()));
        if (r == 0) return 0;
        S aggregate = e();
        return search_left(1, 0, base_, r, aggregate, predicate);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_LAZY_SEGTREE_HPP
