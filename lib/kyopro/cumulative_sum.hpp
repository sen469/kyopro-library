#ifndef KYOPRO_CUMULATIVE_SUM_HPP
#define KYOPRO_CUMULATIVE_SUM_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace kyopro {

namespace internal {

template <class T, int D>
struct cumulative_sum_nested_vector {
    using type =
        std::vector<typename cumulative_sum_nested_vector<T, D - 1>::type>;
};

template <class T>
struct cumulative_sum_nested_vector<T, 1> {
    using type = std::vector<T>;
};

}  // namespace internal

template <class T, int D>
class cumulative_sum_nd {
    static_assert(1 <= D, "D must be positive");

public:
    using point_type = std::array<int, D>;
    using value_array =
        typename internal::cumulative_sum_nested_vector<T, D>::type;

private:
    point_type sizes_;
    std::array<std::size_t, D> strides_;
    std::vector<T> values_;
    std::vector<T> data_;

    std::size_t values_size() const {
        std::size_t res = 1;
        for (int size : sizes_) res *= (std::size_t)size;
        return res;
    }

    std::size_t table_size() const {
        std::size_t res = 1;
        for (int size : sizes_) res *= (std::size_t)(size + 1);
        return res;
    }

    void build_strides() {
        std::size_t stride = 1;
        for (int i = D - 1; i >= 0; i--) {
            strides_[i] = stride;
            stride *= (std::size_t)(sizes_[i] + 1);
        }
    }

    std::size_t data_index(const point_type& indexes) const {
        std::size_t res = 0;
        for (int i = 0; i < D; i++) {
            assert(0 <= indexes[i] && indexes[i] <= sizes_[i]);
            res += (std::size_t)indexes[i] * strides_[i];
        }
        return res;
    }

    std::size_t value_index(const std::vector<int>& indexes) const {
        assert((int)indexes.size() == D);
        std::size_t res = 0;
        for (int i = 0; i < D; i++) {
            assert(0 <= indexes[i] && indexes[i] < sizes_[i]);
            res = res * (std::size_t)sizes_[i] + (std::size_t)indexes[i];
        }
        return res;
    }

    point_type to_point(const std::vector<long long>& indexes,
                        bool allow_endpoint) const {
        assert((int)indexes.size() == D);
        point_type res{};
        for (int i = 0; i < D; i++) {
            assert(0 <= indexes[i]);
            assert(indexes[i] <= sizes_[i] - (allow_endpoint ? 0 : 1));
            res[i] = (int)indexes[i];
        }
        return res;
    }

    template <int AXIS, class Vec>
    void flatten(const Vec& v, std::vector<T>& flat) {
        int size = (int)v.size();
        if (sizes_[AXIS] == -1) {
            sizes_[AXIS] = size;
        } else {
            assert(sizes_[AXIS] == size);
        }

        if constexpr (AXIS + 1 == D) {
            for (const T& x : v) flat.push_back(x);
        } else {
            for (const auto& child : v) flatten<AXIS + 1>(child, flat);
        }
    }

    void rebuild() {
        build_strides();
        data_.assign(table_size(), T(0));

        point_type point{};
        point.fill(0);
        for (std::size_t pos = 0; pos < values_.size(); pos++) {
            point_type shifted = point;
            for (int i = 0; i < D; i++) shifted[i]++;
            data_[data_index(shifted)] = values_[pos];

            for (int i = D - 1; i >= 0; i--) {
                point[i]++;
                if (point[i] < sizes_[i]) break;
                point[i] = 0;
            }
        }

        for (int axis = 0; axis < D; axis++) {
            int len = sizes_[axis] + 1;
            std::size_t stride = strides_[axis];
            for (std::size_t i = 0; i < data_.size(); i++) {
                if ((int)((i / stride) % (std::size_t)len) == 0) continue;
                data_[i] = data_[i] + data_[i - stride];
            }
        }
    }

public:
    cumulative_sum_nd() {
        sizes_.fill(0);
        rebuild();
    }

    explicit cumulative_sum_nd(const point_type& sizes) { build(sizes); }

    explicit cumulative_sum_nd(const value_array& v) { build(v); }

    cumulative_sum_nd(const point_type& sizes, const std::vector<T>& v) {
        build(sizes, v);
    }

    void build(const point_type& sizes) {
        sizes_ = sizes;
        for (int size : sizes_) assert(0 <= size);
        values_.assign(values_size(), T(0));
        rebuild();
    }

    void build(const value_array& v) {
        sizes_.fill(-1);
        values_.clear();
        flatten<0>(v, values_);
        for (int& size : sizes_) {
            if (size == -1) size = 0;
        }
        assert(values_.size() == values_size());
        rebuild();
    }

    void build(const point_type& sizes, const std::vector<T>& v) {
        sizes_ = sizes;
        for (int size : sizes_) assert(0 <= size);
        values_ = v;
        assert(values_.size() == values_size());
        rebuild();
    }

    int dimension() const { return D; }

    int size(int axis) const {
        assert(0 <= axis && axis < D);
        return sizes_[axis];
    }

    std::vector<int> sizes() const {
        return std::vector<int>(sizes_.begin(), sizes_.end());
    }

    std::size_t total_size() const { return values_.size(); }

    bool empty() const { return values_.empty(); }

    T get(const std::vector<int>& indexes) const {
        return values_[value_index(indexes)];
    }

    void set(const std::vector<int>& indexes, const T& value) {
        values_[value_index(indexes)] = value;
        rebuild();
    }

    T prefix_sum(const std::vector<long long>& r) const {
        return data_[data_index(to_point(r, true))];
    }

    T prefix_sum(std::initializer_list<long long> r) const {
        return prefix_sum(std::vector<long long>(r));
    }

    T prefix_sum(const point_type& r) const { return data_[data_index(r)]; }

    T sum(const std::vector<long long>& l,
          const std::vector<long long>& r) const {
        assert((int)l.size() == D);
        assert((int)r.size() == D);

        point_type lower{};
        point_type upper{};
        for (int i = 0; i < D; i++) {
            assert(0 <= l[i] && l[i] <= r[i] && r[i] <= sizes_[i]);
            lower[i] = (int)l[i];
            upper[i] = (int)r[i];
        }

        std::size_t subsets = 1;
        for (int i = 0; i < D; i++) subsets *= 2;

        T res = T(0);
        for (std::size_t mask = 0; mask < subsets; mask++) {
            point_type point{};
            int lower_count = 0;
            std::size_t x = mask;
            for (int i = 0; i < D; i++) {
                bool use_lower = x % 2 == 1;
                x /= 2;
                if (use_lower) lower_count++;
                point[i] = use_lower ? lower[i] : upper[i];
            }
            if (lower_count % 2 == 0) {
                res = res + data_[data_index(point)];
            } else {
                res = res - data_[data_index(point)];
            }
        }
        return res;
    }

    T all_sum() const {
        return sum(std::vector<long long>(D, 0),
                   std::vector<long long>(sizes_.begin(), sizes_.end()));
    }
};

}  // namespace kyopro

#endif  // KYOPRO_CUMULATIVE_SUM_HPP
