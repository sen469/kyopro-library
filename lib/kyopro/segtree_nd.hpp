#ifndef KYOPRO_SEGTREE_ND_HPP
#define KYOPRO_SEGTREE_ND_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace kyopro {

namespace internal {

template <class T, int D>
struct segtree_nested_vector {
    using type = std::vector<typename segtree_nested_vector<T, D - 1>::type>;
};

template <class T>
struct segtree_nested_vector<T, 1> {
    using type = std::vector<T>;
};

}  // namespace internal

template <class S, int D, S (*op)(S, S), S (*e)()>
class segtree_nd {
    static_assert(1 <= D, "D must be positive");

public:
    using point_type = std::array<int, D>;
    using value_array = typename internal::segtree_nested_vector<S, D>::type;

private:
    point_type sizes_;
    point_type seg_sizes_;
    std::array<std::size_t, D> strides_;
    std::vector<S> data_;

    std::size_t original_size() const {
        std::size_t res = 1;
        for (int size : sizes_) res *= (std::size_t)size;
        return res;
    }

    void build_layout() {
        for (int i = 0; i < D; i++) {
            assert(0 <= sizes_[i]);
            seg_sizes_[i] = 1;
            while (seg_sizes_[i] < sizes_[i]) seg_sizes_[i] <<= 1;
        }

        std::size_t total = 1;
        for (int i = D - 1; i >= 0; i--) {
            strides_[i] = total;
            total *= (std::size_t)(2 * seg_sizes_[i]);
        }
        data_.assign(total, e());
    }

    std::size_t index(const point_type& p) const {
        std::size_t res = 0;
        for (int i = 0; i < D; i++) {
            assert(0 <= p[i] && p[i] < 2 * seg_sizes_[i]);
            res += (std::size_t)p[i] * strides_[i];
        }
        return res;
    }

    std::size_t value_index(const std::vector<int>& p) const {
        assert((int)p.size() == D);
        std::size_t res = 0;
        for (int i = 0; i < D; i++) {
            assert(0 <= p[i] && p[i] < sizes_[i]);
            res = res * (std::size_t)sizes_[i] + (std::size_t)p[i];
        }
        return res;
    }

    template <int AXIS, class Vec>
    void flatten(const Vec& v, std::vector<S>& flat) {
        int size = (int)v.size();
        if (sizes_[AXIS] == -1) {
            sizes_[AXIS] = size;
        } else {
            assert(sizes_[AXIS] == size);
        }

        if constexpr (AXIS + 1 == D) {
            for (const S& x : v) flat.push_back(x);
        } else {
            for (const auto& child : v) flatten<AXIS + 1>(child, flat);
        }
    }

    void build_axis(int axis) {
        point_type p{};
        auto dfs = [&](auto&& self, int dim) -> void {
            if (dim == D) {
                point_type q = p;
                for (int node = seg_sizes_[axis] - 1; node >= 1; node--) {
                    q[axis] = node;
                    point_type left = q;
                    point_type right = q;
                    left[axis] = 2 * node;
                    right[axis] = 2 * node + 1;
                    data_[index(q)] =
                        op(data_[index(left)], data_[index(right)]);
                }
                return;
            }
            if (dim == axis) {
                self(self, dim + 1);
                return;
            }
            for (int node = 1; node < 2 * seg_sizes_[dim]; node++) {
                p[dim] = node;
                self(self, dim + 1);
            }
        };
        dfs(dfs, 0);
    }

    S all_prod_impl() const {
        point_type root{};
        root.fill(1);
        return data_[index(root)];
    }

    void build_from_flat(const std::vector<S>& v) {
        assert(v.size() == original_size());
        build_layout();

        point_type point{};
        point.fill(0);
        for (std::size_t pos = 0; pos < v.size(); pos++) {
            point_type leaf = point;
            for (int i = 0; i < D; i++) leaf[i] += seg_sizes_[i];
            data_[index(leaf)] = v[pos];

            for (int i = D - 1; i >= 0; i--) {
                point[i]++;
                if (point[i] < sizes_[i]) break;
                point[i] = 0;
            }
        }

        for (int axis = D - 1; axis >= 0; axis--) build_axis(axis);
    }

    std::vector<int> to_vector(std::initializer_list<int> p) const {
        return std::vector<int>(p);
    }

    std::vector<int> to_vector(std::initializer_list<long long> p) const {
        std::vector<int> res;
        for (long long x : p) res.push_back((int)x);
        return res;
    }

    std::vector<std::vector<int>> prod_nodes(const std::vector<int>& l,
                                             const std::vector<int>& r) const {
        assert((int)l.size() == D);
        assert((int)r.size() == D);
        std::vector<std::vector<int>> nodes(D);
        for (int axis = 0; axis < D; axis++) {
            assert(0 <= l[axis] && l[axis] <= r[axis] && r[axis] <= sizes_[axis]);
            int left = l[axis] + seg_sizes_[axis];
            int right = r[axis] + seg_sizes_[axis];
            while (left < right) {
                if (left & 1) nodes[axis].push_back(left++);
                if (right & 1) nodes[axis].push_back(--right);
                left >>= 1;
                right >>= 1;
            }
        }
        return nodes;
    }

