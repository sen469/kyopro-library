# BigInt

実装: [lib/kyopro/bigint.hpp](../lib/kyopro/bigint.hpp)

```cpp
#include "kyopro/bigint.hpp"
```

符号付き多倍長整数です。標準整数型に収まらない値を `cin` で読み込み、四則演算と比較を扱えます。
内部は 10^9 進数の `vector<int>` で保持します。乗算は小さい入力では素朴法、大きい入力では NTT による畳み込みで計算します。

## 例

```cpp
#include <bits/stdc++.h>
#include "kyopro/bigint.hpp"

int main() {
    kyopro::bigint a, b;
    std::cin >> a >> b;
    std::cout << a + b << '\n';
}
```

`a, b` が `[-1e37, 1e37]` の範囲でもそのまま読み込めます。

## コンストラクタ

```cpp
kyopro::bigint x;
kyopro::bigint y("123456789012345678901234567890");
kyopro::bigint z = 123456789LL;
kyopro::bigint w = (__int128)1 << 100;
```

`std::string`, C 文字列、整数型から作成できます。

## メンバ関数

- `to_string()`: 10 進表記の文字列を返します。
- `to_integral<T>()`: `T` に変換します。値が `T` に収まる場合に使ってください。
- `abs()`: 絶対値を返します。
- `sign()`: 正なら `1`、負なら `-1`、ゼロなら `0` を返します。
- `is_zero()`: ゼロ判定です。

## 演算

```cpp
x + y
x - y
x * y
x / y
x % y
x += y
x -= y
x *= y
x /= y
x %= y
-x
x < y, x <= y, x > y, x >= y, x == y, x != y
divmod(x, y)
```

`/`, `%`, `divmod(x, y)` は `y != 0` のとき使えます。負数を含む場合、商は 0 方向に丸め、余りは被除数と同じ符号になります。

## 計算量

- 加減算: 桁数に対して線形時間
- 乗算: 小さい入力では `O(nm)`、大きい入力では `O(n log n)`
- 除算・剰余: 小さい入力では `O(nm)`、大きい入力では Newton 逆数と NTT 乗算で高速化

乗算時は 10^9 進数の内部表現を一時的に 10^3 進数へ分解し、`atcoder::convolution_ll` で正確に畳み込みます。

## 問題
- [Addition of Big Integers](https://judge.yosupo.jp/problem/addition_of_big_integers)
- [Multiplication of Big Integers](https://judge.yosupo.jp/problem/multiplication_of_big_integers)
- [Division of Bit Integers](https://judge.yosupo.jp/problem/division_of_big_integers)
