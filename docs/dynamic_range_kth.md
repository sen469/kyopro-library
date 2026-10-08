# Dynamic Range Kth

[実装の解説](explanations/dynamic_range_kth.md)

実装: [lib/kyopro/dynamic_range_kth.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/dynamic_range_kth.hpp)

整数配列の点更新と区間 Kth をオンラインで処理します。
更新先の値やクエリを事前に列挙する必要はありません。
負数、重複、整数型の最小値・最大値も扱えます。

```cpp
#include "kyopro/dynamic_range_kth.hpp"

std::vector<long long> a = {5, 1, 4, 1, 3};
kyopro::dynamic_range_kth<> ds(a);
cout << ds.kth_smallest(0, 5, 2) << '\n';  // 3
ds.set(1, 1000000000000LL);                // 未登録の値も使える
cout << ds.kth_smallest(0, 5, 2) << '\n';  // 4
cout << ds.kth_largest(1, 4, 0) << '\n';   // 1000000000000
```

区間は半開区間 `[l, r)`、順位 `k` は **0-indexed** です。
配列の長さは固定です。値の更新には `set` を使います。

## コンストラクタ / build

```cpp
kyopro::dynamic_range_kth<T> ds;
kyopro::dynamic_range_kth<T> ds(const vector<T>& a);
void ds.build(const vector<T>& a);
```

既定の `T` は `long long` です。引数なしでは空の配列を作ります。
`build` は配列 `a` から再構築します。

**制約**

- `T` は `bool` 以外の整数型
- 配列長は `int` に収まる
- 内部ノード数は `int` に収まり、必要なメモリを確保できる

`B` を `T` のビット数、`n` を配列長として、構築時間は
$O(nB\log(n+1))$、空間は最悪 $O(nB\log(n+1))$ です。
Fenwick Tree の各要素に Binary Trie を持つため、静的な Wavelet Matrix より
メモリを多く使います。削除後の不要ノードは次の更新で再利用します。

## set / get / operator[]

```cpp
void ds.set(int i, T x);
T ds.get(int i) const;
T ds[i];
```

`set(i, x)` は `a[i]` を `x` に置き換えます。
`get(i)` と `operator[]` は現在の値を返します。

**制約**

- `0 <= i < n`

**計算量**

- `set`: $O(B\log(n+1))$
- `get`, `operator[]`: $O(1)$

## kth_smallest / kth_largest

```cpp
T ds.kth_smallest(int l, int r, int k) const;
T ds.kth_largest(int l, int r, int k) const;
```

区間内の小さい順・大きい順で `k` 番目の値を返します。
重複する値もそれぞれ 1 要素として数えます。

**制約**

- `0 <= l <= r <= n`
- `0 <= k < r - l`

**計算量**

- $O(B\log(n+1))$

## range_freq / count

```cpp
int ds.range_freq(int l, int r, T upper) const;
int ds.range_freq(int l, int r, T lower, T upper) const;
int ds.count(int l, int r, T x) const;
```

それぞれ、区間内で `v < upper`、`lower <= v < upper`、`v == x` を満たす
要素数を返します。空区間や `lower >= upper` の値域に対しては `0` を返します。

**制約**

- `0 <= l <= r <= n`

**計算量**

- $O(B\log(n+1))$

## size / empty

```cpp
int ds.size() const;
bool ds.empty() const;
```

配列長と、配列が空かを返します。計算量は $O(1)$ です。