    S prod_dfs(int axis, point_type& p,
               const std::vector<std::vector<int>>& nodes) const {
        if (axis == D) return data_[index(p)];
        S res = e();
        for (int node : nodes[axis]) {
            p[axis] = node;
            res = op(res, prod_dfs(axis + 1, p, nodes));
        }
        return res;
    }

public:
    segtree_nd() {
        sizes_.fill(0);
        build(std::vector<int>(D, 0));
    }

    explicit segtree_nd(const std::vector<int>& sizes) { build(sizes); }

    explicit segtree_nd(const point_type& sizes) {
        build(std::vector<int>(sizes.begin(), sizes.end()));
    }

    explicit segtree_nd(const value_array& v) { build(v); }

    segtree_nd(const std::vector<int>& sizes, const std::vector<S>& v) {
        build(sizes, v);
    }

    segtree_nd(const point_type& sizes, const std::vector<S>& v) {
        build(std::vector<int>(sizes.begin(), sizes.end()), v);
    }

    void build(const std::vector<int>& sizes) {
        assert((int)sizes.size() == D);
        sizes_ = {};
        for (int i = 0; i < D; i++) sizes_[i] = sizes[i];
        std::vector<S> v(original_size(), e());
        build_from_flat(v);
    }

    void build(const value_array& v) {
        sizes_.fill(-1);
        std::vector<S> flat;
        flatten<0>(v, flat);
        for (int& size : sizes_) {
            if (size == -1) size = 0;
        }
        build_from_flat(flat);
    }

    void build(const std::vector<int>& sizes, const std::vector<S>& v) {
        assert((int)sizes.size() == D);
        sizes_ = {};
        for (int i = 0; i < D; i++) sizes_[i] = sizes[i];
        build_from_flat(v);
    }

    int dimension() const { return D; }

    int size(int axis) const {
        assert(0 <= axis && axis < D);
        return sizes_[axis];
    }

    std::vector<int> sizes() const {
        return std::vector<int>(sizes_.begin(), sizes_.end());
    }

    std::size_t total_size() const { return original_size(); }

    bool empty() const { return total_size() == 0; }

    void set(const std::vector<int>& p, const S& value) {
        assert((int)p.size() == D);
        point_type leaf{};
        std::vector<std::vector<int>> paths(D);
        for (int i = 0; i < D; i++) {
            assert(0 <= p[i] && p[i] < sizes_[i]);
            int node = p[i] + seg_sizes_[i];
            leaf[i] = node;
            while (node >= 1) {
                paths[i].push_back(node);
                node >>= 1;
            }
        }

        data_[index(leaf)] = value;
        int max_total = 0;
        for (int i = 0; i < D; i++) max_total += (int)paths[i].size() - 1;

        std::vector<int> dist(D, 0);
        auto update = [&](const std::vector<int>& d) {
            point_type q{};
            int axis = -1;
            for (int i = 0; i < D; i++) {
                q[i] = paths[i][d[i]];
                if (axis == -1 && d[i] > 0) axis = i;
            }
            if (axis == -1) return;
            point_type left = q;
            point_type right = q;
            left[axis] = 2 * q[axis];
            right[axis] = 2 * q[axis] + 1;
            data_[index(q)] = op(data_[index(left)], data_[index(right)]);
        };

        auto dfs = [&](auto&& self, int axis, int remaining) -> void {
            if (axis == D) {
                if (remaining == 0) update(dist);
                return;
            }
            int limit = (int)paths[axis].size() - 1;
            for (int d = 0; d <= limit && d <= remaining; d++) {
                dist[axis] = d;
                self(self, axis + 1, remaining - d);
            }
        };

        for (int total = 1; total <= max_total; total++) dfs(dfs, 0, total);
    }

    void set(std::initializer_list<int> p, const S& value) {
        set(to_vector(p), value);
    }

    S get(const std::vector<int>& p) const {
        assert((int)p.size() == D);
        point_type leaf{};
        for (int i = 0; i < D; i++) {
            assert(0 <= p[i] && p[i] < sizes_[i]);
            leaf[i] = p[i] + seg_sizes_[i];
        }
        return data_[index(leaf)];
    }

    S get(std::initializer_list<int> p) const { return get(to_vector(p)); }

    S prod(const std::vector<int>& l, const std::vector<int>& r) const {
        auto nodes = prod_nodes(l, r);
        for (const auto& v : nodes) {
            if (v.empty()) return e();
        }
        point_type p{};
        return prod_dfs(0, p, nodes);
    }

    S prod(std::initializer_list<int> l, std::initializer_list<int> r) const {
        return prod(to_vector(l), to_vector(r));
    }

    S all_prod() const { return all_prod_impl(); }
};

}  // namespace kyopro

#endif  // KYOPRO_SEGTREE_ND_HPP
