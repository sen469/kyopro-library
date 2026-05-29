#include <bits/stdc++.h>

#include "kyopro/angular_sort.hpp"

using namespace std;

int main() {
    {
        vector<pair<int, int>> points = {
            {0, 1}, {-1, 0}, {0, -1}, {1, 0},
            {1, 1}, {-1, 1}, {-1, -1}, {1, -1},
        };

        kyopro::angular_sort(points);

        vector<pair<int, int>> expected = {
            {1, 0}, {1, 1}, {0, 1}, {-1, 1},
            {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
        };
        assert(points == expected);
    }

    {
        vector<pair<long long, long long>> points = {
            {2, 2}, {1, 1}, {0, 0}, {-2, 0}, {3, 0},
        };

        kyopro::angular_sort(points);

        vector<pair<long long, long long>> expected = {
            {0, 0}, {3, 0}, {1, 1}, {2, 2}, {-2, 0},
        };
        assert(points == expected);
    }

    {
        vector<pair<int, int>> points = {
            {0, -1}, {1, 0}, {-1, 0}, {0, 1},
        };

        auto order = kyopro::angular_sorted_indices(points);
        assert(order == vector<int>({1, 3, 2, 0}));
    }

    {
        vector<pair<long long, long long>> points = {
            {1000000000LL, 1000000000LL},
            {1000000000LL, 999999999LL},
            {-1000000000LL, 1LL},
        };

        kyopro::angular_sort(points);

        vector<pair<long long, long long>> expected = {
            {1000000000LL, 999999999LL},
            {1000000000LL, 1000000000LL},
            {-1000000000LL, 1LL},
        };
        assert(points == expected);
    }

    return 0;
}
