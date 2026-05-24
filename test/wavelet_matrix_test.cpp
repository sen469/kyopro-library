#include <bits/stdc++.h>

#include "kyopro/wavelet_matrix.hpp"

using namespace std;

template <class T>
T kth_smallest_naive(vector<T> a, int l, int r, int k) {
    vector<T> b(a.begin() + l, a.begin() + r);
    sort(b.begin(), b.end());
    return b[k];
}

template <class T>
int range_freq_naive(const vector<T>& a, int l, int r, T lower, T upper) {
    int res = 0;
    for (int i = l; i < r; i++) {
        if (lower <= a[i] && a[i] < upper) res++;
    }
    return res;
}

int main() {
    {
        vector<long long> a = {5, 1, 4, 1, 3};
        kyopro::wavelet_matrix<long long> wm(a);

        assert(wm.size() == 5);
        assert(wm.distinct_size() == 4);
        assert(wm.sorted_values() == vector<long long>({1, 3, 4, 5}));
        for (int i = 0; i < (int)a.size(); i++) {
            assert(wm.access(i) == a[i]);
            assert(wm[i] == a[i]);
        }
        assert(wm.kth_smallest(1, 5, 0) == 1);
        assert(wm.kth_smallest(1, 5, 2) == 3);
        assert(wm.kth_largest(1, 5, 0) == 4);
        assert(wm.range_freq(1, 5, 4LL) == 3);
        assert(wm.range_freq(1, 5, 2LL, 5LL) == 2);
        assert(wm.count(0, 5, 1LL) == 2);
        assert(wm.count(0, 5, 2LL) == 0);
        assert(wm.prev_value(0, 5, 4LL) == 3);
        assert(wm.next_value(0, 5, 2LL) == 3);
    }

    {
        vector<int> a = {-10, 7, -10, 3, 7, 100};
        kyopro::wavelet_matrix<int> wm(a);

        for (int l = 0; l < (int)a.size(); l++) {
            for (int r = l + 1; r <= (int)a.size(); r++) {
                for (int k = 0; k < r - l; k++) {
                    assert(wm.kth_smallest(l, r, k) == kth_smallest_naive(a, l, r, k));
                    assert(wm.kth_largest(l, r, k) == kth_smallest_naive(a, l, r, r - l - 1 - k));
                }
                for (int lower = -12; lower <= 102; lower++) {
                    for (int upper = lower; upper <= 102; upper++) {
                        assert(wm.range_freq(l, r, lower, upper) == range_freq_naive(a, l, r, lower, upper));
                    }
                }
            }
        }
    }

    {
        vector<int> a = {42, 42, 42};
        kyopro::wavelet_matrix<int> wm(a);

        assert(wm.kth_smallest(0, 3, 2) == 42);
        assert(wm.kth_largest(0, 3, 1) == 42);
        assert(wm.range_freq(0, 3, 42) == 0);
        assert(wm.range_freq(0, 3, 43) == 3);
        assert(wm.count(0, 3, 42) == 3);
        assert(wm.prev_value(0, 3, 43) == 42);
        assert(wm.next_value(0, 3, 42) == 42);
    }

    {
        vector<int> a;
        kyopro::wavelet_matrix<int> wm(a);
        assert(wm.empty());
        assert(wm.size() == 0);
        assert(wm.distinct_size() == 0);
    }

    return 0;
}
