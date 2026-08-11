#include <bits/stdc++.h>

#include "kyopro/binary_search.hpp"

using namespace std;

int main() {
    assert(kyopro::binary_search(0, 10, [](int x) { return x >= 6; }) == 6);
    assert(kyopro::binary_search(0, 10, [](int) { return true; }) == 0);
    assert(kyopro::binary_search(0, 10, [](int) { return false; }) == 10);
    assert(kyopro::binary_search(4, 4, [](int) { return true; }) == 4);
    assert(kyopro::binary_search(-10, 5, [](int x) { return x >= -3; }) == -3);

    {
        int first = numeric_limits<int>::min();
        int last = numeric_limits<int>::max();
        assert(kyopro::binary_search(first, last,
                                     [](int x) { return x >= 123456789; }) ==
               123456789);
    }

    {
        unsigned int first = 0;
        unsigned int last = numeric_limits<unsigned int>::max();
        assert(kyopro::binary_search(first, last, [](unsigned int x) {
                   return x >= 3000000000U;
               }) == 3000000000U);
    }

    for (int first = -20; first <= 20; first++) {
        for (int last = first; last <= 20; last++) {
            for (int boundary = first; boundary <= last; boundary++) {
                assert(kyopro::binary_search(first, last, [&](int x) {
                           return x >= boundary;
                       }) == boundary);
            }
        }
    }

    return 0;
}
