#ifndef KYOPRO_DYNAMIC_SEGTREE_HPP
#define KYOPRO_DYNAMIC_SEGTREE_HPP

#include <cassert>
#include <utility>

namespace kyopro {

template <class S, S (*op)(S, S), S (*e)()>
class dynamic_segtree {
private:
    struct node {
        S prod;
        node* left;
        node* right;

        explicit node(const S& x) : prod(x), left(nullptr), right(nullptr) {}
    };

    long long left_bound_;
    long long right_bound_;
    node* root;

    static S prod(node* t) { return t ? t->prod : e(); }

    static node* clone(node* t) {
        if (!t) return nullptr;
        node* res = new node(t->prod);
        res->left = clone(t->left);
        res->right = clone(t->right);
        return res;
    }

    static void destroy(node* t) {
        if (!t) return;
        destroy(t->left);
        destroy(t->right);
        delete t;
    }

    static void update(node* t) {
        if (!t) return;
        t->prod = op(prod(t->left), prod(t->right));
    }

    static long long midpoint(long long l, long long r) {
        return l + (r - l) / 2;
    }

    static void set(node*& t, long long l, long long r, long long p,
                    const S& x) {
        if (!t) t = new node(e());
        if (r - l == 1) {
            t->prod = x;
            return;
        }
        long long m = midpoint(l, r);
        if (p < m) {
            set(t->left, l, m, p, x);
        } else {
            set(t->right, m, r, p, x);
        }
        update(t);
    }

    static S get(node* t, long long l, long long r, long long p) {
        if (!t) return e();
        if (r - l == 1) return t->prod;
        long long m = midpoint(l, r);
        if (p < m) return get(t->left, l, m, p);
        return get(t->right, m, r, p);
    }

    static S prod(node* t, long long l, long long r, long long ql,
                  long long qr) {
        if (!t || qr <= l || r <= ql) return e();
        if (ql <= l && r <= qr) return t->prod;
        long long m = midpoint(l, r);
        return op(prod(t->left, l, m, ql, qr),
                  prod(t->right, m, r, ql, qr));
    }

    template <class F>
    static long long max_right(node* t, long long l, long long r,
                               long long ql, S& sm, const F& f) {
        if (r <= ql) return r;
        if (ql <= l) {
            S nxt = op(sm, prod(t));
            if (f(nxt)) {
                sm = nxt;
                return r;
            }
            if (r - l == 1) return l;
        }
        long long m = midpoint(l, r);
        long long res = l;
        if (ql < m) {
            res = max_right(t ? t->left : nullptr, l, m, ql, sm, f);
            if (res < m) return res;
        }
        return max_right(t ? t->right : nullptr, m, r, ql, sm, f);
    }

    template <class F>
    static long long min_left(node* t, long long l, long long r, long long qr,
                              S& sm, const F& f) {
        if (qr <= l) return l;
        if (r <= qr) {
            S nxt = op(prod(t), sm);
            if (f(nxt)) {
                sm = nxt;
                return l;
            }
            if (r - l == 1) return r;
        }
        long long m = midpoint(l, r);
        long long res = r;
        if (m < qr) {
            res = min_left(t ? t->right : nullptr, m, r, qr, sm, f);
            if (m < res) return res;
        }
        return min_left(t ? t->left : nullptr, l, m, qr, sm, f);
    }

public:
    dynamic_segtree() : left_bound_(0), right_bound_(0), root(nullptr) {}

    explicit dynamic_segtree(long long n) : dynamic_segtree(0, n) {}

    dynamic_segtree(long long l, long long r)
        : left_bound_(l), right_bound_(r), root(nullptr) {
        assert(l <= r);
    }

    dynamic_segtree(const dynamic_segtree& other)
        : left_bound_(other.left_bound_),
          right_bound_(other.right_bound_),
          root(clone(other.root)) {}

    dynamic_segtree(dynamic_segtree&& other) noexcept
        : left_bound_(other.left_bound_),
          right_bound_(other.right_bound_),
          root(other.root) {
        other.root = nullptr;
    }

    dynamic_segtree& operator=(dynamic_segtree other) {
        swap(other);
        return *this;
    }

    ~dynamic_segtree() { destroy(root); }

    void swap(dynamic_segtree& other) {
        std::swap(left_bound_, other.left_bound_);
        std::swap(right_bound_, other.right_bound_);
        std::swap(root, other.root);
    }

    long long left_bound() const { return left_bound_; }
    long long right_bound() const { return right_bound_; }
    long long size() const { return right_bound_ - left_bound_; }
    bool empty() const { return left_bound_ == right_bound_; }

    void set(long long p, const S& x) {
        assert(left_bound_ <= p && p < right_bound_);
        set(root, left_bound_, right_bound_, p, x);
    }

    S get(long long p) const {
        assert(left_bound_ <= p && p < right_bound_);
        return get(root, left_bound_, right_bound_, p);
    }

    S operator[](long long p) const { return get(p); }

    S prod(long long l, long long r) const {
        assert(left_bound_ <= l && l <= r && r <= right_bound_);
        return prod(root, left_bound_, right_bound_, l, r);
    }

    S all_prod() const { return prod(root); }

    template <bool (*f)(S)>
    long long max_right(long long l) const {
        return max_right(l, [](S x) { return f(x); });
    }

    template <class F>
    long long max_right(long long l, F f) const {
        assert(left_bound_ <= l && l <= right_bound_);
        assert(f(e()));
        if (l == right_bound_) return right_bound_;
        S sm = e();
        return max_right(root, left_bound_, right_bound_, l, sm, f);
    }

    template <bool (*f)(S)>
    long long min_left(long long r) const {
        return min_left(r, [](S x) { return f(x); });
    }

    template <class F>
    long long min_left(long long r, F f) const {
        assert(left_bound_ <= r && r <= right_bound_);
        assert(f(e()));
        if (r == left_bound_) return left_bound_;
        S sm = e();
        return min_left(root, left_bound_, right_bound_, r, sm, f);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DYNAMIC_SEGTREE_HPP
