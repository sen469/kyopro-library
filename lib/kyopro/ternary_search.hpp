#ifndef KYOPRO_TERNARY_SEARCH_HPP
#define KYOPRO_TERNARY_SEARCH_HPP

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

#include "kyopro/binary_search.hpp"

namespace kyopro {

// Returns the leftmost optimum in the nonempty integer interval [first, last).
// By default the minimum is searched for. Pass std::greater<> to find a maximum.
template <class Int, class Function, class Compare = std::less<>>
Int ternary_search(Int first, Int last, Function function,
                   Compare compare = Compare()) {
    static_assert(std::is_integral<Int>::value,
                  "ternary_search requires an integral type");
    static_assert(!std::is_same<typename std::remove_cv<Int>::type, bool>::value,
                  "ternary_search does not support bool");
    assert(first < last);

    while (internal::search_distance(first, last) > 3) {
        auto distance = internal::search_distance(first, last);
        auto third = distance / 3;
        Int middle1 = internal::search_advance(first, third);
        Int middle2 = internal::search_advance(first, distance - third);

        auto value1 = function(middle1);
        auto value2 = function(middle2);
        if (compare(value2, value1)) {
            first = internal::search_advance(middle1, 1);
        } else {
            last = middle2;
        }
    }

    Int best = first;
    auto best_value = function(best);
    for (Int x = internal::search_advance(first, 1); x < last;
         x = internal::search_advance(x, 1)) {
        auto value = function(x);
        if (compare(value, best_value)) {
            best = x;
            best_value = std::move(value);
        }
    }
    return best;
}

}  // namespace kyopro

#endif  // KYOPRO_TERNARY_SEARCH_HPP
