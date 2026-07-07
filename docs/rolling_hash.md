# Rolling Hash

実装: [lib/kyopro/rolling_hash.hpp](../lib/kyopro/rolling_hash.hpp)

文字列や整数列の部分列ハッシュを求めます。
法は $2^{61} - 1$ です。

```cpp
#include "kyopro/rolling_hash.hpp"
```

## コンストラクタ

```cpp
rolling_hash();
```

長さ 0 の列に対する Rolling Hash を作ります。

**計算量**

- $O(1)$

```cpp
explicit rolling_hash(const string& s, ull base = rolling_hash::default_base());
```

文字列 `s` から Rolling Hash を作ります。
各文字は `unsigned char` として扱われます。

**引数**

- `const string& s`: 対象の文字列
- `ull base`: 基数

**制約**

- `1 <= base < rolling_hash::mod()`

**計算量**

- $O(n)$

```cpp
template <class T>
explicit rolling_hash(const vector<T>& v, ull base = rolling_hash::default_base());
```

整数列 `v` から Rolling Hash を作ります。

**引数**

- `const vector<T>& v`: 対象の整数列
- `ull base`: 基数

**制約**

- `1 <= base < rolling_hash::mod()`
- `T` は `unsigned long long` に変換可能

**計算量**

- $O(n)$

## build

```cpp
void rh.build(const string& s, ull base = rolling_hash::default_base());
```

文字列 `s` から再構築します。

**制約**

- `1 <= base < rolling_hash::mod()`

**計算量**

- $O(n)$

```cpp
template <class T>
void rh.build(const vector<T>& v, ull base = rolling_hash::default_base());
```

整数列 `v` から再構築します。

**制約**

- `1 <= base < rolling_hash::mod()`
- `T` は `unsigned long long` に変換可能

**計算量**

- $O(n)$

## size

```cpp
int rh.size() const;
```

列の長さを返します。

**計算量**

- $O(1)$

## empty

```cpp
bool rh.empty() const;
```

列の長さが 0 なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

## base

```cpp
ull rh.base() const;
```

基数を返します。

**計算量**

- $O(1)$

## get

```cpp
ull rh.get(int l, int r) const;
```

半開区間 `[l, r)` のハッシュ値を返します。

**制約**

- `0 <= l <= r <= rh.size()`

**計算量**

- $O(1)$

## all

```cpp
ull rh.all() const;
```

列全体のハッシュ値を返します。

**計算量**

- $O(1)$

## concat

```cpp
ull rh.concat(ull left_hash, ull right_hash, int right_length) const;
```

`left_hash` の後ろに、長さ `right_length` の `right_hash` を連結したハッシュ値を返します。

**制約**

- `0 <= right_length <= rh.size()`
- `left_hash`, `right_hash` は `rh` と同じ基数で作られている

**計算量**

- $O(1)$

## same

```cpp
bool rh.same(int l1, int r1, int l2, int r2) const;
```

半開区間 `[l1, r1)` と `[l2, r2)` のハッシュ値が等しいなら `true`、そうでなければ `false` を返します。
長さが異なる場合は `false` を返します。

**制約**

- `0 <= l1 <= r1 <= rh.size()`
- `0 <= l2 <= r2 <= rh.size()`

**計算量**

- $O(1)$

## lcp

```cpp
int rh.lcp(int a, int b) const;
```

suffix `a` と suffix `b` の LCP の長さを返します。

**制約**

- `0 <= a, b <= rh.size()`

**計算量**

- $O(\log n)$

```cpp
int rh.lcp(int l1, int r1, int l2, int r2) const;
```

半開区間 `[l1, r1)` と `[l2, r2)` の LCP の長さを返します。

**制約**

- `0 <= l1 <= r1 <= rh.size()`
- `0 <= l2 <= r2 <= rh.size()`

**計算量**

- $O(\log n)$

## static mod

```cpp
static constexpr ull rolling_hash::mod();
```

法 $2^{61} - 1$ を返します。

**計算量**

- $O(1)$

## static default_base

```cpp
static constexpr ull rolling_hash::default_base();
```

デフォルトの基数を返します。

**計算量**

- $O(1)$

## 使用例

```cpp
string s = "abracadabra";
kyopro::rolling_hash rh(s);

cout << rh.same(0, 4, 7, 11) << '\n'; // 1
cout << rh.lcp(0, 7) << '\n';         // 4

auto [l, r] = pair<int, int>{3, 7};
auto h = rh.get(l, r);
```
