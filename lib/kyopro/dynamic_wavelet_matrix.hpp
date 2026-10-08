#ifndef KYOPRO_DYNAMIC_WAVELET_MATRIX_HPP
#define KYOPRO_DYNAMIC_WAVELET_MATRIX_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace kyopro {

namespace internal {

// AVL rope with up to 64 packed bits per leaf. Node 0 is empty.
class dynamic_wavelet_bit_vector {
    struct node {
        std::uint64_t data = 0;
        int left = 0, right = 0;
        int size = 0, ones = 0, height = 0;
    };

    std::vector<node> nodes_{node()};
    std::vector<int> free_;
    int root_ = 0;

    static std::uint64_t mask(int length) {
        return length == 64 ? ~std::uint64_t(0)
                            : (std::uint64_t(1) << length) - 1;
    }

    static int popcount(std::uint64_t data) {
        return __builtin_popcountll(data);
    }

    bool leaf(int v) const { return nodes_[v].left == 0; }

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

    int make_leaf(std::uint64_t data, int length) {
        int v = allocate();
        nodes_[v].data = data;
        nodes_[v].size = length;
        nodes_[v].ones = popcount(data);
        nodes_[v].height = 1;
        return v;
    }

    void pull(int v) {
        int l = nodes_[v].left, r = nodes_[v].right;
        nodes_[v].size = nodes_[l].size + nodes_[r].size;
        nodes_[v].ones = nodes_[l].ones + nodes_[r].ones;
        nodes_[v].height = 1 + std::max(nodes_[l].height, nodes_[r].height);
    }

    int rotate_left(int v) {
        int r = nodes_[v].right;
        nodes_[v].right = nodes_[r].left;
        nodes_[r].left = v;
        pull(v);
        pull(r);
        return r;
    }

    int rotate_right(int v) {
        int l = nodes_[v].left;
        nodes_[v].left = nodes_[l].right;
        nodes_[l].right = v;
        pull(v);
        pull(l);
        return l;
    }

    int balance(int v) {
        int l = nodes_[v].left, r = nodes_[v].right;
        if (leaf(l) && leaf(r) && nodes_[l].size + nodes_[r].size <= 64) {
            nodes_[v].data = nodes_[l].data |
                             (nodes_[r].data << nodes_[l].size);
            nodes_[v].size = nodes_[l].size + nodes_[r].size;
            nodes_[v].ones = nodes_[l].ones + nodes_[r].ones;
            nodes_[v].height = 1;
            nodes_[v].left = nodes_[v].right = 0;
            free_.push_back(l);
            free_.push_back(r);
            return v;
        }
        pull(v);
        if (nodes_[l].height > nodes_[r].height + 1) {
            if (nodes_[nodes_[l].right].height > nodes_[nodes_[l].left].height) {
                nodes_[v].left = rotate_left(l);
            }
            return rotate_right(v);
        }
        if (nodes_[r].height > nodes_[l].height + 1) {
            if (nodes_[nodes_[r].left].height > nodes_[nodes_[r].right].height) {
                nodes_[v].right = rotate_right(r);
            }
            return rotate_left(v);
        }
        return v;
    }

    int build_tree(const std::vector<unsigned char>& bits, int first, int last) {
        if (first == last) return 0;
        if (last - first <= 64) {
            std::uint64_t data = 0;
            for (int i = first; i < last; i++) {
                data |= std::uint64_t(bits[i]) << (i - first);
            }
            return make_leaf(data, last - first);
        }
        int middle = first + (last - first) / 2;
        int l = build_tree(bits, first, middle);
        int r = build_tree(bits, middle, last);
        int v = allocate();
        nodes_[v].left = l;
        nodes_[v].right = r;
        pull(v);
        return v;
    }

