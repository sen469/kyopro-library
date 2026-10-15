#include <bits/stdc++.h>

#include "kyopro/angular_sort.hpp"

using namespace std;

void options_test() {
    using point = pair<int, int>;
    vector<point> points;
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) points.emplace_back(x, y);
    }
    points.emplace_back(0, 0);
    const long double tau = 2 * acosl(-1.0L);
    for (point start : vector<point>{{1, 0}, {0, 1}, {-1, 0}, {0, -1},
                                     {1, 2}, {-1, 2}, {-1, -2}, {1, -2}}) {
        for (bool include_start : {false, true}) {
            for (bool clockwise : {false, true}) {
                for (auto origin : {kyopro::angular_origin::first,
                                    kyopro::angular_origin::last,
                                    kyopro::angular_origin::direction}) {
                    for (point origin_direction : vector<point>{start, {1, 0}, {1, 1}}) {
                        kyopro::angular_sort_options<int> options;
                        options.start = start;
                        options.include_start = include_start;
                        options.clockwise = clockwise;
                        options.origin = origin;
                        options.origin_direction = origin_direction;
                        auto angle = [&](point p) {
                            if (p == point{0, 0}) {
                                if (origin == kyopro::angular_origin::first) return -1.0L;
                                if (origin == kyopro::angular_origin::last) return tau + 1;
                                p = origin_direction;
                            }
                            long double cross = start.first * p.second - start.second * p.first;
                            long double dot = start.first * p.first + start.second * p.second;
                            if (clockwise) cross = -cross;
                            long double theta = atan2l(cross, dot);
                            if (theta < 0) theta += tau;
                            if (!include_start && theta == 0) theta = tau;
                            return theta;
                        };
                        auto expected = points;
                        sort(expected.begin(), expected.end(), [&](point a, point b) {
                            long double diff = angle(a) - angle(b);
                            if (abs(diff) > 1e-12L) return diff < 0;
                            return a.first * a.first + a.second * a.second <
                                   b.first * b.first + b.second * b.second;
                        });
                        auto actual = points;
                        kyopro::angular_sort(actual, options);
                        assert(actual == expected);
                        auto order = kyopro::angular_sorted_indices(points, options);
                        for (int i = 0; i < (int)points.size(); i++) {
                            assert(points[order[i]] == expected[i]);
                        }
                        kyopro::polar_angle_less<int> less(options);
                        for (point a : points) {
                            assert(!less(a, a));
                            for (point b : points) {
                                if (less(a, b)) assert(!less(b, a));
                                for (point c : points) {
                                    if (less(a, b) && less(b, c)) assert(less(a, c));
                                    if (!less(a, b) && !less(b, a) &&
                                        !less(b, c) && !less(c, b)) {
                                        assert(!less(a, c) && !less(c, a));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    options_test();
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

    {
        vector<pair<int, int>> points = {
            {0, -1}, {1, 0}, {-1, 0}, {0, 1}, {0, 0}, {0, 2},
        };
        auto original = points;
        auto order = kyopro::angular_sorted_indices(points, {0, 1});
        assert(points == original);
        assert(order == vector<int>({4, 3, 5, 2, 0, 1}));
        kyopro::angular_sort(points, {0, 1});
        vector<pair<int, int>> expected = {
            {0, 0}, {0, 1}, {0, 2}, {-1, 0}, {0, -1}, {1, 0},
        };
        assert(points == expected);
    }

    {
        // Start and opposite rays, including distance ties and duplicates.
        vector<pair<long long, long long>> points = {
            {-2, -4}, {1, 2}, {0, 0}, {2, 4}, {-1, -2}, {1, 0},
            {0, 1}, {1, 2},
        };
        vector<pair<long long, long long>> expected = {
            {0, 0}, {1, 2}, {1, 2}, {2, 4}, {0, 1}, {-1, -2},
            {-2, -4}, {1, 0},
        };
        sort(points.begin(), points.end(), kyopro::polar_angle_less<long long>({1, 2}));
        assert(points == expected);
    }

    {
        vector<pair<double, double>> points = {
            {1.0, 0.0}, {0.5, 1.0}, {-0.5, -1.0}, {0.0, 1.0}, {0.0, 0.0},
        };
        kyopro::angular_sort(points, {0.5, 1.0});
        vector<pair<double, double>> expected = {
            {0.0, 0.0}, {0.5, 1.0}, {0.0, 1.0}, {-0.5, -1.0}, {1.0, 0.0},
        };
        assert(points == expected);
    }

    {
        // Compare every small integer start direction against an atan2 oracle.
        vector<pair<int, int>> points;
        for (int x = -3; x <= 3; x++) {
            for (int y = -3; y <= 3; y++) points.emplace_back(x, y);
        }
        for (int sx = -3; sx <= 3; sx++) {
            for (int sy = -3; sy <= 3; sy++) {
                if (sx == 0 && sy == 0) continue;
                pair<int, int> start = {sx, sy};
                auto angle = [&](pair<int, int> p) {
                    if (p == pair<int, int>{0, 0}) return -1.0L;
                    long double cross = sx * p.second - sy * p.first;
                    long double dot = sx * p.first + sy * p.second;
                    long double theta = atan2l(cross, dot);
                    if (theta < 0) theta += 2 * acosl(-1.0L);
                    return theta;
                };
                auto expected = points;
                sort(expected.begin(), expected.end(), [&](auto a, auto b) {
                    long double difference = angle(a) - angle(b);
                    if (abs(difference) > 1e-12L) return difference < 0;
                    return a.first * a.first + a.second * a.second <
                           b.first * b.first + b.second * b.second;
                });
                auto actual = points;
                kyopro::angular_sort(actual, start);
                assert(actual == expected);
                auto indices = kyopro::angular_sorted_indices(points, start);
                for (int i = 0; i < (int)points.size(); i++) {
                    assert(points[indices[i]] == expected[i]);
                }
                auto scaled = points;
                kyopro::angular_sort(scaled, {sx * 3, sy * 3});
                assert(scaled == expected);
                kyopro::polar_angle_less<int> less(start);
                for (auto a : points) {
                    assert(!less(a, a));
                    for (auto b : points) {
                        if (!less(a, b)) continue;
                        assert(!less(b, a));
                        for (auto c : points) {
                            if (less(b, c)) assert(less(a, c));
                        }
                    }
                }
            }
        }
    }

    {
        vector<pair<int, int>> empty;
        kyopro::angular_sort(empty, {-1, -1});
        assert(kyopro::angular_sorted_indices(empty, {-1, -1}).empty());
    }

    {
        // atan2 in (-pi, pi], with atan2(0, 0) = 0.
        vector<pair<int, int>> points = {
            {1, 0}, {0, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {2, 2}, {-10, -1},
        };
        kyopro::angular_sort_options<int> options;
        options.start = {-1, 0};
        options.include_start = false;
        options.origin = kyopro::angular_origin::direction;
        kyopro::angular_sort(points, options);
        vector<pair<int, int>> expected = {
            {-10, -1}, {0, -1}, {0, 0}, {1, 0}, {1, 1},
            {2, 2}, {0, 1}, {-1, 0},
        };
        assert(points == expected);
    }

    {
        vector<pair<long double, long double>> points = {
            {0, 0}, {-1, 0}, {0, -1}, {1, 0}, {0, 1},
        };
        kyopro::angular_sort_options<long double> options;
        options.start = {0, 1};
        options.clockwise = true;
        options.include_start = false;
        options.origin = kyopro::angular_origin::last;
        kyopro::angular_sort(points, options);
        vector<pair<long double, long double>> expected = {
            {1, 0}, {0, -1}, {-1, 0}, {0, 1}, {0, 0},
        };
        assert(points == expected);
    }

    return 0;
}
