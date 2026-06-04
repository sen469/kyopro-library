# Arbitrary Mod Convolution

任意 mod で畳み込みを行います。
3 つの NTT friendly prime で畳み込み、CRT で指定した mod に復元します。

```cpp
#include "kyopro/arbitrary_mod_convolution.hpp"
```

## arbitrary_mod_convolution

```cpp
template <class T>
vector<int> arbitrary_mod_convolution(
    const vector<T>& a,
    const vector<T>& b,
    int mod
);
```

`a` と `b` の畳み込みを `mod` で割った値として返します。

**引数**

- `const vector<T>& a`: 左辺の列
- `const vector<T>& b`: 右辺の列
- `int mod`: 法

**戻り値**

- 長さ `a.size() + b.size() - 1` の `vector<int>`
- `res[k] = sum_{i + j = k} a[i] * b[j] mod mod`
- `a` または `b` が空の場合は空の `vector<int>`
- `mod == 1` の場合は全要素 `0`

**制約**

- `1 <= mod`
- `T` は整数型
- `a.size() + b.size() - 1 <= 2^24`

**計算量**

- $O(n \log n)$

## コンパイル時 mod 版

```cpp
template <int MOD, class T>
vector<int> arbitrary_mod_convolution(
    const vector<T>& a,
    const vector<T>& b
);
```

`MOD` をコンパイル時定数として指定します。

**引数**

- `const vector<T>& a`: 左辺の列
- `const vector<T>& b`: 右辺の列

**戻り値**

- `arbitrary_mod_convolution(a, b, MOD)` と同じ

**制約**

- `1 <= MOD`
- `T` は整数型
- `a.size() + b.size() - 1 <= 2^24`

**計算量**

- $O(n \log n)$

## 使用例

```cpp
vector<long long> a = {1, 2, 3};
vector<long long> b = {4, 5};

auto c = kyopro::arbitrary_mod_convolution(a, b, 1000000007);
// c = {4, 13, 22, 15}
```
## 問題例
[Convolution (Mod 1,000,000,007)](https://judge.yosupo.jp/problem/convolution_mod_1000000007)
