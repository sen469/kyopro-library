#include <bits/stdc++.h>

#include "kyopro/interval_set.hpp"

using namespace std;

int main() {
    {
        kyopro::interval_set<int> intervals;
        assert(intervals.empty());

        intervals.add(1, 4);
        intervals.add(6, 9);
        intervals.add(4, 6);
        vector<pair<int, int>> expected = {{1, 9}};
        assert(intervals.intervals() == expected);
        assert(intervals.size() == 1);
        assert(intervals.interval_count() == 1);
        assert(intervals.contains(1));
        assert(intervals.contains(8));
        assert(!intervals.contains(9));
        assert(intervals.contains(2, 8));
        assert(!intervals.contains(0, 2));
        assert(intervals.contains(5, 5));

        intervals.erase(3, 7);
        expected = {{1, 3}, {7, 9}};
        assert(intervals.intervals() == expected);
        assert(intervals.find(2) == make_optional(make_pair(1, 3)));
        assert(!intervals.find(5).has_value());
        assert(intervals.mex(0) == 0);
        assert(intervals.mex(1) == 3);
        assert(intervals.mex(7) == 9);

        intervals.erase(-100, 100);
        assert(intervals.empty());
        intervals.add(5, 5);
        assert(intervals.empty());
    }

    {
        kyopro::interval_set<long long> intervals;
        intervals.add(-10, -5);
        intervals.add(-3, 2);
        intervals.add(-7, 0);
        vector<pair<long long, long long>> expected = {{-10, 2}};
        assert(intervals.intervals() == expected);
        assert(intervals.mex(-8) == 2);
        intervals.erase(-6, -4);
        expected = {{-10, -6}, {-4, 2}};
        assert(intervals.intervals() == expected);
    }

    {
        constexpr int LOW = -30;
        constexpr int HIGH = 31;
        mt19937 rng(469);
        kyopro::interval_set<int> intervals;
        vector<bool> covered(HIGH - LOW);

        for (int iteration = 0; iteration < 5000; iteration++) {
            int left = LOW + (int)(rng() % (HIGH - LOW + 1));
            int right = LOW + (int)(rng() % (HIGH - LOW + 1));
            if (left > right) swap(left, right);

            if (rng() & 1) {
                intervals.add(left, right);
                for (int x = left; x < right; x++) covered[x - LOW] = true;
            } else {
                intervals.erase(left, right);
                for (int x = left; x < right; x++) covered[x - LOW] = false;
            }

            for (int x = LOW; x < HIGH; x++) {
                assert(intervals.contains(x) == covered[x - LOW]);
                int expected_mex = x;
                while (expected_mex < HIGH && covered[expected_mex - LOW]) expected_mex++;
                assert(intervals.mex(x) == expected_mex);
            }

            int query_left = LOW + (int)(rng() % (HIGH - LOW + 1));
            int query_right = LOW + (int)(rng() % (HIGH - LOW + 1));
            if (query_left > query_right) swap(query_left, query_right);
            bool expected_contains = true;
            for (int x = query_left; x < query_right; x++) {
                expected_contains = expected_contains && covered[x - LOW];
            }
            assert(intervals.contains(query_left, query_right) == expected_contains);

            vector<pair<int, int>> expected_intervals;
            for (int x = LOW; x < HIGH;) {
                if (!covered[x - LOW]) {
                    x++;
                    continue;
                }
                int start = x;
                while (x < HIGH && covered[x - LOW]) x++;
                expected_intervals.push_back({start, x});
            }
            assert(intervals.intervals() == expected_intervals);
        }

        intervals.clear();
        assert(intervals.empty());
    }

    return 0;
}