    int insert_node(int v, int i, int bit) {
        if (v == 0) return make_leaf(bit, 1);
        if (leaf(v)) {
            std::uint64_t data = nodes_[v].data;
            if (nodes_[v].size < 64) {
                auto low = mask(i);
                nodes_[v].data = (data & low) | ((data & ~low) << 1) |
                                 (std::uint64_t(bit) << i);
                nodes_[v].size++;
                nodes_[v].ones += bit;
                return v;
            }
            // Split 65 bits into leaves of size 32 and 33.
            std::uint64_t low = 0, high = 0;
            for (int j = 0; j < 65; j++) {
                int b = j == i ? bit : (data >> (j - (j > i))) & 1;
                if (j < 32) low |= std::uint64_t(b) << j;
                else high |= std::uint64_t(b) << (j - 32);
            }
            int l = make_leaf(low, 32);
            int r = make_leaf(high, 33);
            nodes_[v].data = 0;
            nodes_[v].left = l;
            nodes_[v].right = r;
            pull(v);
            return v;
        }
        int left_size = nodes_[nodes_[v].left].size;
        if (i < left_size) {
            int l = insert_node(nodes_[v].left, i, bit);
            nodes_[v].left = l;
        } else {
            int r = insert_node(nodes_[v].right, i - left_size, bit);
            nodes_[v].right = r;
        }
        return balance(v);
    }

    int erase_node(int v, int i) {
        if (leaf(v)) {
            auto data = nodes_[v].data;
            int bit = (data >> i) & 1;
            auto high = i == 63 ? 0 : data >> (i + 1);
            nodes_[v].data = (data & mask(i)) | (high << i);
            nodes_[v].size--;
            nodes_[v].ones -= bit;
            if (nodes_[v].size == 0) {
                free_.push_back(v);
                return 0;
            }
            return v;
        }
        int left_size = nodes_[nodes_[v].left].size;
        if (i < left_size) {
            nodes_[v].left = erase_node(nodes_[v].left, i);
        } else {
            nodes_[v].right = erase_node(nodes_[v].right, i - left_size);
        }
        int l = nodes_[v].left, r = nodes_[v].right;
        if (l == 0 || r == 0) {
            free_.push_back(v);
            return l == 0 ? r : l;
        }
        return balance(v);
    }

public:
    void build(const std::vector<unsigned char>& bits) {
        assert(bits.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        nodes_.assign(1, node());
        free_.clear();
        root_ = build_tree(bits, 0, static_cast<int>(bits.size()));
    }

    int size() const { return nodes_[root_].size; }

    int access(int i) const {
        assert(0 <= i && i < size());
        int v = root_;
        while (!leaf(v)) {
            int l = nodes_[v].left;
            if (i < nodes_[l].size) v = l;
            else {
                i -= nodes_[l].size;
                v = nodes_[v].right;
            }
        }
        return (nodes_[v].data >> i) & 1;
    }

    int rank1(int r) const {
        assert(0 <= r && r <= size());
        int v = root_, result = 0;
        while (!leaf(v)) {
            int l = nodes_[v].left;
            if (r <= nodes_[l].size) v = l;
            else {
                r -= nodes_[l].size;
                result += nodes_[l].ones;
                v = nodes_[v].right;
            }
        }
        return result + popcount(nodes_[v].data & mask(r));
    }

    void insert(int i, int bit) {
        assert(0 <= i && i <= size());
        assert(size() < std::numeric_limits<int>::max());
        assert(bit == 0 || bit == 1);
        root_ = insert_node(root_, i, bit);
    }

    void erase(int i) {
        assert(0 <= i && i < size());
        root_ = erase_node(root_, i);
    }
};

}  // namespace internal

template <class T = long long>
class dynamic_wavelet_matrix {
    static_assert(std::is_integral<T>::value && !std::is_same<T, bool>::value,
                  "dynamic_wavelet_matrix<T>: T must be an integer type other than bool");
    using U = typename std::make_unsigned<T>::type;
    static constexpr int bits = std::numeric_limits<U>::digits;

    int n_ = 0;
    std::vector<internal::dynamic_wavelet_bit_vector> matrix_{bits};
    std::vector<int> zeros_ = std::vector<int>(bits, 0);

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

    void check_range(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        (void)l;
        (void)r;
    }

public:
    dynamic_wavelet_matrix() = default;

    explicit dynamic_wavelet_matrix(const std::vector<T>& a) { build(a); }

