#include <bits/stdc++.h>

#include "kyopro/rolling_hash.hpp"

using namespace std;

int naive_lcp(const string& s, int l1, int r1, int l2, int r2) {
    int len = min(r1 - l1, r2 - l2);
    int res = 0;
    while (res < len && s[l1 + res] == s[l2 + res]) res++;
    return res;
}

int main() {
    {
        string s = "abracadabra";
        kyopro::rolling_hash rh(s);

        assert(rh.size() == (int)s.size());
        assert(!rh.empty());
        assert(rh.same(0, 4, 7, 11));
        assert(!rh.same(0, 3, 3, 6));
        assert(rh.get(0, rh.size()) == rh.all());

        auto left = rh.get(0, 4);
        auto right = rh.get(4, 7);
        assert(rh.concat(left, right, 3) == rh.get(0, 7));

        assert(rh.lcp(0, 7) == 4);
        assert(rh.lcp(0, 11) == 0);
        assert(rh.lcp(0, 11, 7, 11) == 4);
        assert(rh.lcp(3, 7, 5, 11) == naive_lcp(s, 3, 7, 5, 11));
    }

    {
        string s = "aaaaa";
        kyopro::rolling_hash rh(s);
        for (int l1 = 0; l1 <= (int)s.size(); l1++) {
            for (int r1 = l1; r1 <= (int)s.size(); r1++) {
                for (int l2 = 0; l2 <= (int)s.size(); l2++) {
                    for (int r2 = l2; r2 <= (int)s.size(); r2++) {
                        bool expected = s.substr(l1, r1 - l1) == s.substr(l2, r2 - l2);
                        assert(rh.same(l1, r1, l2, r2) == expected);
                        assert(rh.lcp(l1, r1, l2, r2) == naive_lcp(s, l1, r1, l2, r2));
                    }
                }
            }
        }
    }

    {
        vector<int> v = {1, 2, 3, 1, 2, 3, 4};
        kyopro::rolling_hash rh(v);
        assert(rh.same(0, 3, 3, 6));
        assert(!rh.same(0, 4, 3, 7));
        assert(rh.lcp(0, 7, 3, 7) == 3);
    }

    {
        kyopro::rolling_hash rh;
        assert(rh.size() == 0);
        assert(rh.empty());
        assert(rh.all() == 0);
        rh.build(string("abcabc"));
        assert(rh.same(0, 3, 3, 6));
    }

    {
        string s = "mississippi";
        kyopro::rolling_hash rh(s, 911382323);
        for (int a = 0; a <= (int)s.size(); a++) {
            for (int b = 0; b <= (int)s.size(); b++) {
                assert(rh.lcp(a, b) == naive_lcp(s, a, (int)s.size(), b, (int)s.size()));
            }
        }
    }

    return 0;
}
