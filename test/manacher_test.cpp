#include <bits/stdc++.h>

#include "kyopro/manacher.hpp"

using namespace std;

int main() {
    {
        string s = "";
        vector<int> expected;
        assert(kyopro::manacher(s) == expected);
        assert(kyopro::manacher_even(s) == expected);
    }

    {
        string s = "abacaba";
        vector<int> odd = {1, 2, 1, 4, 1, 2, 1};
        vector<int> even = {0, 0, 0, 0, 0, 0, 0};
        assert(kyopro::manacher(s) == odd);
        assert(kyopro::manacher_even(s) == even);

        auto radii = kyopro::palindrome_radii(s);
        assert(radii.odd == odd);
        assert(radii.even == even);
        assert(radii.is_palindrome(0, 7));
        assert(radii.is_palindrome(0, 3));
        assert(radii.is_palindrome(2, 5));
        assert(!radii.is_palindrome(0, 4));
    }

    {
        string s = "abba";
        vector<int> odd = {1, 1, 1, 1};
        vector<int> even = {0, 0, 2, 0};
        assert(kyopro::manacher(s) == odd);
        assert(kyopro::manacher_even(s) == even);

        auto radii = kyopro::palindrome_radii(s);
        assert(radii.is_palindrome(0, 4));
        assert(radii.is_palindrome(1, 3));
        assert(radii.is_palindrome(2, 2));
        assert(!radii.is_palindrome(0, 3));
    }

    {
        vector<int> v = {1, 2, 2, 1, 3};
        vector<int> odd = {1, 1, 1, 1, 1};
        vector<int> even = {0, 0, 2, 0, 0};
        assert(kyopro::manacher(v) == odd);
        assert(kyopro::manacher_even(v) == even);
    }

    return 0;
}
