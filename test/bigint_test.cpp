#include <bits/stdc++.h>

#include "kyopro/bigint.hpp"

using namespace std;

string multiply_decimal(string a, string b) {
    bool neg = false;
    if (a[0] == '-') {
        neg = !neg;
        a = a.substr(1);
    }
    if (b[0] == '-') {
        neg = !neg;
        b = b.substr(1);
    }
    vector<int> c(a.size() + b.size());
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        for (int j = (int)b.size() - 1; j >= 0; j--) {
            c[i + j + 1] += (a[i] - '0') * (b[j] - '0');
        }
    }
    for (int i = (int)c.size() - 1; i > 0; i--) {
        c[i - 1] += c[i] / 10;
        c[i] %= 10;
    }
    string s;
    int i = 0;
    while (i + 1 < (int)c.size() && c[i] == 0) i++;
    for (; i < (int)c.size(); i++) s += char('0' + c[i]);
    if (s != "0" && neg) s = "-" + s;
    return s;
}

string random_decimal(mt19937& rng, int len) {
    string s(len, '0');
    s[0] = char('1' + rng() % 9);
    for (int i = 1; i < len; i++) s[i] = char('0' + rng() % 10);
    if (rng() % 2) s = "-" + s;
    return s;
}

int main() {
    using kyopro::bigint;

    bigint a, b;
    {
        istringstream iss("10000000000000000000000000000000000000 -1");
        iss >> a >> b;
        assert((a + b).to_string() == "9999999999999999999999999999999999999");
    }

    assert(bigint("0").to_string() == "0");
    assert(bigint("-0").to_string() == "0");
    assert(bigint("00000123").to_string() == "123");
    assert(bigint("-00000123").to_string() == "-123");

    assert((bigint("10000000000000000000000000000000000000") +
            bigint("10000000000000000000000000000000000000"))
               .to_string() == "20000000000000000000000000000000000000");
    assert((bigint("-10000000000000000000000000000000000000") +
            bigint("10000000000000000000000000000000000000"))
               .to_string() == "0");
    assert((bigint("-10000000000000000000000000000000000000") + bigint("-1")).to_string() ==
           "-10000000000000000000000000000000000001");

    assert((bigint("10000000000000000000000000000000000000") - bigint("1")).to_string() ==
           "9999999999999999999999999999999999999");
    assert((bigint("1") - bigint("10000000000000000000000000000000000000")).to_string() ==
           "-9999999999999999999999999999999999999");

    assert(bigint("12345678901234567890") * bigint("9876543210") ==
           bigint("121932631124828532111263526900"));
    assert(bigint("-123456789") * bigint("987654321") == bigint("-121932631112635269"));

    assert(bigint("121932631124828532111263526900") / bigint("9876543210") ==
           bigint("12345678901234567890"));
    assert(bigint("121932631124828532111263526901") % bigint("9876543210") == bigint("1"));
    assert(bigint("7") / bigint("3") == bigint("2"));
    assert(bigint("7") % bigint("3") == bigint("1"));
    assert(bigint("-7") / bigint("3") == bigint("-2"));
    assert(bigint("-7") % bigint("3") == bigint("-1"));
    assert(bigint("7") / bigint("-3") == bigint("-2"));
    assert(bigint("7") % bigint("-3") == bigint("1"));
    assert(bigint("-7") / bigint("-3") == bigint("2"));
    assert(bigint("-7") % bigint("-3") == bigint("-1"));
    {
        auto [q, r] = divmod(bigint("-100000000000000000000"), bigint("97"));
        assert(q == bigint("-1030927835051546391"));
        assert(r == bigint("-73"));
    }

    assert(bigint("-10") < bigint("-2"));
    assert(bigint("999999999999999999999") > bigint("999999999999999999998"));
    assert(bigint("42") == bigint(42));

    __int128 x = ((__int128)1 << 100) + 12345;
    bigint bx = x;
    assert(bx.to_string() == "1267650600228229401496703217721");
    assert(bx.to_integral<__int128>() == x);
    assert(bigint(-x).to_integral<__int128>() == -x);

    mt19937 rng(0);
    for (int len_a : {1, 2, 9, 10, 80, 90, 180, 260, 800}) {
        for (int len_b : {1, 7, 81, 120, 240, 760}) {
            string sa = random_decimal(rng, len_a);
            string sb = random_decimal(rng, len_b);
            bigint got = bigint(sa) * bigint(sb);
            assert(got.to_string() == multiply_decimal(sa, sb));
        }
    }

    for (int len_a : {1, 2, 9, 10, 80, 180, 800}) {
        for (int len_b : {1, 7, 81, 120, 240}) {
            string sa = random_decimal(rng, len_a);
            string sb = random_decimal(rng, len_b);
            bigint a = bigint(sa), b = bigint(sb);
            auto [q, r] = divmod(a, b);
            assert(q * b + r == a);
            assert(r.abs() < b.abs());
            assert(r.is_zero() || r.sign() == a.sign());
        }
    }

    return 0;
}
