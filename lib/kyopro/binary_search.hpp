#ifndef KYOPRO_BINARY_SEARCH_HPP
#define KYOPRO_BINARY_SEARCH_HPP

#include <cassert>
#include <limits>
#include <type_traits>

namespace kyopro {

namespace internal {

template <class Int>
using search_unsigned_t = typename std::make_unsigned<Int>::type;

template <class Int>
search_unsigned_t<Int> search_distance(Int first, Int last) {
    using U = search_unsigned_t<Int>;
    return static_cast<U>(last) - static_cast<U>(first);
}

template <class Int>
Int search_advance(Int first, search_unsigned_t<Int> offset) {
    using U = search_unsigned_t<Int>;
    U value = static_cast<U>(first) + offset;

    if constexpr (std::is_signed<Int>::value) {
        U max_value = static_cast<U>(std::numeric_limits<Int>::max());
        if (value > max_value) {
            U min_value = static_cast<U>(std::numeric_limits<Int>::min());
            return std::numeric_limits<Int>::min() +
                   static_cast<Int>(value - min_value);
        }
    }
    return static_cast<Int>(value);
}

}  // namespace internal

// Returns the first x in [first, last) for which pred(x) is true.
// Returns last if no such x exists.
template <class Int, class Predicate>
Int binary_search(Int first, Int last, Predicate pred) {
    static_assert(std::is_integral<Int>::value,
                  "binary_search requires an integral type");
    static_assert(!std::is_same<typename std::remove_cv<Int>::type, bool>::value,
                  "binary_search does not support bool");
    assert(first <= last);

    while (first < last) {
        auto distance = internal::search_distance(first, last);
        Int middle = internal::search_advance(first, distance / 2);
        if (pred(middle)) {
            last = middle;
        } else {
            first = internal::search_advance(middle, 1);
        }
    }
    return first;
}

}  // namespace kyopro

#endif  // KYOPRO_BINARY_SEARCH_HPP