    void build(const std::vector<T>& a) {
        assert(a.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
        n_ = static_cast<int>(a.size());
        std::vector<U> current(n_), next(n_);
        for (int i = 0; i < n_; i++) current[i] = encode(a[i]);
        std::vector<unsigned char> level_bits(n_);
        for (int depth = 0; depth < bits; depth++) {
            int bit = bits - 1 - depth;
            int zero_count = 0;
            for (int i = 0; i < n_; i++) {
                level_bits[i] = (current[i] >> bit) & U(1);
                zero_count += level_bits[i] == 0;
            }
            zeros_[depth] = zero_count;
            matrix_[depth].build(level_bits);
            int zero_pos = 0, one_pos = zero_count;
            for (int i = 0; i < n_; i++) {
                if (level_bits[i]) next[one_pos++] = current[i];
                else next[zero_pos++] = current[i];
            }
            current.swap(next);
        }
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    T access(int i) const {
        assert(0 <= i && i < n_);
        U key = 0;
        for (int depth = 0; depth < bits; depth++) {
            int bit = matrix_[depth].access(i);
            int ones = matrix_[depth].rank1(i);
            if (bit) {
                key |= U(1) << (bits - 1 - depth);
                i = zeros_[depth] + ones;
            } else {
                i -= ones;
            }
        }
        return decode(key);
    }

    T get(int i) const { return access(i); }

    T operator[](int i) const { return access(i); }

    void insert(int i, T x) {
        assert(0 <= i && i <= n_);
        assert(n_ < std::numeric_limits<int>::max());
        U key = encode(x);
        for (int depth = 0; depth < bits; depth++) {
            int bit = (key >> (bits - 1 - depth)) & U(1);
            int ones = matrix_[depth].rank1(i);
            matrix_[depth].insert(i, bit);
            if (bit) i = zeros_[depth] + ones;
            else {
                i -= ones;
                zeros_[depth]++;
            }
        }
        n_++;
    }

    void push_back(T x) { insert(n_, x); }

    void erase(int i) {
        assert(0 <= i && i < n_);
        for (int depth = 0; depth < bits; depth++) {
            int bit = matrix_[depth].access(i);
            int ones = matrix_[depth].rank1(i);
            int next = bit ? zeros_[depth] + ones : i - ones;
            matrix_[depth].erase(i);
            if (!bit) zeros_[depth]--;
            i = next;
        }
        n_--;
    }

    void set(int i, T x) {
        assert(0 <= i && i < n_);
        erase(i);
        insert(i, x);
    }

    T kth_smallest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        U key = 0;
        for (int depth = 0; depth < bits; depth++) {
            int ones_l = matrix_[depth].rank1(l);
            int ones_r = matrix_[depth].rank1(r);
            int zero_count = (r - l) - (ones_r - ones_l);
            if (k < zero_count) {
                l -= ones_l;
                r -= ones_r;
            } else {
                k -= zero_count;
                key |= U(1) << (bits - 1 - depth);
                l = zeros_[depth] + ones_l;
                r = zeros_[depth] + ones_r;
            }
        }
        return decode(key);
    }

    T kth_largest(int l, int r, int k) const {
        check_range(l, r);
        assert(0 <= k && k < r - l);
        return kth_smallest(l, r, r - l - 1 - k);
    }

    int range_freq(int l, int r, T upper) const {
        check_range(l, r);
        U key = encode(upper);
        int result = 0;
        for (int depth = 0; depth < bits; depth++) {
            int ones_l = matrix_[depth].rank1(l);
            int ones_r = matrix_[depth].rank1(r);
            if ((key >> (bits - 1 - depth)) & U(1)) {
                result += (r - l) - (ones_r - ones_l);
                l = zeros_[depth] + ones_l;
                r = zeros_[depth] + ones_r;
            } else {
                l -= ones_l;
                r -= ones_r;
            }
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
        U key = encode(x);
        for (int depth = 0; depth < bits; depth++) {
            int ones_l = matrix_[depth].rank1(l);
            int ones_r = matrix_[depth].rank1(r);
            if ((key >> (bits - 1 - depth)) & U(1)) {
                l = zeros_[depth] + ones_l;
                r = zeros_[depth] + ones_r;
            } else {
                l -= ones_l;
                r -= ones_r;
            }
        }
        return r - l;
    }

    T prev_value(int l, int r, T upper) const {
        int k = range_freq(l, r, upper);
        assert(k > 0);
        return kth_smallest(l, r, k - 1);
    }

    T next_value(int l, int r, T lower) const {
        int k = range_freq(l, r, lower);
        assert(k < r - l);
        return kth_smallest(l, r, k);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DYNAMIC_WAVELET_MATRIX_HPP
