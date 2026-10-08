#ifndef KYOPRO_DYNAMIC_RANGE_KTH_HPP
#define KYOPRO_DYNAMIC_RANGE_KTH_HPP

#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

namespace kyopro {

// Fenwick tree of binary tries. Update values need no coordinate compression.
template <class T = long long>
class dynamic_range_kth {
    static_assert(std::is_integral<T>::value && !std::is_same<T, bool>::value,
                  "dynamic_range_kth<T>: T must be an integer type other than bool");
    using U = typename std::make_unsigned<T>::type;
    static constexpr int bits = std::numeric_limits<U>::digits;

    struct node {
        int child[2] = {0, 0};
        int count = 0;
    };

    std::vector<T> values_;
    std::vector<int> roots_{0};
    std::vector<node> nodes_{node()};  // Node 0 is the empty subtree.
    std::vector<int> free_;

    static U encode(T x) {
        U key = static_cast<U>(x);
        if constexpr (std::is_signed<T>::value) key ^= U(1) << (bits - 1);
        return key;
    }

    static T decode(U key) {
        if constexpr (std::is_signed<T>::value) {
            U sign = U(1) << (bits - 1);
            if (key < sign) {
                return std::numeric_limits<T>::min() + static_cast<T>(key);
            }
            return static_cast<T>(key - sign);
        } else {
            return key;
        }
    }

    int allocate() {
        if (!free_.empty()) {
            int v = free_.back();
            free_.pop_back();
            nodes_[v] = node();
            return v;
        }
        assert(nodes_.size() < static_cast<std::size_t>(std::numeric_limits<int>::max()));
        nodes_.push_back(node());
        return static_cast<int>(nodes_.size()) - 1;
    }

    int change(int v, U key, int bit, int delta) {
        if (v == 0) {
            assert(delta == 1);
            v = allocate();
        }
        nodes_[v].count += delta;
        if (bit >= 0) {
            int branch = (key >> bit) & U(1);
            int child = change(nodes_[v].child[branch], key, bit - 1, delta);
            // change() may reallocate nodes_, so no references survive it.
            nodes_[v].child[branch] = child;
        }
        if (nodes_[v].count == 0) {
            free_.push_back(v);
            return 0;
        }
        return v;
    }

    void add(int i, T x, int delta) {
        U key = encode(x);
        for (std::size_t j = static_cast<std::size_t>(i) + 1;
             j < roots_.size(); j += j & -j) {
            roots_[j] = change(roots_[j], key, bits - 1, delta);
        }
    }

    std::vector<int> prefix_roots(int r) const {
        std::vector<int> result;
        for (; r > 0; r -= r & -r) result.push_back(roots_[r]);
        return result;
    }

    int child_count(const std::vector<int>& roots, int branch) const {
        int result = 0;
        for (int v : roots) result += nodes_[nodes_[v].child[branch]].count;
        return result;
    }

    void descend(std::vector<int>& roots, int branch) const {
        for (int& v : roots) v = nodes_[v].child[branch];
    }

    void check_range(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        (void)l;
        (void)r;
    }

public:
    dynamic_range_kth() = default;

    explicit dynamic_range_kth(const std::vector<T>& a) { build(a); }

    void build(const std::vector<T>& a) {
        assert(a.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        values_ = a;
        roots_.assign(a.size() + 1, 0);
        nodes_.assign(1, node());
        free_.clear();
        for (int i = 0; i < size(); i++) add(i, values_[i], 1);
    }

    int size() const { return static_cast<int>(values_.size()); }

    bool empty() const { return values_.empty(); }

    T get(int i) const {
        assert(0 <= i && i < size());
        return values_[i];
    }

    T operator[](int i) const { return get(i); }

    void set(int i, T x) {
        assert(0 <= i && i < size());
        if (values_[i] == x) return;
        add(i, values_[i], -1);
        add(i, x, 1);
        values_[i] = x;
    }

    T kth_smallest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        auto left = prefix_roots(l);
        auto right = prefix_roots(r);
        U key = 0;
        for (int bit = bits - 1; bit >= 0; bit--) {
            int zeros = child_count(right, 0) - child_count(left, 0);
            int branch = k >= zeros;
            if (branch) {
                k -= zeros;
                key |= U(1) << bit;
            }
            descend(left, branch);
            descend(right, branch);
        }
        return decode(key);
    }

    T kth_largest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        return kth_smallest(l, r, r - l - 1 - k);
    }

    // Number of values strictly smaller than upper in [l, r).
    int range_freq(int l, int r, T upper) const {
        check_range(l, r);
        auto left = prefix_roots(l);
        auto right = prefix_roots(r);
        U key = encode(upper);
        int result = 0;
        for (int bit = bits - 1; bit >= 0; bit--) {
            int branch = (key >> bit) & U(1);
            if (branch) result += child_count(right, 0) - child_count(left, 0);
            descend(left, branch);
            descend(right, branch);
        }
        return result;
    }

    int range_freq(int l, int r, T lower, T upper) const {
        check_range(l, r);
        if (!(lower < upper)) return 0;
        return range_freq(l, r, upper) - range_freq(l, r, lower);
    }

    int count(int l, int r, T x) const {
        check_range(l, r);
        auto left = prefix_roots(l);
        auto right = prefix_roots(r);
        U key = encode(x);
        for (int bit = bits - 1; bit >= 0; bit--) {
            int branch = (key >> bit) & U(1);
            descend(left, branch);
            descend(right, branch);
        }
        int result = 0;
        for (int v : right) result += nodes_[v].count;
        for (int v : left) result -= nodes_[v].count;
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DYNAMIC_RANGE_KTH_HPP
