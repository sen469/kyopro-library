#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include "kyopro/dynamic_range_kth.hpp"

template <class T>
void check(const kyopro::dynamic_range_kth<T>& ds, const std::vector<T>& a,
           int l, int r, T x, T y) {
    std::vector<T> sorted(a.begin() + l, a.begin() + r);
    std::sort(sorted.begin(), sorted.end());
    for (int k = 0; k < r - l; k++) {
        assert(ds.kth_smallest(l, r, k) == sorted[k]);
        assert(ds.kth_largest(l, r, k) == sorted[r - l - 1 - k]);
    }
    assert(ds.count(l, r, x) == std::count(sorted.begin(), sorted.end(), x));
    assert(ds.range_freq(l, r, x) ==
           std::lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin());
    assert(ds.range_freq(l, r, x, y) ==
           std::count_if(sorted.begin(), sorted.end(), [&](T v) {
               return x <= v && v < y;
           }));
}

template <class T>
void exercise() {
    std::vector<T> a = {std::numeric_limits<T>::min(), T(0),
                        std::numeric_limits<T>::max(), T(3), T(3)};
    kyopro::dynamic_range_kth<T> ds(a);
    for (int l = 0; l <= ds.size(); l++) {
        for (int r = l; r <= ds.size(); r++) {
            check(ds, a, l, r, std::numeric_limits<T>::min(),
                  std::numeric_limits<T>::max());
            check(ds, a, l, r, T(3), T(0));
            check(ds, a, l, r, std::numeric_limits<T>::max(),
                  std::numeric_limits<T>::max());
        }
    }
    // Repeated replacement exercises deletion of paths and node reuse.
    for (int i = 0; i < 50; i++) {
        a[0] = i % 2 ? std::numeric_limits<T>::max()
                     : std::numeric_limits<T>::min();
        ds.set(0, a[0]);
        assert(ds.get(0) == a[0]);
        assert(ds[0] == a[0]);
        check(ds, a, 0, ds.size(), T(3), std::numeric_limits<T>::max());
    }
    auto copied = ds;
    copied.set(0, T(1));
    assert(ds[0] == a[0]);
    ds.build({T(2), T(2), T(2)});
    assert(ds.kth_smallest(0, 3, 2) == T(2));
    ds.build({});
    assert(ds.empty());
    assert(ds.size() == 0);
    assert(ds.count(0, 0, T(0)) == 0);
    assert(ds.range_freq(0, 0, T(0)) == 0);
}

int main() {
    exercise<int>();
    exercise<long long>();
    exercise<unsigned long long>();
    exercise<std::int8_t>();
    exercise<std::uint8_t>();

    kyopro::dynamic_range_kth<> empty;
    assert(empty.empty());
    assert(empty.range_freq(0, 0, 10) == 0);

    std::mt19937 rng(469);
    for (int n : {1, 2, 7, 16, 31}) {
        std::vector<long long> a(n);
        kyopro::dynamic_range_kth<> ds(a);
        for (int step = 0; step < 600; step++) {
            int i = rng() % n;
            long long x = static_cast<int>(rng() % 101) - 50;
            if (step % 11 == 0) x = std::numeric_limits<long long>::min();
            if (step % 13 == 0) x = std::numeric_limits<long long>::max();
            a[i] = x;
            ds.set(i, x);
            ds.set(i, x);  // Updating to the same value is a no-op.
            int l = rng() % (n + 1);
            int r = rng() % (n + 1);
            if (r < l) std::swap(l, r);
            long long lower = static_cast<int>(rng() % 101) - 50;
            long long upper = static_cast<int>(rng() % 101) - 50;
            check(ds, a, l, r, lower, upper);
        }
    }
}
