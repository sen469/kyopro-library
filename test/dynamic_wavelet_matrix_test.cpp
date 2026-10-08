#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include "kyopro/dynamic_wavelet_matrix.hpp"
#include "kyopro/wavelet_matrix.hpp"

void bit_vector_test() {
    std::mt19937 rng(469);
    using bit_vector = kyopro::internal::dynamic_wavelet_bit_vector;
    for (int length : {0, 1, 63, 64, 65, 127, 128, 129, 1024}) {
        std::vector<unsigned char> a(length);
        for (auto& bit : a) bit = rng() & 1;
        bit_vector bv;
        bv.build(a);
        auto verify = [&] {
            assert(bv.size() == static_cast<int>(a.size()));
            int sum = 0;
            for (int i = 0; i < bv.size(); i++) {
                assert(bv.rank1(i) == sum);
                assert(bv.access(i) == a[i]);
                sum += a[i];
            }
            assert(bv.rank1(bv.size()) == sum);
        };
        verify();
        for (int step = 0; step < 3000; step++) {
            if (a.empty() || rng() % 3 != 0) {
                int i = rng() % (a.size() + 1);
                if (step % 7 == 0) i = 0;
                if (step % 11 == 0) i = a.size();
                unsigned char bit = rng() & 1;
                bv.insert(i, bit);
                a.insert(a.begin() + i, bit);
            } else {
                int i = rng() % a.size();
                bv.erase(i);
                a.erase(a.begin() + i);
            }
            if (step % 53 == 0) verify();
        }
        verify();
        while (!a.empty()) {
            int i = rng() % a.size();
            bv.erase(i);
            a.erase(a.begin() + i);
            if (a.size() % 61 == 0) verify();
        }
        verify();
        bv.insert(0, 1);
        assert(bv.access(0) == 1);
        assert(bv.rank1(1) == 1);
    }
}

template <class T>
void check(const kyopro::dynamic_wavelet_matrix<T>& wm,
           const std::vector<T>& a, int l, int r, T x, T y) {
    assert(wm.size() == static_cast<int>(a.size()));
    assert(wm.empty() == a.empty());
    std::vector<T> sorted(a.begin() + l, a.begin() + r);
    std::sort(sorted.begin(), sorted.end());
    for (int k = 0; k < r - l; k++) {
        assert(wm.kth_smallest(l, r, k) == sorted[k]);
        assert(wm.kth_largest(l, r, k) == sorted[r - l - 1 - k]);
    }
    assert(wm.count(l, r, x) == std::count(sorted.begin(), sorted.end(), x));
    int less = std::lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin();
    assert(wm.range_freq(l, r, x) == less);
    assert(wm.range_freq(l, r, x, y) ==
           std::count_if(sorted.begin(), sorted.end(), [&](T v) {
               return x <= v && v < y;
           }));
    if (less > 0) assert(wm.prev_value(l, r, x) == sorted[less - 1]);
    if (less < r - l) assert(wm.next_value(l, r, x) == sorted[less]);
}

template <class T>
void matrix_test() {
    std::mt19937_64 rng(469);
    std::vector<T> a = {std::numeric_limits<T>::min(), T(0), T(3), T(3),
                        std::numeric_limits<T>::max()};
    kyopro::dynamic_wavelet_matrix<T> wm(a);
    for (int l = 0; l <= wm.size(); l++) {
        for (int r = l; r <= wm.size(); r++) {
            check(wm, a, l, r, std::numeric_limits<T>::min(),
                  std::numeric_limits<T>::max());
            check(wm, a, l, r, std::numeric_limits<T>::max(), T(0));
            check(wm, a, l, r, T(3), T(4));
        }
    }
    auto random_value = [&]() -> T {
        if (rng() % 7 == 0) return std::numeric_limits<T>::min();
        if (rng() % 7 == 0) return std::numeric_limits<T>::max();
        T x = static_cast<T>(rng() & static_cast<std::uint64_t>(std::numeric_limits<T>::max()));
        if constexpr (std::is_signed<T>::value) {
            if (rng() & 1) return -x;
        }
        return x;
    };
    for (int step = 0; step < 1800; step++) {
        int op = rng() % 4;
        if (a.empty() || op == 0 || (op == 1 && a.size() < 180)) {
            T x = random_value();
            int i = rng() % (a.size() + 1);
            if (step % 5 == 0) i = 0;
            if (step % 7 == 0) i = a.size();
            if (i == static_cast<int>(a.size())) wm.push_back(x);
            else wm.insert(i, x);
            a.insert(a.begin() + i, x);
        } else if (op == 2) {
            int i = rng() % a.size();
            wm.erase(i);
            a.erase(a.begin() + i);
        } else {
            int i = rng() % a.size();
            T x = random_value();
            wm.set(i, x);
            a[i] = x;
        }
        int l = rng() % (a.size() + 1), r = rng() % (a.size() + 1);
        if (r < l) std::swap(l, r);
        check(wm, a, l, r, random_value(), random_value());
        if (step % 31 == 0) {
            for (int i = 0; i < wm.size(); i++) {
                assert(wm.access(i) == a[i]);
                assert(wm.get(i) == a[i]);
                assert(wm[i] == a[i]);
            }
        }
    }
    kyopro::wavelet_matrix<T> static_wm(a);
    for (int k = 0; k < wm.size(); k++) {
        assert(wm.kth_smallest(0, wm.size(), k) ==
               static_wm.kth_smallest(0, wm.size(), k));
    }
    auto copy = wm;
    copy.push_back(T(1));
    assert(wm.size() == static_cast<int>(a.size()));
    while (!wm.empty()) wm.erase(wm.size() / 2);
    wm.push_back(T(1));
    assert(wm[0] == T(1));
    wm.build({T(2), T(2), T(2)});
    assert(wm.count(0, 3, T(2)) == 3);
    wm.set(1, T(2));
    assert(wm.kth_smallest(0, 3, 2) == T(2));
    wm.build({});
    assert(wm.empty());
    assert(wm.range_freq(0, 0, T(1)) == 0);
    assert(wm.count(0, 0, T(0)) == 0);
}

int main() {
    bit_vector_test();
    matrix_test<int>();
    matrix_test<long long>();
    matrix_test<unsigned long long>();
    matrix_test<std::int8_t>();
    matrix_test<std::uint8_t>();
    kyopro::dynamic_wavelet_matrix<> empty;
    assert(empty.empty());
    empty.insert(0, -1);
    assert(empty.access(0) == -1);
    empty.erase(0);
    assert(empty.count(0, 0, 0) == 0);
}
