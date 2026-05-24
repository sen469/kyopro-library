# Matrix

行列の加算、減算、乗算、累乗を行います。
`long long` や `atcoder::modint` など、`+`, `-`, `*`, `T(0)`, `T(1)` が使える型で利用できます。

```cpp
#include "kyopro/matrix.hpp"
```

## matrix

```cpp
kyopro::matrix<long long> a({
    {1, 1},
    {1, 0},
});
```

`matrix<T>` は `vector<vector<T>>` で値を保持します。
`a[i][j]` で要素にアクセスできます。

## identity

```cpp
auto e = kyopro::matrix<long long>::identity(n);
```

`n * n` の単位行列を返します。

## 演算

```cpp
auto c = a + b;
auto d = a - b;
auto e = a * b;
```

行列の加算、減算、乗算ができます。

**計算量**

- 加算、減算: $O(hw)$
- 乗算: $O(hwk)$

## pow

```cpp
auto b = a.pow(k);
auto c = kyopro::matrix_power(a, k);
```

正方行列 `a` の `k` 乗を二分累乗で求めます。
`k = 0` の場合は単位行列を返します。

**計算量**

- $O(n^3 \log k)$

## 問題
- [Pow of Matrix](https://judge.yosupo.jp/submission/374671)

## 使用例

```cpp
kyopro::matrix<long long> a({
    {1, 1},
    {1, 0},
});

auto b = a.pow(10);
cout << b[0][1] << '\n'; // 55
```
