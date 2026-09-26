#include <bits/stdc++.h>

#include "kyopro/kmp.hpp"

using namespace std;

template <class Text, class Pattern>
vector<int> naive_search(const Text& text, const Pattern& pattern) {
    vector<int> result;
    int n = (int)text.size();
    int m = (int)pattern.size();
    for (int i = 0; i + m <= n; i++) {
        bool match = true;
        for (int j = 0; j < m; j++) {
            if (!(text[i + j] == pattern[j])) match = false;
        }
        if (match) result.push_back(i);
    }
    return result;
}

int main() {
    {
        string pattern = "ababcabab";
        vector<int> expected = {0, 0, 1, 2, 0, 1, 2, 3, 4};
        assert(kyopro::prefix_function(pattern) == expected);
    }

    {
        string text = "abracadabra";
        string pattern = "abra";
        vector<int> expected = {0, 7};
        assert(kyopro::kmp_search(text, pattern) == expected);
        assert(kyopro::kmp_find(text, pattern) == 0);
    }

    {
        string text = "aaaaa";
        string pattern = "aaa";
        vector<int> expected = {0, 1, 2};
        assert(kyopro::kmp_search(text, pattern) == expected);
        assert(kyopro::kmp_find(text, pattern) == 0);
    }

    {
        string text = "abcdef";
        assert(kyopro::kmp_search(text, string("xyz")).empty());
        assert(kyopro::kmp_find(text, string("xyz")) == -1);
        assert(kyopro::kmp_search(text, string("abcdefg")).empty());
    }

    {
        string text = "abc";
        vector<int> expected = {0, 1, 2, 3};
        assert(kyopro::kmp_search(text, string()) == expected);
        assert(kyopro::kmp_find(text, string()) == 0);
        assert(kyopro::kmp_search(string(), string()) == vector<int>({0}));
    }

    {
        vector<int> text = {1, 2, 1, 2, 1, 3, 1, 2, 1};
        vector<int> pattern = {1, 2, 1};
        vector<int> expected = {0, 2, 6};
        assert(kyopro::kmp_search(text, pattern) == expected);
        assert(kyopro::kmp_find(text, pattern) == 0);
    }

    {
        mt19937 rng(469);
        for (int iteration = 0; iteration < 5000; iteration++) {
            int n = (int)(rng() % 30);
            int m = (int)(rng() % 12);
            string text(n, 'a');
            string pattern(m, 'a');
            for (char& c : text) c += rng() % 4;
            for (char& c : pattern) c += rng() % 4;

            vector<int> expected = naive_search(text, pattern);
            assert(kyopro::kmp_search(text, pattern) == expected);
            int first = expected.empty() ? -1 : expected[0];
            assert(kyopro::kmp_find(text, pattern) == first);
        }
    }

    return 0;
}
