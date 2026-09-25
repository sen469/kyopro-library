#ifndef KYOPRO_BINARY_TRIE_HPP
#define KYOPRO_BINARY_TRIE_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace kyopro {

template <class T = std::uint32_t, int BIT_SIZE = std::numeric_limits<T>::digits>
class binary_trie {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");
    static_assert(0 < BIT_SIZE, "BIT_SIZE must be positive");
    static_assert(BIT_SIZE <= std::numeric_limits<T>::digits,
                  "BIT_SIZE must not exceed the number of value bits in T");

private:
    struct node {
        std::array<int, 2> next;
        int count;

        node() : next{{-1, -1}}, count(0) {}
    };

    std::vector<node> nodes_;
    T xor_all_;

    static int bit(T x, int k) { return (int)((x >> k) & T(1)); }

    static bool in_range(T x) {
        if constexpr (BIT_SIZE == std::numeric_limits<T>::digits) {
            return true;
        } else {
            return x < (T(1) << BIT_SIZE);
        }
    }

    bool alive(int v) const { return v != -1 && nodes_[v].count > 0; }

public:
    binary_trie() : nodes_(1), xor_all_(0) {}

    void insert(T x) {
        assert(in_range(x));
        x ^= xor_all_;

        int v = 0;
        nodes_[v].count++;
        for (int k = BIT_SIZE - 1; k >= 0; k--) {
            int b = bit(x, k);
            if (nodes_[v].next[b] == -1) {
                nodes_[v].next[b] = (int)nodes_.size();
                nodes_.push_back(node());
            }
            v = nodes_[v].next[b];
            nodes_[v].count++;
        }
    }

    bool erase(T x) {
        assert(in_range(x));
        if (!contains(x)) return false;
        x ^= xor_all_;

        int v = 0;
        nodes_[v].count--;
        for (int k = BIT_SIZE - 1; k >= 0; k--) {
            v = nodes_[v].next[bit(x, k)];
            nodes_[v].count--;
        }
        return true;
    }

    int count(T x) const {
        assert(in_range(x));
        x ^= xor_all_;

        int v = 0;
        for (int k = BIT_SIZE - 1; k >= 0; k--) {
            v = nodes_[v].next[bit(x, k)];
            if (v == -1) return 0;
        }
        return nodes_[v].count;
    }

    bool contains(T x) const { return count(x) > 0; }

    int size() const { return nodes_[0].count; }

    bool empty() const { return size() == 0; }

    T kth_element(int k) const {
        assert(0 <= k && k < size());

        int v = 0;
        T result = 0;
        for (int i = BIT_SIZE - 1; i >= 0; i--) {
            int zero = bit(xor_all_, i);
            int zero_child = nodes_[v].next[zero];
            int zero_count = alive(zero_child) ? nodes_[zero_child].count : 0;
            if (k < zero_count) {
                v = zero_child;
            } else {
                k -= zero_count;
                v = nodes_[v].next[zero ^ 1];
                result |= T(1) << i;
            }
        }
        return result;
    }

    T min_element() const {
        assert(!empty());
        return kth_element(0);
    }

    T max_element() const {
        assert(!empty());
        return kth_element(size() - 1);
    }

    int count_less(T x) const {
        assert(in_range(x));

        int v = 0;
        int result = 0;
        for (int i = BIT_SIZE - 1; i >= 0 && v != -1; i--) {
            int zero = bit(xor_all_, i);
            if (bit(x, i)) {
                int zero_child = nodes_[v].next[zero];
                if (alive(zero_child)) result += nodes_[zero_child].count;
                v = nodes_[v].next[zero ^ 1];
            } else {
                v = nodes_[v].next[zero];
            }
        }
        return result;
    }

    T min_xor_element(T x) const {
        assert(in_range(x));
        assert(!empty());

        int v = 0;
        T result = 0;
        for (int i = BIT_SIZE - 1; i >= 0; i--) {
            int preferred = bit(xor_all_, i) ^ bit(x, i);
            int chosen = alive(nodes_[v].next[preferred]) ? preferred : (preferred ^ 1);
            v = nodes_[v].next[chosen];
            if ((chosen ^ bit(xor_all_, i)) != 0) result |= T(1) << i;
        }
        return result;
    }

    T max_xor_element(T x) const {
        assert(in_range(x));
        assert(!empty());

        int v = 0;
        T result = 0;
        for (int i = BIT_SIZE - 1; i >= 0; i--) {
            int preferred = bit(xor_all_, i) ^ bit(x, i) ^ 1;
            int chosen = alive(nodes_[v].next[preferred]) ? preferred : (preferred ^ 1);
            v = nodes_[v].next[chosen];
            if ((chosen ^ bit(xor_all_, i)) != 0) result |= T(1) << i;
        }
        return result;
    }

    T min_xor(T x) const { return min_xor_element(x) ^ x; }

    T max_xor(T x) const { return max_xor_element(x) ^ x; }

    void apply_xor(T x) {
        assert(in_range(x));
        xor_all_ ^= x;
    }

    int node_count() const { return (int)nodes_.size(); }
};

}  // namespace kyopro

#endif  // KYOPRO_BINARY_TRIE_HPP
