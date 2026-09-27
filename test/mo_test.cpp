#include <bits/stdc++.h>

#include "kyopro/mo.hpp"

using namespace std;

int main() {
    {
        vector<int> a = {3, 1, 4, 1, 5, 9, 2};
        vector<pair<int, int>> ranges = {{0, 3}, {2, 7}, {1, 1}, {0, 7}, {4, 6}};
        kyopro::mo mo((int)a.size());
        for (auto [l, r] : ranges) mo.add_query(l, r);

        long long sum = 0;
        vector<long long> answer(ranges.size());
        mo.run([&](int i) { sum += a[i]; }, [&](int i) { sum -= a[i]; },
               [&](int id) { answer[id] = sum; });

        for (int id = 0; id < (int)ranges.size(); id++) {
            auto [l, r] = ranges[id];
            assert(answer[id] == accumulate(a.begin() + l, a.begin() + r, 0LL));
        }
        assert(mo.element_count() == (int)a.size());
        assert(mo.query_count() == (int)ranges.size());
    }

    {
        string s = "algorithm";
        vector<pair<int, int>> ranges = {{2, 7}, {0, 9}, {4, 4}, {1, 5}, {8, 9}};
        kyopro::mo mo((int)s.size());
        for (auto [l, r] : ranges) mo.add_query(l, r);

        deque<char> current;
        vector<string> answer(ranges.size());
        mo.run([&](int i) { current.push_front(s[i]); },
               [&](int i) { current.push_back(s[i]); },
               [&](int i) {
                   assert(current.front() == s[i]);
                   current.pop_front();
               },
               [&](int i) {
                   assert(current.back() == s[i]);
                   current.pop_back();
               },
               [&](int id) { answer[id] = string(current.begin(), current.end()); });

        for (int id = 0; id < (int)ranges.size(); id++) {
            auto [l, r] = ranges[id];
            assert(answer[id] == s.substr(l, r - l));
        }
    }

    {
        mt19937 rng(469);
        for (int iteration = 0; iteration < 200; iteration++) {
            int n = (int)(rng() % 50);
            int q = (int)(rng() % 100);
            vector<int> a(n);
            for (int& x : a) x = (int)(rng() % 15);

            kyopro::mo mo(n);
            vector<pair<int, int>> ranges;
            for (int id = 0; id < q; id++) {
                int l = (int)(rng() % (n + 1));
                int r = (int)(rng() % (n + 1));
                if (l > r) swap(l, r);
                assert(mo.add_query(l, r) == id);
                ranges.push_back({l, r});
            }

            vector<int> frequency(15);
            int distinct = 0;
            vector<int> answer(q);
            auto add = [&](int i) {
                if (frequency[a[i]]++ == 0) distinct++;
            };
            auto erase = [&](int i) {
                if (--frequency[a[i]] == 0) distinct--;
            };
            mo.run(add, erase, [&](int id) { answer[id] = distinct; });

            for (int id = 0; id < q; id++) {
                auto [l, r] = ranges[id];
                set<int> values(a.begin() + l, a.begin() + r);
                assert(answer[id] == (int)values.size());
            }
        }
    }

    {
        kyopro::mo mo;
        int call_count = 0;
        mo.run([&](int) { call_count++; }, [&](int) { call_count++; },
               [&](int) { call_count++; });
        assert(call_count == 0);
        assert(mo.add_query(0, 0) == 0);
        mo.run([&](int) { call_count++; }, [&](int) { call_count++; },
               [&](int id) {
                   assert(id == 0);
                   call_count++;
               });
        assert(call_count == 1);
    }

    return 0;
}
