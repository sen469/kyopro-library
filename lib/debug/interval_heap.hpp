#ifndef KYOPRO_DEBUG_INTERVAL_HEAP_HPP
#define KYOPRO_DEBUG_INTERVAL_HEAP_HPP

#include "debug/debug.hpp"
#include "kyopro/interval_heap.hpp"

namespace kyopro {

// ADL also finds this printer for heaps nested inside standard containers.
template <class T, class Compare>
std::ostream& operator<<(std::ostream& os, interval_heap<T, Compare> heap) {
    // Keep the global debug printers visible for vector, pair, __int128, etc.
    using ::operator<<;
    os << "{";
    bool first = true;
    while (!heap.empty()) {
        if (!first) os << ", ";
        os << heap.min();
        heap.pop_min();
        first = false;
    }
    return os << "}";
}

}  // namespace kyopro

#endif  // KYOPRO_DEBUG_INTERVAL_HEAP_HPP
