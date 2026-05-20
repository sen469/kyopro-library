#include <bits/stdc++.h>

#include "kyopro/run_length_encoding.hpp"

using namespace std;

int main() {
    {
        string s = "aaabbcaa";
        vector<pair<char, int>> expected = {{'a', 3}, {'b', 2}, {'c', 1}, {'a', 2}};
        assert(kyopro::run_length_encoding(s) == expected);
    }

    {
        string s = "";
        vector<pair<char, int>> expected;
        assert(kyopro::run_length_encoding(s) == expected);
    }

    {
        vector<int> v = {1, 1, 2, 2, 2, 1, 3, 3};
        vector<pair<int, int>> expected = {{1, 2}, {2, 3}, {1, 1}, {3, 2}};
        assert(kyopro::run_length_encoding(v) == expected);
        vector<pair<int, int>> expected_sub = {{2, 3}, {1, 1}};
        assert(kyopro::run_length_encoding(v.begin() + 2, v.begin() + 6) == expected_sub);
    }

    {
        vector<long long> v = {5};
        vector<pair<long long, int>> expected = {{5, 1}};
        assert(kyopro::run_length_encoding(v) == expected);
    }

    return 0;
}
