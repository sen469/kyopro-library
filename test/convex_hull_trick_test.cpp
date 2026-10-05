#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include "kyopro/convex_hull_trick.hpp"

using ll = long long;
using wide = __int128;

template <bool is_min>
void check(const std::vector<std::pair<ll, ll>>& lines, const std::vector<ll>& xs) {
    kyopro::convex_hull_trick<ll, is_min> cht;
    assert(cht.empty());
    assert(cht.size() == 0);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        cht.add_line(lines[i].first, lines[i].second);
        for (ll x : xs) {
            wide expected = static_cast<wide>(lines[0].first) * x + lines[0].second;
            for (std::size_t j = 1; j <= i; ++j) {
                wide value = static_cast<wide>(lines[j].first) * x + lines[j].second;
                expected = is_min ? std::min(expected, value) : std::max(expected, value);
            }
            assert(cht.query_wide(x) == expected);
            if (std::numeric_limits<ll>::lowest() <= expected &&
                expected <= std::numeric_limits<ll>::max()) {
                assert(cht.query(x) == expected);
            }
        }
    }
    const auto copy = cht;
    if (!lines.empty()) assert(copy.query_wide(0) == cht.query_wide(0));
    cht.clear();
    assert(cht.empty());
    assert(cht.size() == 0);
    cht.add_line(17, -4);
    assert(cht.query(2) == 30);
}

int main() {
    check<true>({{3, 2}, {3, 7}, {3, -1}, {1, 0}, {-2, 4}}, {0, 10, -10, 2, -1});
    check<false>({{-3, 2}, {-3, -7}, {-3, 4}, {1, 0}, {2, -4}}, {0, 10, -10, 2, -1});
    check<true>({{5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}}, {-1, 0, 1});
    check<true>({{3, -1}, {2, 0}, {1, -1}}, {-3, -1, 0, 1, 3});
    check<false>({{1, -1}, {2, 0}, {3, -1}}, {-3, -1, 0, 1, 3});

    const ll low = std::numeric_limits<ll>::lowest();
    const ll high = std::numeric_limits<ll>::max();
    const std::vector<ll> extremes = {low, low + 1, -1, 0, 1, high - 1, high};
    check<true>({{high, low}, {high - 1, high}, {low, low}}, extremes);
    check<true>({{high, high}, {high - 1, low}, {low, high}}, extremes);
    check<false>({{low, high}, {low + 1, low}, {high, high}}, extremes);
    check<false>({{low, low}, {low + 1, high}, {high, low}}, extremes);
    check<true>({{low, high}, {low, low}, {low, low}}, extremes);
    check<false>({{low, low}, {low, high}, {low, high}}, extremes);

    std::mt19937 rng(469);
    for (int trial = 0; trial < 100; ++trial) {
        std::vector<std::pair<ll, ll>> lines;
        std::vector<ll> xs;
        for (int i = 0; i < 50; ++i) {
            lines.emplace_back(static_cast<int>(rng() % 41) - 20,
                               static_cast<int>(rng() % 201) - 100);
            xs.push_back(static_cast<int>(rng() % 201) - 100);
        }
        std::sort(lines.begin(), lines.end());
        check<false>(lines, xs);
        std::reverse(lines.begin(), lines.end());
        check<true>(lines, xs);
    }

    kyopro::convex_hull_trick<int> small;
    small.add_line(2, 1);
    small.add_line(-1, 3);
    assert(small.query(4) == -1);
    assert(small.query(-4) == -7);
    kyopro::convex_hull_trick<> same_slope;
    same_slope.add_line(0, 1);
    same_slope.add_line(0, 2);
    same_slope.add_line(0, -1);
    assert(same_slope.size() == 1);
    assert(same_slope.query(0) == -1);
}
