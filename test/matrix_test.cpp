#include <bits/stdc++.h>

#include <atcoder/modint>

#include "kyopro/matrix.hpp"

using namespace std;
using mint = atcoder::modint998244353;

int main() {
    {
        kyopro::matrix<long long> a({
            {1, 1},
            {1, 0},
        });

        auto b = a.pow(10);
        assert(b[0][0] == 89);
        assert(b[0][1] == 55);
        assert(b[1][0] == 55);
        assert(b[1][1] == 34);

        auto c = kyopro::matrix_power(a, 0);
        assert(c[0][0] == 1);
        assert(c[0][1] == 0);
        assert(c[1][0] == 0);
        assert(c[1][1] == 1);
    }

    {
        kyopro::matrix<int> a({
            {1, 2, 3},
            {4, 5, 6},
        });
        kyopro::matrix<int> b({
            {7, 8},
            {9, 10},
            {11, 12},
        });

        auto c = a * b;
        assert(c.height() == 2);
        assert(c.width() == 2);
        assert(c[0][0] == 58);
        assert(c[0][1] == 64);
        assert(c[1][0] == 139);
        assert(c[1][1] == 154);
    }

    {
        kyopro::matrix<int> a(2, 2);
        a[0][0] = 1;
        a[0][1] = 2;
        a[1][0] = 3;
        a[1][1] = 4;

        kyopro::matrix<int> b({
            {5, 6},
            {7, 8},
        });

        auto sum = a + b;
        assert(sum[0][0] == 6);
        assert(sum[0][1] == 8);
        assert(sum[1][0] == 10);
        assert(sum[1][1] == 12);

        auto diff = sum - b;
        assert(diff.data() == a.data());
    }

    {
        kyopro::matrix<mint> a({
            {mint(1), mint(1)},
            {mint(1), mint(0)},
        });

        auto b = a.pow(100);
        assert(b[0][1].val() == 494958974);
    }

    {
        const kyopro::matrix<mint> a({{0, 2}, {3, 4}});
        auto original = a.data();
        assert(a.determinant() == mint(-6));
        assert(a.rank() == 2);
        auto inv = a.inverse();
        assert(inv.has_value());
        assert((a * *inv).data() == kyopro::matrix<mint>::identity(2).data());
        assert((*inv * a).data() == kyopro::matrix<mint>::identity(2).data());
        assert(a.data() == original);
        assert(a.inverse()->data() == inv->data());
    }

    {
        kyopro::matrix<mint> singular({{1, 2, 3}, {2, 4, 6}, {0, 0, 0}});
        assert(singular.determinant() == mint(0));
        assert(singular.rank() == 1);
        assert(!singular.inverse());
        kyopro::matrix<mint> wide({{0, 1, 2, 3}, {0, 0, 0, 1}});
        assert(wide.rank() == 2);
        kyopro::matrix<mint> tall({{0, 0}, {0, 1}, {0, 2}, {0, 0}});
        assert(tall.rank() == 1);
        assert(kyopro::matrix<mint>(3, 4).rank() == 0);
        assert(kyopro::matrix<mint>(3, 0).rank() == 0);
    }

    {
        kyopro::matrix<mint> empty;
        assert(empty.determinant() == mint(1));
        assert(empty.rank() == 0);
        auto inv = empty.inverse();
        assert(inv && inv->height() == 0 && inv->width() == 0);
        kyopro::matrix<mint> scalar(1, 1, mint(5));
        assert(scalar.determinant() == mint(5));
        assert((*scalar.inverse())[0][0] == mint(5).inv());
        kyopro::matrix<mint> permutation({{0, 1, 0}, {0, 0, 1}, {1, 0, 0}});
        assert(permutation.determinant() == mint(1));
        assert((permutation * *permutation.inverse()).data() == kyopro::matrix<mint>::identity(3).data());
    }

    {
        using small_mint = atcoder::static_modint<3>;
        for (int a = 0; a < 3; a++) for (int b = 0; b < 3; b++) {
            for (int c = 0; c < 3; c++) for (int d = 0; d < 3; d++) {
                kyopro::matrix<small_mint> m({{a, b}, {c, d}});
                small_mint det = small_mint(a) * d - small_mint(b) * c;
                assert(m.determinant() == det);
                int expected_rank = det != small_mint(0) ? 2 : (a || b || c || d ? 1 : 0);
                assert(m.rank() == expected_rank);
                auto inv = m.inverse();
                assert(inv.has_value() == (det != small_mint(0)));
                if (inv) {
                    auto identity = kyopro::matrix<small_mint>::identity(2);
                    assert((m * *inv).data() == identity.data());
                    assert((*inv * m).data() == identity.data());
                }
            }
        }
    }

    {
        using dynamic_mint = atcoder::dynamic_modint<42>;
        dynamic_mint::set_mod(7);
        kyopro::matrix<dynamic_mint> a({{2, 1}, {1, 1}});
        assert(a.determinant() == dynamic_mint(1));
        assert(a.rank() == 2);
        assert((a * *a.inverse()).data() == kyopro::matrix<dynamic_mint>::identity(2).data());
    }

    {
        const kyopro::matrix<mint> a({{0, 2}, {3, 4}});
        const vector<mint> b = {4, 11};
        auto original = a.data();
        auto result = a.solve(b);
        assert(result.exists && result.basis.empty());
        assert(result.solution == vector<mint>({1, 2}));
        assert(a.data() == original && b == vector<mint>({4, 11}));
        assert(a.solve(b).solution == result.solution);
    }

    {
        kyopro::matrix<mint> a({{0, 1, 2, 0}, {0, 2, 4, 0}, {0, 0, 0, 1}});
        auto result = a.solve(vector<mint>{3, 6, 5});
        assert(result.exists);
        assert(result.solution == vector<mint>({0, 3, 0, 5}));
        assert(result.basis == vector<vector<mint>>({{1, 0, 0, 0}, {0, -2, 1, 0}}));
        auto impossible = a.solve(vector<mint>{3, 7, 5});
        assert(!impossible.exists && impossible.solution.empty() && impossible.basis.empty());
        kyopro::matrix<mint> tall({{1, 0}, {0, 1}, {1, 1}});
        assert(tall.solve(vector<mint>{2, 3, 5}).solution == vector<mint>({2, 3}));
        assert(!tall.solve(vector<mint>{2, 3, 6}).exists);
    }

    {
        kyopro::matrix<mint> no_equations(0, 3);
        assert(no_equations.width() == 3);
        auto result = no_equations.solve({});
        assert(result.exists && result.solution == vector<mint>(3, mint(0)));
        assert(result.basis == kyopro::matrix<mint>::identity(3).data());
        kyopro::matrix<mint> no_variables(2, 0);
        auto zero = no_variables.solve(vector<mint>{0, 0});
        assert(zero.exists && zero.solution.empty() && zero.basis.empty());
        assert(!no_variables.solve(vector<mint>{0, 1}).exists);
        kyopro::matrix<mint> empty;
        auto empty_result = empty.solve({});
        assert(empty_result.exists && empty_result.solution.empty() && empty_result.basis.empty());
        assert((no_equations * kyopro::matrix<mint>(3, 2)).width() == 2);
    }

    {
        using binary_mint = atcoder::static_modint<2>;
        for (int h = 0; h <= 2; h++) for (int w = 0; w <= 3; w++) {
            for (int mask = 0; mask < (1 << (h * w)); mask++) {
                kyopro::matrix<binary_mint> a(h, w);
                for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
                    a[i][j] = (mask >> (i * w + j)) & 1;
                }
                for (int rhs_mask = 0; rhs_mask < (1 << h); rhs_mask++) {
                    vector<binary_mint> b(h);
                    for (int i = 0; i < h; i++) b[i] = (rhs_mask >> i) & 1;
                    set<vector<int>> expected;
                    for (int x_mask = 0; x_mask < (1 << w); x_mask++) {
                        vector<int> x(w);
                        for (int j = 0; j < w; j++) x[j] = (x_mask >> j) & 1;
                        bool valid = true;
                        for (int i = 0; i < h; i++) {
                            binary_mint sum = 0;
                            for (int j = 0; j < w; j++) sum += a[i][j] * x[j];
                            if (sum != b[i]) valid = false;
                        }
                        if (valid) expected.insert(x);
                    }
                    auto result = a.solve(b);
                    assert(result.exists == !expected.empty());
                    if (!result.exists) {
                        assert(result.solution.empty() && result.basis.empty());
                        continue;
                    }
                    assert((int)result.solution.size() == w);
                    assert((int)result.basis.size() == w - a.rank());
                    set<vector<int>> actual;
                    int dimension = (int)result.basis.size();
                    for (int coefficients = 0; coefficients < (1 << dimension); coefficients++) {
                        auto x = result.solution;
                        for (int k = 0; k < dimension; k++) if ((coefficients >> k) & 1) {
                            assert((int)result.basis[k].size() == w);
                            for (int j = 0; j < w; j++) x[j] += result.basis[k][j];
                        }
                        vector<int> values(w);
                        for (int j = 0; j < w; j++) values[j] = x[j].val();
                        actual.insert(values);
                    }
                    assert((int)actual.size() == (1 << dimension));
                    assert(actual == expected);
                }
            }
        }
    }

    return 0;
}
