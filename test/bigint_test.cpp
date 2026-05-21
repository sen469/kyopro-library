#include <bits/stdc++.h>

#include "kyopro/bigint.hpp"

using namespace std;

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

    assert(bigint("-10") < bigint("-2"));
    assert(bigint("999999999999999999999") > bigint("999999999999999999998"));
    assert(bigint("42") == bigint(42));

    __int128 x = ((__int128)1 << 100) + 12345;
    bigint bx = x;
    assert(bx.to_string() == "1267650600228229401496703217721");
    assert(bx.to_integral<__int128>() == x);
    assert(bigint(-x).to_integral<__int128>() == -x);

    return 0;
}
