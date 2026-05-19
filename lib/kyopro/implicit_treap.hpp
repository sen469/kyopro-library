#ifndef KYOPRO_IMPLICIT_TREAP_HPP
#define KYOPRO_IMPLICIT_TREAP_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

namespace kyopro {

template <class S, S (*op)(S, S), S (*e)(), class F,
          S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
class implicit_treap {
private:
    struct node {
        S val;
        S prod;
        S rprod;
        F lazy;
        node* left;
        node* right;
        int size;
        uint32_t priority;
        bool rev;
        bool lazy_flag;

        node(const S& v, uint32_t p)
            : val(v),
              prod(v),
              rprod(v),
              lazy(id()),
              left(nullptr),
              right(nullptr),
              size(1),
              priority(p),
              rev(false),
              lazy_flag(false) {}
    };

    node* root;
    uint32_t rng_state;

    static int size(node* t) { return t ? t->size : 0; }
    static S prod(node* t) { return t ? t->prod : e(); }
    static S rprod(node* t) { return t ? t->rprod : e(); }

    static node* clone(node* t) {
        if (!t) return nullptr;
        node* res = new node(t->val, t->priority);
        res->prod = t->prod;
        res->rprod = t->rprod;
        res->lazy = t->lazy;
        res->left = clone(t->left);
        res->right = clone(t->right);
        res->size = t->size;
        res->rev = t->rev;
        res->lazy_flag = t->lazy_flag;
        return res;
    }

    static void destroy(node* t) {
        if (!t) return;
        destroy(t->left);
        destroy(t->right);
        delete t;
    }

    uint32_t next_rand() {
        uint32_t x = rng_state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return rng_state = x;
    }

    node* make_node(const S& x) { return new node(x, next_rand()); }

    static void update(node* t) {
        if (!t) return;
        t->size = 1 + size(t->left) + size(t->right);
        t->prod = op(op(prod(t->left), t->val), prod(t->right));
        t->rprod = op(op(rprod(t->right), t->val), rprod(t->left));
    }

    static void all_apply(node* t, F f) {
        if (!t) return;
        t->val = mapping(f, t->val);
        t->prod = mapping(f, t->prod);
        t->rprod = mapping(f, t->rprod);
        t->lazy = composition(f, t->lazy);
        t->lazy_flag = true;
    }

    static void all_reverse(node* t) {
        if (!t) return;
        std::swap(t->left, t->right);
        std::swap(t->prod, t->rprod);
        t->rev = !t->rev;
    }

    static void push(node* t) {
        if (!t) return;
        if (t->rev) {
            all_reverse(t->left);
            all_reverse(t->right);
            t->rev = false;
        }
        if (t->lazy_flag) {
            all_apply(t->left, t->lazy);
            all_apply(t->right, t->lazy);
            t->lazy = id();
            t->lazy_flag = false;
        }
    }

    static node* merge(node* left, node* right) {
        if (!left) return right;
        if (!right) return left;
        if (left->priority < right->priority) {
            push(left);
            left->right = merge(left->right, right);
            update(left);
            return left;
        } else {
            push(right);
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }

    static std::pair<node*, node*> split(node* t, int k) {
        if (!t) return {nullptr, nullptr};
        push(t);
        if (k <= size(t->left)) {
            auto [l, r] = split(t->left, k);
            t->left = r;
            update(t);
            return {l, t};
        } else {
            auto [l, r] = split(t->right, k - size(t->left) - 1);
            t->right = l;
            update(t);
            return {t, r};
        }
    }

    static S get(node* t, int p) {
        push(t);
        int left_size = size(t->left);
        if (p < left_size) return get(t->left, p);
        if (p == left_size) return t->val;
        return get(t->right, p - left_size - 1);
    }

    static void set(node* t, int p, const S& x) {
        push(t);
        int left_size = size(t->left);
        if (p < left_size) {
            set(t->left, p, x);
        } else if (p == left_size) {
            t->val = x;
        } else {
            set(t->right, p - left_size - 1, x);
        }
        update(t);
    }

    static void enumerate(node* t, std::vector<S>& res) {
        if (!t) return;
        push(t);
        enumerate(t->left, res);
        res.push_back(t->val);
        enumerate(t->right, res);
        update(t);
    }

public:
    implicit_treap() : root(nullptr), rng_state(2463534242u) {}

    explicit implicit_treap(const std::vector<S>& v) : implicit_treap() {
        for (const S& x : v) push_back(x);
    }

    implicit_treap(const implicit_treap& other)
        : root(clone(other.root)), rng_state(other.rng_state) {}

    implicit_treap(implicit_treap&& other) noexcept
        : root(other.root), rng_state(other.rng_state) {
        other.root = nullptr;
    }

    implicit_treap& operator=(implicit_treap other) {
        swap(other);
        return *this;
    }

    ~implicit_treap() { destroy(root); }

    void swap(implicit_treap& other) {
        std::swap(root, other.root);
        std::swap(rng_state, other.rng_state);
    }

    int size() const { return size(root); }
    bool empty() const { return !root; }

    S get(int p) {
        assert(0 <= p && p < size());
        return get(root, p);
    }

    S operator[](int p) { return get(p); }

    void set(int p, const S& x) {
        assert(0 <= p && p < size());
        set(root, p, x);
    }

    S all_prod() const { return prod(root); }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        S res = prod(b);
        root = merge(merge(a, b), c);
        return res;
    }

    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        all_apply(b, f);
        root = merge(merge(a, b), c);
    }

    void reverse(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        all_reverse(b);
        root = merge(merge(a, b), c);
    }

    void insert(int p, const S& x) {
        assert(0 <= p && p <= size());
        auto [a, b] = split(root, p);
        root = merge(merge(a, make_node(x)), b);
    }

    void push_front(const S& x) { insert(0, x); }
    void push_back(const S& x) { insert(size(), x); }

    S erase(int p) {
        assert(0 <= p && p < size());
        auto [a, bc] = split(root, p);
        auto [b, c] = split(bc, 1);
        S res = b->val;
        b->left = b->right = nullptr;
        destroy(b);
        root = merge(a, c);
        return res;
    }

    void erase(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        destroy(b);
        root = merge(a, c);
    }

    std::vector<S> to_vector() {
        std::vector<S> res;
        res.reserve(size());
        enumerate(root, res);
        return res;
    }

    void clear() {
        destroy(root);
        root = nullptr;
    }
};

template <class S, S (*op)(S, S), S (*e)(), class F,
          S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
void swap(implicit_treap<S, op, e, F, mapping, composition, id>& lhs,
          implicit_treap<S, op, e, F, mapping, composition, id>& rhs) {
    lhs.swap(rhs);
}

}  // namespace kyopro

#endif
