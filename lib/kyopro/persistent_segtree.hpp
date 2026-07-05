#ifndef KYOPRO_PERSISTENT_SEGTREE_HPP
#define KYOPRO_PERSISTENT_SEGTREE_HPP

#include <cassert>
#include <memory>
#include <vector>

namespace kyopro {

template <class S, S (*op)(S, S), S (*e)()>
class persistent_segtree {
private:
    struct node {
        S prod;
        int left;
        int right;

        node(const S& prod_, int left_, int right_)
            : prod(prod_), left(left_), right(right_) {}
    };

    int n_;
    int root_;
    std::shared_ptr<std::vector<node>> nodes_;

    persistent_segtree(int n, int root,
                       const std::shared_ptr<std::vector<node>>& nodes)
        : n_(n), root_(root), nodes_(nodes) {}

    S prod_value(int t) const {
        return t == 0 ? e() : (*nodes_)[t].prod;
    }

    int make_node(const S& x, int left = 0, int right = 0) const {
        nodes_->emplace_back(x, left, right);
        return (int)nodes_->size() - 1;
    }

    int build(int l, int r, const std::vector<S>& v) const {
        if (r - l == 1) return make_node(v[l]);
        int m = (l + r) / 2;
        int left = build(l, m, v);
        int right = build(m, r, v);
        return make_node(op(prod_value(left), prod_value(right)), left, right);
    }

    int set(int t, int l, int r, int p, const S& x) const {
        if (r - l == 1) return make_node(x);
        int m = (l + r) / 2;
        int left = t == 0 ? 0 : (*nodes_)[t].left;
        int right = t == 0 ? 0 : (*nodes_)[t].right;
        if (p < m) {
            left = set(left, l, m, p, x);
        } else {
            right = set(right, m, r, p, x);
        }
        return make_node(op(prod_value(left), prod_value(right)), left, right);
    }

    S get(int t, int l, int r, int p) const {
        if (t == 0) return e();
        if (r - l == 1) return (*nodes_)[t].prod;
        int m = (l + r) / 2;
        if (p < m) return get((*nodes_)[t].left, l, m, p);
        return get((*nodes_)[t].right, m, r, p);
    }

    S prod(int t, int l, int r, int ql, int qr) const {
        if (t == 0 || qr <= l || r <= ql) return e();
        if (ql <= l && r <= qr) return (*nodes_)[t].prod;
        int m = (l + r) / 2;
        return op(prod((*nodes_)[t].left, l, m, ql, qr),
                  prod((*nodes_)[t].right, m, r, ql, qr));
    }

    template <class F>
    int max_right(int t, int l, int r, int ql, S& sm, const F& f) const {
        if (r <= ql) return r;
        if (ql <= l) {
            S nxt = op(sm, prod_value(t));
            if (f(nxt)) {
                sm = nxt;
                return r;
            }
            if (r - l == 1) return l;
        }
        int m = (l + r) / 2;
        int left = t == 0 ? 0 : (*nodes_)[t].left;
        int right = t == 0 ? 0 : (*nodes_)[t].right;
        if (ql < m) {
            int res = max_right(left, l, m, ql, sm, f);
            if (res < m) return res;
        }
        return max_right(right, m, r, ql, sm, f);
    }

    template <class F>
    int min_left(int t, int l, int r, int qr, S& sm, const F& f) const {
        if (qr <= l) return l;
        if (r <= qr) {
            S nxt = op(prod_value(t), sm);
            if (f(nxt)) {
                sm = nxt;
                return l;
            }
            if (r - l == 1) return r;
        }
        int m = (l + r) / 2;
        int left = t == 0 ? 0 : (*nodes_)[t].left;
        int right = t == 0 ? 0 : (*nodes_)[t].right;
        if (m < qr) {
            int res = min_left(right, m, r, qr, sm, f);
            if (m < res) return res;
        }
        return min_left(left, l, m, qr, sm, f);
    }

public:
    persistent_segtree()
        : n_(0), root_(0), nodes_(std::make_shared<std::vector<node>>()) {
        nodes_->emplace_back(e(), 0, 0);
    }

    explicit persistent_segtree(int n)
        : n_(n), root_(0), nodes_(std::make_shared<std::vector<node>>()) {
        assert(0 <= n);
        nodes_->emplace_back(e(), 0, 0);
    }

    explicit persistent_segtree(const std::vector<S>& v)
        : n_((int)v.size()),
          root_(0),
          nodes_(std::make_shared<std::vector<node>>()) {
        nodes_->reserve(n_ == 0 ? 1 : 2 * n_);
        nodes_->emplace_back(e(), 0, 0);
        if (n_ > 0) root_ = build(0, n_, v);
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    int node_count() const { return (int)nodes_->size() - 1; }

    persistent_segtree set(int p, const S& x) const {
        assert(0 <= p && p < n_);
        int new_root = set(root_, 0, n_, p, x);
        return persistent_segtree(n_, new_root, nodes_);
    }

    S get(int p) const {
        assert(0 <= p && p < n_);
        return get(root_, 0, n_, p);
    }

    S operator[](int p) const { return get(p); }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        return prod(root_, 0, n_, l, r);
    }

    S all_prod() const { return prod_value(root_); }

    template <bool (*f)(S)>
    int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }

    template <class F>
    int max_right(int l, F f) const {
        assert(0 <= l && l <= n_);
        assert(f(e()));
        if (l == n_) return n_;
        S sm = e();
        return max_right(root_, 0, n_, l, sm, f);
    }

    template <bool (*f)(S)>
    int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }

    template <class F>
    int min_left(int r, F f) const {
        assert(0 <= r && r <= n_);
        assert(f(e()));
        if (r == 0) return 0;
        S sm = e();
        return min_left(root_, 0, n_, r, sm, f);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_PERSISTENT_SEGTREE_HPP
