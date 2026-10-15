#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "kyopro/lis.hpp"

template <class T, class Compare>
void check(const std::vector<T>& a, bool strict, Compare compare) {
    int n = static_cast<int>(a.size());
    std::vector<int> dp(n, 1);
    int expected = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            bool extends = strict ? compare(a[j], a[i]) : !compare(a[i], a[j]);
            if (extends) dp[i] = std::max(dp[i], dp[j] + 1);
        }
        expected = std::max(expected, dp[i]);
    }
    auto result = kyopro::lis(a, strict, compare);
    assert(result.length() == expected);
    assert(kyopro::lis_length(a, strict, compare) == expected);
    assert(static_cast<int>(result.values.size()) == expected);
    for (int k = 0; k < expected; k++) {
        int i = result.indices[k];
        assert(0 <= i && i < n);
        assert(result.values[k] == a[i]);
        if (k == 0) continue;
        int j = result.indices[k - 1];
        assert(j < i);
        assert(strict ? compare(a[j], a[i]) : !compare(a[i], a[j]));
    }
}

int main() {
    std::vector<int> a = {3, 1, 2, 2, 4};
    auto strict = kyopro::lis(a);
    assert(strict.length() == 3);
    assert(strict.values == std::vector<int>({1, 2, 4}));
    assert(kyopro::lis_length(a) == 3);
    auto nonstrict = kyopro::lis(a, false);
    assert(nonstrict.length() == 4);
    assert(nonstrict.indices == std::vector<int>({1, 2, 3, 4}));
    assert(nonstrict.values == std::vector<int>({1, 2, 2, 4}));

    for (const auto& values : std::vector<std::vector<int>>{
             {}, {42}, {2, 2, 2, 2}, {1, 2, 3, 4}, {4, 3, 2, 1},
             {1, 8, 2, 7, 3, 6, 4, 5}}) {
        for (bool is_strict : {false, true}) {
            check(values, is_strict, std::less<int>());
            check(values, is_strict, std::greater<int>());
        }
    }
    std::vector<long long> extremes = {
        std::numeric_limits<long long>::max(),
        std::numeric_limits<long long>::min(), 0, 0,
        std::numeric_limits<long long>::max()};
    for (bool is_strict : {false, true}) {
        check(extremes, is_strict, std::less<long long>());
        check(extremes, is_strict, std::greater<long long>());
        check(std::vector<std::string>{"c", "a", "b", "b", "d"},
              is_strict, std::less<std::string>());
    }

    // Comparison uses only <, not a numeric sentinel or an equality operator.
    struct item {
        int key;
        explicit item(int value) : key(value) {}
    };
    std::vector<item> items = {item(3), item(1), item(2), item(2)};
    auto by_key = [](const item& x, const item& y) { return x.key < y.key; };
    assert(kyopro::lis_length(items, true, by_key) == 2);
    assert(kyopro::lis(items, false, by_key).length() == 3);

    std::mt19937 rng(469);
    for (int trial = 0; trial < 1500; trial++) {
        std::vector<int> values(rng() % 45);
        for (int& x : values) x = static_cast<int>(rng() % 11) - 5;
        auto original = values;
        for (bool is_strict : {false, true}) {
            check(values, is_strict, std::less<int>());
            check(values, is_strict, std::greater<int>());
        }
        assert(values == original);
    }

    std::vector<int> large(200000);
    for (int i = 0; i < static_cast<int>(large.size()); i++) large[i] = i;
    auto result = kyopro::lis(large);
    assert(result.length() == static_cast<int>(large.size()));
    assert(result.values == large);
}
