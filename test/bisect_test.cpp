#include <bits/stdc++.h>

#include "kyopro/bisect.hpp"

using namespace std;
using namespace kyopro;

int main() {
    // Both namespaces may be imported without making either call ambiguous.
    vector<int> values = {1, 3, 5};
    assert(binary_search(values.begin(), values.end(), 3));
    assert(!binary_search(values.begin(), values.end(), 4));
    assert(bisect(0, 10, [](int x) { return x >= 6; }) == 6);
    assert(kyopro::bisect(0, 10, [](int x) { return x >= 6; }) == 6);
    assert(kyopro::bisect(0, 10, [](int) { return true; }) == 0);
    assert(kyopro::bisect(0, 10, [](int) { return false; }) == 10);
    assert(kyopro::bisect(4, 4, [](int) { return true; }) == 4);
    assert(kyopro::bisect(-10, 5, [](int x) { return x >= -3; }) == -3);

    {
        int first = numeric_limits<int>::min();
        int last = numeric_limits<int>::max();
        assert(kyopro::bisect(first, last,
                                     [](int x) { return x >= 123456789; }) ==
               123456789);
    }

    {
        unsigned int first = 0;
        unsigned int last = numeric_limits<unsigned int>::max();
        assert(kyopro::bisect(first, last, [](unsigned int x) {
                   return x >= 3000000000U;
               }) == 3000000000U);
    }

    for (int first = -20; first <= 20; first++) {
        for (int last = first; last <= 20; last++) {
            for (int boundary = first; boundary <= last; boundary++) {
                assert(kyopro::bisect(first, last, [&](int x) {
                           return x >= boundary;
                       }) == boundary);
            }
        }
    }

    return 0;
}
