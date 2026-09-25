#include <bits/stdc++.h>

#include "kyopro/binary_trie.hpp"

using namespace std;

int main() {
    {
        kyopro::binary_trie<unsigned, 4> trie;
        assert(trie.empty());
        assert(trie.size() == 0);

        for (unsigned x : {5, 0, 15, 5, 2, 7}) trie.insert(x);
        assert(trie.size() == 6);
        assert(trie.count(5) == 2);
        assert(trie.contains(15));
        assert(!trie.contains(1));

        vector<unsigned> sorted = {0, 2, 5, 5, 7, 15};
        for (int k = 0; k < (int)sorted.size(); k++) {
            assert(trie.kth_element(k) == sorted[k]);
        }
        assert(trie.min_element() == 0);
        assert(trie.max_element() == 15);
        assert(trie.count_less(0) == 0);
        assert(trie.count_less(5) == 2);
        assert(trie.count_less(6) == 4);

        assert(trie.min_xor_element(6) == 7);
        assert(trie.min_xor(6) == 1);
        assert(trie.max_xor_element(6) == 15);
        assert(trie.max_xor(6) == 9);

        assert(trie.erase(5));
        assert(trie.count(5) == 1);
        assert(trie.erase(5));
        assert(!trie.contains(5));
        assert(!trie.erase(5));
    }

    {
        kyopro::binary_trie<unsigned, 4> trie;
        for (unsigned x : {1, 4, 4, 10}) trie.insert(x);

        trie.apply_xor(3);
        vector<unsigned> sorted = {2, 7, 7, 9};
        for (int k = 0; k < (int)sorted.size(); k++) {
            assert(trie.kth_element(k) == sorted[k]);
        }
        assert(trie.count(7) == 2);
        assert(trie.count_less(8) == 3);

        trie.insert(0);
        assert(trie.contains(0));
        assert(trie.erase(7));
        assert(trie.count(7) == 1);

        trie.apply_xor(3);
        assert(trie.contains(3));
        assert(trie.contains(1));
        assert(trie.contains(4));
        assert(trie.contains(10));
    }

    {
        constexpr unsigned LIMIT = 1U << 8;
        kyopro::binary_trie<unsigned, 8> trie;
        multiset<unsigned> values;
        mt19937 rng(469);

        for (int iteration = 0; iteration < 3000; iteration++) {
            int operation = (int)(rng() % 4);
            unsigned x = rng() % LIMIT;
            if (operation == 0 || values.empty()) {
                trie.insert(x);
                values.insert(x);
            } else if (operation == 1) {
                bool expected = values.find(x) != values.end();
                assert(trie.erase(x) == expected);
                if (expected) values.erase(values.find(x));
            } else if (operation == 2) {
                trie.apply_xor(x);
                multiset<unsigned> next_values;
                for (unsigned value : values) next_values.insert(value ^ x);
                values = std::move(next_values);
            } else {
                assert(trie.count(x) == (int)values.count(x));
            }

            assert(trie.size() == (int)values.size());
            if (values.empty()) continue;

            int k = (int)(rng() % values.size());
            assert(trie.kth_element(k) == *next(values.begin(), k));
            assert(trie.min_element() == *values.begin());
            assert(trie.max_element() == *values.rbegin());
            assert(trie.count_less(x) == distance(values.begin(), values.lower_bound(x)));

            auto min_it = min_element(values.begin(), values.end(),
                                      [&](unsigned a, unsigned b) { return (a ^ x) < (b ^ x); });
            auto max_it = max_element(values.begin(), values.end(),
                                      [&](unsigned a, unsigned b) { return (a ^ x) < (b ^ x); });
            assert(trie.min_xor_element(x) == *min_it);
            assert(trie.max_xor_element(x) == *max_it);
            assert(trie.min_xor(x) == (*min_it ^ x));
            assert(trie.max_xor(x) == (*max_it ^ x));
        }
    }

    {
        kyopro::binary_trie<uint64_t> trie;
        uint64_t high = uint64_t(1) << 63;
        trie.insert(0);
        trie.insert(high);
        assert(trie.max_element() == high);
        assert(trie.max_xor(0) == high);
    }

    return 0;
}
