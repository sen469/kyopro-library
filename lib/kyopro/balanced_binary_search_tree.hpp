#ifndef KYOPRO_BALANCED_BINARY_SEARCH_TREE_HPP
#define KYOPRO_BALANCED_BINARY_SEARCH_TREE_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <utility>
#include <vector>

namespace kyopro {

template <class T, class Compare = std::less<T>>
class balanced_binary_search_tree {
private:
    struct node {
        T key;
        int count;
        int size;
        uint32_t priority;
        node* left;
        node* right;

        node(const T& k, uint32_t p)
            : key(k),
              count(1),
              size(1),
              priority(p),
              left(nullptr),
              right(nullptr) {}
    };

    node* root;
    Compare comp;
    uint32_t rng_state;

    static int size(node* t) { return t ? t->size : 0; }

    bool equal(const T& a, const T& b) const {
        return !comp(a, b) && !comp(b, a);
    }

    static void update(node* t) {
        if (!t) return;
        t->size = size(t->left) + t->count + size(t->right);
    }

    static node* clone(node* t) {
        if (!t) return nullptr;
        node* res = new node(t->key, t->priority);
        res->count = t->count;
        res->size = t->size;
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

    uint32_t next_rand() {
        uint32_t x = rng_state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return rng_state = x;
    }

    node* make_node(const T& x) { return new node(x, next_rand()); }

    static node* rotate_right(node* t) {
        node* s = t->left;
        t->left = s->right;
        s->right = t;
        update(t);
        update(s);
        return s;
    }

    static node* rotate_left(node* t) {
        node* s = t->right;
        t->right = s->left;
        s->left = t;
        update(t);
        update(s);
        return s;
    }

    static node* merge(node* left, node* right) {
        if (!left) return right;
        if (!right) return left;

        if (left->priority < right->priority) {
            left->right = merge(left->right, right);
            update(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }

    node* insert(node* t, const T& x) {
        if (!t) return make_node(x);

        if (equal(x, t->key)) {
            t->count++;
        } else if (comp(x, t->key)) {
            t->left = insert(t->left, x);
            if (t->left->priority < t->priority) t = rotate_right(t);
        } else {
            t->right = insert(t->right, x);
            if (t->right->priority < t->priority) t = rotate_left(t);
        }

        update(t);
        return t;
    }

    node* erase(node* t, const T& x, bool& erased) {
        if (!t) return nullptr;

        if (equal(x, t->key)) {
            erased = true;
            if (t->count > 1) {
                t->count--;
                update(t);
                return t;
            }
            node* res = merge(t->left, t->right);
            t->left = t->right = nullptr;
            delete t;
            return res;
        }

        if (comp(x, t->key)) {
            t->left = erase(t->left, x, erased);
        } else {
            t->right = erase(t->right, x, erased);
        }
        update(t);
        return t;
    }

    node* erase_all(node* t, const T& x, int& erased) {
        if (!t) return nullptr;

        if (equal(x, t->key)) {
            erased = t->count;
            node* res = merge(t->left, t->right);
            t->left = t->right = nullptr;
            delete t;
            return res;
        }

        if (comp(x, t->key)) {
            t->left = erase_all(t->left, x, erased);
        } else {
            t->right = erase_all(t->right, x, erased);
        }
        update(t);
        return t;
    }

    int count(node* t, const T& x) const {
        while (t) {
            if (equal(x, t->key)) return t->count;
            if (comp(x, t->key)) {
                t = t->left;
            } else {
                t = t->right;
            }
        }
        return 0;
    }

    const T& kth(node* t, int k) const {
        int left_size = size(t->left);
        if (k < left_size) return kth(t->left, k);
        if (k < left_size + t->count) return t->key;
        return kth(t->right, k - left_size - t->count);
    }

    void enumerate(node* t, std::vector<T>& res) const {
        if (!t) return;
        enumerate(t->left, res);
        for (int i = 0; i < t->count; i++) res.push_back(t->key);
        enumerate(t->right, res);
    }

public:
    class const_iterator {
    private:
        const balanced_binary_search_tree* tree;
        int index;

        const_iterator(const balanced_binary_search_tree* tree_ptr, int index_value)
            : tree(tree_ptr), index(index_value) {}

        friend class balanced_binary_search_tree;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator() : tree(nullptr), index(0) {}

        reference operator*() const {
            assert(tree);
            assert(0 <= index && index < tree->size());
            return tree->at(index);
        }

        pointer operator->() const { return &**this; }

        const_iterator& operator++() {
            index++;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator res = *this;
            ++*this;
            return res;
        }

        const_iterator& operator--() {
            index--;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator res = *this;
            --*this;
            return res;
        }

        const_iterator& operator+=(difference_type n) {
            index += static_cast<int>(n);
            return *this;
        }

        const_iterator& operator-=(difference_type n) {
            index -= static_cast<int>(n);
            return *this;
        }

        const_iterator operator+(difference_type n) const {
            const_iterator res = *this;
            res += n;
            return res;
        }

        const_iterator operator-(difference_type n) const {
            const_iterator res = *this;
            res -= n;
            return res;
        }

        friend const_iterator operator+(difference_type n, const const_iterator& it) {
            return it + n;
        }

        difference_type operator-(const const_iterator& other) const {
            assert(tree == other.tree);
            return index - other.index;
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        bool operator==(const const_iterator& other) const {
            return tree == other.tree && index == other.index;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const const_iterator& other) const {
            assert(tree == other.tree);
            return index < other.index;
        }

        bool operator<=(const const_iterator& other) const {
            assert(tree == other.tree);
            return index <= other.index;
        }

        bool operator>(const const_iterator& other) const {
            assert(tree == other.tree);
            return index > other.index;
        }

        bool operator>=(const const_iterator& other) const {
            assert(tree == other.tree);
            return index >= other.index;
        }
    };

    using iterator = const_iterator;

    balanced_binary_search_tree()
        : root(nullptr), comp(Compare()), rng_state(2463534242u) {}

    explicit balanced_binary_search_tree(Compare c)
        : root(nullptr), comp(c), rng_state(2463534242u) {}

    explicit balanced_binary_search_tree(const std::vector<T>& v)
        : balanced_binary_search_tree() {
        for (const T& x : v) insert(x);
    }

    balanced_binary_search_tree(const std::vector<T>& v, Compare c)
        : balanced_binary_search_tree(c) {
        for (const T& x : v) insert(x);
    }

    balanced_binary_search_tree(const balanced_binary_search_tree& other)
        : root(clone(other.root)),
          comp(other.comp),
          rng_state(other.rng_state) {}

    balanced_binary_search_tree(balanced_binary_search_tree&& other) noexcept
        : root(other.root),
          comp(std::move(other.comp)),
          rng_state(other.rng_state) {
        other.root = nullptr;
    }

    balanced_binary_search_tree& operator=(balanced_binary_search_tree other) {
        swap(other);
        return *this;
    }

    ~balanced_binary_search_tree() { destroy(root); }

    void swap(balanced_binary_search_tree& other) {
        std::swap(root, other.root);
        std::swap(comp, other.comp);
        std::swap(rng_state, other.rng_state);
    }

    int size() const { return size(root); }
    bool empty() const { return !root; }

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size()); }
    const_iterator begin() const { return const_iterator(this, 0); }
    const_iterator end() const { return const_iterator(this, size()); }
    const_iterator cbegin() const { return const_iterator(this, 0); }
    const_iterator cend() const { return const_iterator(this, size()); }

    void insert(const T& x) { root = insert(root, x); }

    bool erase(const T& x) {
        bool erased = false;
        root = erase(root, x, erased);
        return erased;
    }

    int erase_all(const T& x) {
        int erased = 0;
        root = erase_all(root, x, erased);
        return erased;
    }

    int count(const T& x) const { return count(root, x); }
    bool contains(const T& x) const { return count(x) > 0; }

    const T& at(int k) const {
        assert(0 <= k && k < size());
        return kth(root, k);
    }

    const T& kth(int k) const { return at(k); }

    const T &operator[](int k)
    {
        return kth(root, k);
    }

    int order_of_key(const T& x) const {
        node* t = root;
        int res = 0;

        while (t) {
            if (comp(t->key, x)) {
                res += size(t->left) + t->count;
                t = t->right;
            } else {
                t = t->left;
            }
        }

        return res;
    }

    iterator lower_bound(const T& x) {
        return iterator(this, order_of_key(x));
    }

    const_iterator lower_bound(const T& x) const {
        return const_iterator(this, order_of_key(x));
    }

    std::optional<T> lower_bound_value(const T& x) const {
        node* t = root;
        node* res = nullptr;

        while (t) {
            if (!comp(t->key, x)) {
                res = t;
                t = t->left;
            } else {
                t = t->right;
            }
        }

        if (!res) return std::nullopt;
        return res->key;
    }

    iterator upper_bound(const T& x) {
        node* t = root;
        int res = size();
        int passed = 0;

        while (t) {
            int left_size = size(t->left);
            if (comp(x, t->key)) {
                res = passed + left_size;
                t = t->left;
            } else {
                passed += left_size + t->count;
                t = t->right;
            }
        }

        return iterator(this, res);
    }

    const_iterator upper_bound(const T& x) const {
        node* t = root;
        int res = size();
        int passed = 0;

        while (t) {
            int left_size = size(t->left);
            if (comp(x, t->key)) {
                res = passed + left_size;
                t = t->left;
            } else {
                passed += left_size + t->count;
                t = t->right;
            }
        }

        return const_iterator(this, res);
    }

    std::optional<T> upper_bound_value(const T& x) const {
        node* t = root;
        node* res = nullptr;

        while (t) {
            if (comp(x, t->key)) {
                res = t;
                t = t->left;
            } else {
                t = t->right;
            }
        }

        if (!res) return std::nullopt;
        return res->key;
    }

    std::optional<T> min() const {
        if (!root) return std::nullopt;
        node* t = root;
        while (t->left) t = t->left;
        return t->key;
    }

    std::optional<T> max() const {
        if (!root) return std::nullopt;
        node* t = root;
        while (t->right) t = t->right;
        return t->key;
    }

    std::vector<T> to_vector() const {
        std::vector<T> res;
        res.reserve(size());
        enumerate(root, res);
        return res;
    }

    void clear() {
        destroy(root);
        root = nullptr;
    }
};

template <class T, class Compare>
void swap(balanced_binary_search_tree<T, Compare>& lhs,
          balanced_binary_search_tree<T, Compare>& rhs) {
    lhs.swap(rhs);
}

}  // namespace kyopro

#endif
