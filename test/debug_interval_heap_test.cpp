// Include the core debug header first to check lookup in its existing templates.
#include "debug/debug.hpp"
#include "debug/interval_heap.hpp"

#include <cassert>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

template <class T>
std::string render(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

struct direction {
    bool descending;
    bool operator()(int a, int b) const { return descending ? a > b : a < b; }
};

int main() {
    assert(render(kyopro::interval_heap<int>{}) == "{}");
    assert(render(kyopro::interval_heap<int>{7}) == "{7}");
    const kyopro::interval_heap<int> heap = {5, 1, 3, 1};
    assert(render(heap) == "{1, 1, 3, 5}");
    assert(heap.size() == 4);
    assert(heap.min() == 1);
    assert(heap.max() == 5);
    assert(render(heap) == "{1, 1, 3, 5}");

    kyopro::interval_heap<int, std::greater<int>> descending = {5, 1, 3, 1};
    assert(render(descending) == "{5, 3, 1, 1}");
    kyopro::interval_heap<int, direction> stateful({5, 1, 3, 1}, direction{true});
    assert(render(stateful) == "{5, 3, 1, 1}");

    __int128 large = __int128(1) << 100;
    __int128 maximum = ((__int128(1) << 126) - 1) + (__int128(1) << 126);
    __int128 minimum = -maximum - 1;
    kyopro::interval_heap<__int128> wide = {large, 0, minimum};
    assert(render(wide) == "{-170141183460469231731687303715884105728, 0, "
                           "1267650600228229401496703205376}");

    std::vector<kyopro::interval_heap<int>> nested = {heap, {}};
    assert(render(nested) == "{{1, 1, 3, 5}, {}}");
    assert(render(std::make_pair(heap, 2)) == "({1, 1, 3, 5}, 2)");
    assert(render(std::make_tuple(heap, descending)) ==
           "({1, 1, 3, 5}, {5, 3, 1, 1})");
    kyopro::interval_heap<std::vector<int>> vectors = {{2, 1}, {1, 3}, {}};
    assert(render(vectors) == "{{}, {1, 3}, {2, 1}}");
    kyopro::interval_heap<std::pair<int, int>> pairs = {{2, 1}, {1, 3}};
    assert(render(pairs) == "{(1, 3), (2, 1)}");

    std::ostringstream out;
    auto* previous = std::cerr.rdbuf(out.rdbuf());
    dg(heap, descending, wide, nested, vectors);
    std::cerr.rdbuf(previous);
    assert(out.str().find("{1, 1, 3, 5}") != std::string::npos);
    assert(out.str().find("{5, 3, 1, 1}") != std::string::npos);
    assert(out.str().find("-170141183460469231731687303715884105728") !=
           std::string::npos);
    assert(out.str().find("{{1, 1, 3, 5}, {}}") != std::string::npos);
    assert(out.str().find("{{}, {1, 3}, {2, 1}}") != std::string::npos);

    // Printing neither consumes elements nor changes comparator state.
    assert(render(heap) == "{1, 1, 3, 5}");
    descending.pop_min();
    descending.push(9);
    assert(render(descending) == "{9, 3, 1, 1}");
    assert(stateful.min() == 5);
    assert(stateful.max() == 1);
}
