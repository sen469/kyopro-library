#ifndef KYOPRO_ANGULAR_SORT_HPP
#define KYOPRO_ANGULAR_SORT_HPP

#include <algorithm>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

namespace internal {

template <class T>
using angular_calc_t = typename std::conditional<std::is_integral<T>::value, __int128_t, long double>::type;

template <class T>
int sign(const T& x) {
    return (T(0) < x) - (x < T(0));
}

template <class T>
int angular_half(const std::pair<T, T>& p) {
    if (p.first == T(0) && p.second == T(0)) return -1;
    return (p.second > T(0) || (p.second == T(0) && p.first > T(0))) ? 0 : 1;
}

template <class T>
angular_calc_t<T> cross(const std::pair<T, T>& a, const std::pair<T, T>& b) {
    using C = angular_calc_t<T>;
    return C(a.first) * C(b.second) - C(a.second) * C(b.first);
}

template <class T>
angular_calc_t<T> norm2(const std::pair<T, T>& p) {
    using C = angular_calc_t<T>;
    return C(p.first) * C(p.first) + C(p.second) * C(p.second);
}

}  // namespace internal

template <class T>
struct polar_angle_less {
    bool operator()(const std::pair<T, T>& a, const std::pair<T, T>& b) const {
        int ha = internal::angular_half(a);
        int hb = internal::angular_half(b);
        if (ha != hb) return ha < hb;

        auto cr = internal::cross(a, b);
        if (internal::sign(cr) != 0) return cr > 0;

        return internal::norm2(a) < internal::norm2(b);
    }
};

template <class T>
void angular_sort(std::vector<std::pair<T, T>>& points) {
    std::sort(points.begin(), points.end(), polar_angle_less<T>());
}

template <class T>
std::vector<int> angular_sorted_indices(const std::vector<std::pair<T, T>>& points) {
    std::vector<int> order(points.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return polar_angle_less<T>()(points[i], points[j]);
    });
    return order;
}

}  // namespace kyopro

#endif  // KYOPRO_ANGULAR_SORT_HPP
