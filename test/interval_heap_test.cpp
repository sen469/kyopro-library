#include <bits/stdc++.h>

#include "kyopro/interval_heap.hpp"

using namespace std;

template <class T>
void check_heap(const kyopro::interval_heap<T>& heap, const multiset<T>& expected) {
    assert(heap.size() == (int)expected.size());
    assert(heap.empty() == expected.empty());
    if (!expected.empty()) {
        assert(heap.min() == *expected.begin());
        assert(heap.max() == *expected.rbegin());
        assert(heap.top_min() == *expected.begin());
        assert(heap.top_max() == *expected.rbegin());
    }
}

int main() {
    {
        kyopro::interval_heap<int> heap;
        multiset<int> expected;
        mt19937 rng(0);

        for (int step = 0; step < 100000; step++) {
            int kind = uniform_int_distribution<int>(0, 3)(rng);
            int x = uniform_int_distribution<int>(-1000, 1000)(rng);

            if (kind == 0 || expected.empty()) {
                heap.push(x);
                expected.insert(x);
            } else if (kind == 1) {
                heap.pop_min();
                expected.erase(expected.begin());
            } else if (kind == 2) {
                heap.pop_max();
                auto it = prev(expected.end());
                expected.erase(it);
            } else {
                heap.emplace(x);
                expected.insert(x);
            }

            check_heap(heap, expected);
        }
    }

    {
        kyopro::interval_heap<int> heap = {5, 1, 4, 1, 3};
        assert(heap.size() == 5);
        assert(heap.min() == 1);
        assert(heap.max() == 5);

        vector<int> ascending;
        while (!heap.empty()) {
            ascending.push_back(heap.min());
            heap.pop_min();
        }
        assert(ascending == vector<int>({1, 1, 3, 4, 5}));
    }

    {
        kyopro::interval_heap<int> heap(vector<int>{5, 1, 4, 1, 3});
        vector<int> descending;
        while (!heap.empty()) {
            descending.push_back(heap.max());
            heap.pop_max();
        }
        assert(descending == vector<int>({5, 4, 3, 1, 1}));
    }

    {
        kyopro::interval_heap<int, greater<int>> heap;
        heap.push(1);
        heap.push(5);
        heap.push(3);

        assert(heap.min() == 5);
        assert(heap.max() == 1);
        heap.clear();
        assert(heap.empty());
    }

    return 0;
}
