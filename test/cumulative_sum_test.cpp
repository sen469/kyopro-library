#include <bits/stdc++.h>

#include "kyopro/cumulative_sum.hpp"

using namespace std;

long long naive_sum(const vector<long long>& v, int l, int r) {
    long long res = 0;
    for (int i = l; i < r; i++) res += v[i];
    return res;
}

long long naive_sum_2d(const vector<vector<long long>>& v, int y1, int x1,
                       int y2, int x2) {
    long long res = 0;
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            res += v[y][x];
        }
    }
    return res;
}

long long naive_sum_3d(const vector<vector<vector<long long>>>& v, int z1,
                       int y1, int x1, int z2, int y2, int x2) {
    long long res = 0;
    for (int z = z1; z < z2; z++) {
        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                res += v[z][y][x];
            }
        }
    }
    return res;
}

int main() {
    {
        kyopro::cumulative_sum<long long> cs;
        assert(cs.size() == 0);
        assert(cs.empty());
        assert(cs.prefix_sum(0) == 0);
        assert(cs.sum(0, 0) == 0);
        assert(cs.all_sum() == 0);
    }

    {
        vector<long long> v = {3, -1, 4, 1, -5, 9};
        kyopro::cumulative_sum<long long> cs(v);
        assert(cs.size() == (int)v.size());
        assert(!cs.empty());
        for (int r = 0; r <= (int)v.size(); r++) {
            assert(cs.prefix_sum(r) == naive_sum(v, 0, r));
        }
        for (int l = 0; l <= (int)v.size(); l++) {
            for (int r = l; r <= (int)v.size(); r++) {
                assert(cs.sum(l, r) == naive_sum(v, l, r));
            }
        }
        assert(cs.all_sum() == naive_sum(v, 0, (int)v.size()));

        cs.build(vector<long long>{10, 20, -7});
        assert(cs.size() == 3);
        assert(cs.sum(0, 2) == 30);
        assert(cs.sum(1, 3) == 13);
    }

    {
        kyopro::cumulative_sum<long long> cs(5);
        assert(cs.size() == 5);
        assert(cs.sum(0, 5) == 0);
        assert(cs.sum(2, 4) == 0);
    }

    {
        kyopro::cumulative_sum_2d<long long> cs;
        assert(cs.height() == 0);
        assert(cs.width() == 0);
        assert(cs.empty());
        assert(cs.prefix_sum(0, 0) == 0);
        assert(cs.sum(0, 0, 0, 0) == 0);
        assert(cs.all_sum() == 0);
    }

    {
        vector<vector<long long>> v = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {-1, -2, -3, -4},
        };
        kyopro::cumulative_sum_2d<long long> cs(v);
        int h = (int)v.size();
        int w = (int)v[0].size();
        assert(cs.height() == h);
        assert(cs.width() == w);
        assert(!cs.empty());

        for (int y = 0; y <= h; y++) {
            for (int x = 0; x <= w; x++) {
                assert(cs.prefix_sum(y, x) == naive_sum_2d(v, 0, 0, y, x));
            }
        }
        for (int y1 = 0; y1 <= h; y1++) {
            for (int y2 = y1; y2 <= h; y2++) {
                for (int x1 = 0; x1 <= w; x1++) {
                    for (int x2 = x1; x2 <= w; x2++) {
                        assert(cs.sum(y1, x1, y2, x2) ==
                               naive_sum_2d(v, y1, x1, y2, x2));
                    }
                }
            }
        }
        assert(cs.all_sum() == naive_sum_2d(v, 0, 0, h, w));
    }

    {
        kyopro::cumulative_sum_2d<long long> cs(2, 3);
        assert(cs.height() == 2);
        assert(cs.width() == 3);
        assert(cs.sum(0, 0, 2, 3) == 0);
        assert(cs.sum(1, 1, 2, 2) == 0);

        cs.build(vector<vector<long long>>{{1}, {2}, {3}});
        assert(cs.height() == 3);
        assert(cs.width() == 1);
        assert(cs.sum(0, 0, 3, 1) == 6);
        assert(cs.sum(1, 0, 3, 1) == 5);
    }

    {
        kyopro::cumulative_sum_3d<long long> cs;
        assert(cs.depth() == 0);
        assert(cs.height() == 0);
        assert(cs.width() == 0);
        assert(cs.empty());
        assert(cs.prefix_sum(0, 0, 0) == 0);
        assert(cs.sum(0, 0, 0, 0, 0, 0) == 0);
        assert(cs.all_sum() == 0);
    }

    {
        vector<vector<vector<long long>>> v = {
            {
                {1, 2, 3},
                {4, 5, 6},
            },
            {
                {-1, -2, -3},
                {7, 8, 9},
            },
        };
        kyopro::cumulative_sum_3d<long long> cs(v);
        int d = (int)v.size();
        int h = (int)v[0].size();
        int w = (int)v[0][0].size();
        assert(cs.depth() == d);
        assert(cs.height() == h);
        assert(cs.width() == w);
        assert(!cs.empty());

        for (int z = 0; z <= d; z++) {
            for (int y = 0; y <= h; y++) {
                for (int x = 0; x <= w; x++) {
                    assert(cs.prefix_sum(z, y, x) ==
                           naive_sum_3d(v, 0, 0, 0, z, y, x));
                }
            }
        }

        for (int z1 = 0; z1 <= d; z1++) {
            for (int z2 = z1; z2 <= d; z2++) {
                for (int y1 = 0; y1 <= h; y1++) {
                    for (int y2 = y1; y2 <= h; y2++) {
                        for (int x1 = 0; x1 <= w; x1++) {
                            for (int x2 = x1; x2 <= w; x2++) {
                                assert(cs.sum(z1, y1, x1, z2, y2, x2) ==
                                       naive_sum_3d(v, z1, y1, x1, z2, y2, x2));
                            }
                        }
                    }
                }
            }
        }
        assert(cs.all_sum() == naive_sum_3d(v, 0, 0, 0, d, h, w));
    }

    {
        kyopro::cumulative_sum_3d<long long> cs(2, 3, 4);
        assert(cs.depth() == 2);
        assert(cs.height() == 3);
        assert(cs.width() == 4);
        assert(cs.sum(0, 0, 0, 2, 3, 4) == 0);
        assert(cs.sum(1, 1, 1, 2, 3, 4) == 0);

        cs.build(vector<vector<vector<long long>>>{
            {{1}, {2}},
            {{3}, {4}},
            {{5}, {6}},
        });
        assert(cs.depth() == 3);
        assert(cs.height() == 2);
        assert(cs.width() == 1);
        assert(cs.sum(0, 0, 0, 3, 2, 1) == 21);
        assert(cs.sum(1, 0, 0, 3, 1, 1) == 8);
    }

    return 0;
}
