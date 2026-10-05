#ifndef KYOPRO_CONVEX_HULL_TRICK_HPP
#define KYOPRO_CONVEX_HULL_TRICK_HPP

#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

namespace kyopro {

template <class T = long long, bool is_min = true>
class convex_hull_trick {
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value && sizeof(T) <= 8,
                  "T must be a signed integer type of at most 64 bits");

    using wide = __int128;
    using unsigned_wide = unsigned __int128;

    struct line {
        wide a, b;
        wide eval(T x) const { return a * static_cast<wide>(x) + b; }
    };

    std::vector<line> hull_;
    wide last_slope_ = 0;

    static bool redundant(const line& a, const line& b, const line& c) {
        const wide left = b.b - a.b;
        const wide right = c.b - b.b;
        if ((left < 0) != (right < 0)) return left >= 0;

        // Compare consecutive intersections. Unsigned products cover full 64-bit inputs.
        const unsigned_wide lhs = static_cast<unsigned_wide>(left < 0 ? -left : left)
                                  * static_cast<unsigned_wide>(b.a - c.a);
        const unsigned_wide rhs = static_cast<unsigned_wide>(right < 0 ? -right : right)
                                  * static_cast<unsigned_wide>(a.a - b.a);
        return left < 0 ? lhs <= rhs : lhs >= rhs;
    }

public:
    convex_hull_trick() = default;

    bool empty() const { return hull_.empty(); }

    int size() const { return static_cast<int>(hull_.size()); }

    void clear() {
        hull_.clear();
        last_slope_ = 0;
    }

    void add_line(T a, T b) {
        line next{static_cast<wide>(a), static_cast<wide>(b)};
        if (!is_min) {
            next.a = -next.a;
            next.b = -next.b;
        }
        assert(empty() || last_slope_ >= next.a);
        last_slope_ = next.a;

        if (!empty() && hull_.back().a == next.a) {
            if (hull_.back().b <= next.b) return;
            hull_.pop_back();
        }
        while (hull_.size() >= 2 &&
               redundant(hull_[hull_.size() - 2], hull_.back(), next)) {
            hull_.pop_back();
        }
        hull_.push_back(next);
    }

    wide query_wide(T x) const {
        assert(!empty());
        int l = 0, r = size() - 1;
        while (l < r) {
            const int m = l + (r - l) / 2;
            if (hull_[m].eval(x) >= hull_[m + 1].eval(x)) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        const wide result = hull_[l].eval(x);
        return is_min ? result : -result;
    }

    T query(T x) const {
        const wide result = query_wide(x);
        assert(static_cast<wide>(std::numeric_limits<T>::lowest()) <= result &&
               result <= static_cast<wide>(std::numeric_limits<T>::max()));
        return static_cast<T>(result);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_CONVEX_HULL_TRICK_HPP
