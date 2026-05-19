#include <bits/stdc++.h>

#include "kyopro/balanced_binary_search_tree.hpp"

using namespace std;

template <class T>
vector<T> multiset_to_vector(const multiset<T>& s) {
    return vector<T>(s.begin(), s.end());
}

int main() {
    kyopro::balanced_binary_search_tree<int> t;
    multiset<int> ms;
    mt19937 rng(0);

    for (int step = 0; step < 50000; step++) {
        int kind = uniform_int_distribution<int>(0, 8)(rng);
        int x = uniform_int_distribution<int>(-100, 100)(rng);

        if (kind == 0 || ms.empty()) {
            t.insert(x);
            ms.insert(x);
        } else if (kind == 1) {
            bool got = t.erase(x);
            auto it = ms.find(x);
            bool expected = it != ms.end();
            assert(got == expected);
            if (it != ms.end()) ms.erase(it);
        } else if (kind == 2) {
            int got = t.erase_all(x);
            int expected = (int)ms.erase(x);
            assert(got == expected);
        } else if (kind == 3) {
            assert(t.count(x) == (int)ms.count(x));
            assert(t.contains(x) == (ms.count(x) > 0));
        } else if (kind == 4) {
            auto got = t.lower_bound(x);
            auto it = ms.lower_bound(x);
            assert((bool)got == (it != ms.end()));
            if (it != ms.end()) assert(*got == *it);
        } else if (kind == 5) {
            auto got = t.upper_bound(x);
            auto it = ms.upper_bound(x);
            assert((bool)got == (it != ms.end()));
            if (it != ms.end()) assert(*got == *it);
        } else if (kind == 6) {
            int k = uniform_int_distribution<int>(0, (int)ms.size() - 1)(rng);
            auto it = ms.begin();
            advance(it, k);
            assert(t.kth(k) == *it);
        } else if (kind == 7) {
            assert(t.order_of_key(x) == distance(ms.begin(), ms.lower_bound(x)));
        } else {
            auto got_min = t.min();
            auto got_max = t.max();
            assert((bool)got_min == !ms.empty());
            assert((bool)got_max == !ms.empty());
            if (!ms.empty()) {
                assert(*got_min == *ms.begin());
                assert(*got_max == *ms.rbegin());
            }
        }

        assert(t.size() == (int)ms.size());
        assert(t.empty() == ms.empty());
        assert(t.to_vector() == multiset_to_vector(ms));
    }

    kyopro::balanced_binary_search_tree<int> copied = t;
    assert(copied.to_vector() == t.to_vector());
    copied.insert(12345);
    assert(copied.size() == t.size() + 1);

    t.clear();
    assert(t.empty());
    assert(!t.lower_bound(0));
    assert(!t.upper_bound(0));
    assert(!t.min());
    assert(!t.max());

    return 0;
}
