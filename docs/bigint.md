# BigInt

```cpp
#include "kyopro/bigint.hpp"
```

符号付き多倍長整数です。標準整数型に収まらない値を `cin` で読み込み、加減乗算 `+`, `-`, `*` と比較を扱えます。
内部は 10^9 進数の `vector<int>` で保持します。

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
x += y
x -= y
x *= y
-x
x < y, x <= y, x > y, x >= y, x == y, x != y
```
