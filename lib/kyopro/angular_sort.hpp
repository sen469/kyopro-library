#ifndef KYOPRO_ANGULAR_SORT_HPP
#define KYOPRO_ANGULAR_SORT_HPP

#include <algorithm>
#include <cassert>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

enum class angular_origin { first, last, direction };

template <class T>
struct angular_sort_options {
    std::pair<T, T> start = {T(1), T(0)};
    bool include_start = true;
    bool clockwise = false;
    angular_origin origin = angular_origin::first;
    std::pair<T, T> origin_direction = {T(1), T(0)};

    // Keep angular_sort(points, {x, y}) unambiguous in C++17, too.
    angular_sort_options() {}
};

namespace internal {

template <class T>
using angular_calc_t = typename std::conditional<std::is_integral<T>::value, __int128_t, long double>::type;

template <class T>
int sign(const T& x) {
    return (T(0) < x) - (x < T(0));
}

template <class T>
angular_calc_t<T> cross(const std::pair<T, T>& a, const std::pair<T, T>& b) {
    using C = angular_calc_t<T>;
    return C(a.first) * C(b.second) - C(a.second) * C(b.first);
}

template <class T>
int angular_half(const std::pair<T, T>& p, const std::pair<T, T>& start,
                 bool include_start, bool clockwise) {
    auto cr = cross(start, p);
    if (cr != 0) return (clockwise ? cr < 0 : cr > 0) ? 0 : 1;
    // Collinear nonzero vectors: distinguish the start ray from its opposite
    // using signs instead of a dot product to avoid additional large products.
    bool same_direction = start.first != T(0)
                              ? sign(p.first) == sign(start.first)
                              : sign(p.second) == sign(start.second);
    return same_direction ? (include_start ? 0 : 2) : 1;
}

template <class T>
angular_calc_t<T> norm2(const std::pair<T, T>& p) {
    using C = angular_calc_t<T>;
    return C(p.first) * C(p.first) + C(p.second) * C(p.second);
}

}  // namespace internal

template <class T>
struct polar_angle_less {
   private:
    angular_sort_options<T> options_;

    void validate() const {
        assert(options_.start.first != T(0) || options_.start.second != T(0));
        if (options_.origin == angular_origin::direction) {
            assert(options_.origin_direction.first != T(0) ||
                   options_.origin_direction.second != T(0));
        }
    }

   public:
    polar_angle_less() = default;

    explicit polar_angle_less(std::pair<T, T> start) {
        options_.start = start;
        validate();
    }

    explicit polar_angle_less(const angular_sort_options<T>& options) : options_(options) {
        validate();
    }

    bool operator()(const std::pair<T, T>& a, const std::pair<T, T>& b) const {
        bool a_zero = a.first == T(0) && a.second == T(0);
        bool b_zero = b.first == T(0) && b.second == T(0);
        if (options_.origin != angular_origin::direction && (a_zero || b_zero)) {
            if (a_zero == b_zero) return false;
            return options_.origin == angular_origin::first ? a_zero : b_zero;
        }

        const auto& da = a_zero ? options_.origin_direction : a;
        const auto& db = b_zero ? options_.origin_direction : b;
        int ha = internal::angular_half(da, options_.start, options_.include_start,
                                        options_.clockwise);
        int hb = internal::angular_half(db, options_.start, options_.include_start,
                                        options_.clockwise);
        if (ha != hb) return ha < hb;

        auto cr = internal::cross(da, db);
        if (internal::sign(cr) != 0) return options_.clockwise ? cr < 0 : cr > 0;

        return internal::norm2(a) < internal::norm2(b);
    }
};

template <class T>
void angular_sort(std::vector<std::pair<T, T>>& points,
                  const angular_sort_options<T>& options) {
    std::sort(points.begin(), points.end(), polar_angle_less<T>(options));
}

template <class T>
void angular_sort(std::vector<std::pair<T, T>>& points,
                  std::pair<T, T> start = {T(1), T(0)}) {
    std::sort(points.begin(), points.end(), polar_angle_less<T>(start));
}

template <class T>
std::vector<int> angular_sorted_indices(const std::vector<std::pair<T, T>>& points,
                                      const angular_sort_options<T>& options) {
    std::vector<int> order(points.size());
    std::iota(order.begin(), order.end(), 0);
    polar_angle_less<T> less(options);
    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return less(points[i], points[j]);
    });
    return order;
}

template <class T>
std::vector<int> angular_sorted_indices(const std::vector<std::pair<T, T>>& points,
                                      std::pair<T, T> start = {T(1), T(0)}) {
    angular_sort_options<T> options;
    options.start = start;
    return angular_sorted_indices(points, options);
}

}  // namespace kyopro

#endif  // KYOPRO_ANGULAR_SORT_HPP
