#include <bits/stdc++.h>

#include "kyopro/ternary_search.hpp"

using namespace std;

int main() {
    assert(kyopro::ternary_search(-100, 101, [](int x) {
               return (x - 7) * (x - 7);
           }) == 7);

    assert(kyopro::ternary_search(0, 20, [](int x) {
               return abs(x - 5) + abs(x - 8);
           }) == 5);

    assert(kyopro::ternary_search(
               0, 20, [](int x) { return -(x - 12) * (x - 12); },
               greater<>()) == 12);

    assert(kyopro::ternary_search(3, 4, [](int x) { return x; }) == 3);

    {
        long long first = numeric_limits<long long>::min();
        long long last = numeric_limits<long long>::max();
        assert(kyopro::ternary_search(first, last, [](long long x) {
                   return x >= 42 ? static_cast<unsigned long long>(x) - 42
                                  : 42ULL - static_cast<unsigned long long>(x);
               }) == 42);
    }

    for (int first = -10; first < 10; first++) {
        for (int last = first + 1; last <= 11; last++) {
            for (int left = first; left < last; left++) {
                for (int right = left; right < last; right++) {
                    auto convex = [&](int x) {
                        if (x < left) return left - x;
                        if (right < x) return x - right;
                        return 0;
                    };
                    assert(kyopro::ternary_search(first, last, convex) == left);
                    assert(kyopro::ternary_search(
                               first, last,
                               [&](int x) { return -convex(x); }, greater<>()) ==
                           left);
                }
            }
        }
    }

    return 0;
}
