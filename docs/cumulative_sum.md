# Cumulative Sum

実装: [lib/kyopro/cumulative_sum.hpp](../lib/kyopro/cumulative_sum.hpp)

入力された `D` 次元データの累積和を計算します。
範囲は各次元について半開区間 `[l[i], r[i])` で指定します。

```cpp
#include "kyopro/cumulative_sum.hpp"
```

## cumulative_sum_nd

```cpp
template <class T, int D>
class cumulative_sum_nd;
```

`T` を要素に持つ `D` 次元ベクトルから累積和を構築します。
次元に応じて、`vector<T>`, `vector<vector<T>>`, `vector<vector<vector<T>>>` のようなネストした `vector` を渡せます。

**制約**

- `1 <= D`
- `T(0)`, `operator+`, `operator-` が使える
- 入力された `D` 次元ベクトルは長方形

## コンストラクタ

```cpp
cumulative_sum_nd<T, D> acc;
cumulative_sum_nd<T, D> acc(const array<int, D>& sizes);
cumulative_sum_nd<T, D> acc(const vector<...>& v);
cumulative_sum_nd<T, D> acc(const array<int, D>& sizes, const vector<T>& v);
```

- `cumulative_sum_nd()` は各次元の長さが 0 の累積和を作ります。
- `cumulative_sum_nd(sizes)` は形状 `sizes` の全要素 `T(0)` の累積和を作ります。
- `cumulative_sum_nd(v)` は `D` 次元ベクトル `v` から累積和を作ります。
- `cumulative_sum_nd(sizes, v)` は行優先順に平坦化された配列 `v` から累積和を作ります。

**計算量**

- $O(DN)$

ここで `N = \prod_i (size_i + 1)` です。

## build

```cpp
void acc.build(const array<int, D>& sizes);
void acc.build(const vector<...>& v);
void acc.build(const array<int, D>& sizes, const vector<T>& v);
```

累積和を再構築します。

- `build(sizes)` は形状 `sizes` の全要素 `T(0)` の累積和にします。
- `build(v)` は `D` 次元ベクトル `v` から再構築します。
- `build(sizes, v)` は行優先順に平坦化された配列 `v` から再構築します。

**計算量**

- $O(DN)$

## get

```cpp
T acc.get(const vector<int>& indexes) const;
```

指定されたインデックスに対応する元データの値を取得します。

**制約**

- `indexes.size() == D`
- 全ての `i` に対して `0 <= indexes[i] < acc.size(i)`

**計算量**

- $O(D)$

## set

```cpp
void acc.set(const vector<int>& indexes, const T& value);
```

指定されたインデックスに対応する元データの値を `value` に更新し、累積和を再構築します。

**制約**

- `indexes.size() == D`
- 全ての `i` に対して `0 <= indexes[i] < acc.size(i)`

**計算量**

- $O(DN)$

## prefix_sum

```cpp
T acc.prefix_sum(vector<long long> r) const;
T acc.prefix_sum(array<int, D> r) const;
```

範囲 `[0, r[0]) x ... x [0, r[D - 1])` の和を返します。
`acc.prefix_sum({2, 3})` のようにも呼べます。

**制約**

- `r` の次元は `D`
- 全ての `i` に対して `0 <= r[i] <= acc.size(i)`

**計算量**

- $O(D)$

## sum

```cpp
T acc.sum(vector<long long> l, vector<long long> r) const;
```

範囲 `[l[0], r[0]) x ... x [l[D - 1], r[D - 1])` の和を返します。

**制約**

- `l.size() == r.size() == D`
- 全ての `i` に対して `0 <= l[i] <= r[i] <= acc.size(i)`

**計算量**

- $O(D2^D)$

## dimension / size / sizes / total_size / empty / all_sum

```cpp
int acc.dimension() const;
int acc.size(int axis) const;
vector<int> acc.sizes() const;
size_t acc.total_size() const;
bool acc.empty() const;
T acc.all_sum() const;
```

- `dimension()` は次元数 `D` を返します。
- `size(axis)` は指定した軸の長さを返します。
- `sizes()` は各軸の長さを返します。
- `total_size()` は元データの要素数を返します。
- `empty()` は元データの要素数が 0 かどうかを返します。
- `all_sum()` は全体の和を返します。

**計算量**

- `dimension`, `size`, `total_size`, `empty`: $O(1)$
- `sizes`, `all_sum`: $O(D)$

## 使用例

```cpp
vector<long long> a = {1, 2, 3, 4};
kyopro::cumulative_sum_nd<long long, 1> acc1(a);
cout << acc1.get({2}) << '\n';        // 3
cout << acc1.prefix_sum({3}) << '\n'; // 6
cout << acc1.sum({1}, {3}) << '\n';   // 5

vector<vector<long long>> b = {
    {1, 2, 3},
    {4, 5, 6},
};
kyopro::cumulative_sum_nd<long long, 2> acc2(b);
cout << acc2.sum({0, 1}, {2, 3}) << '\n'; // 16

acc2.set({1, 1}, 50);
cout << acc2.get({1, 1}) << '\n';         // 50
cout << acc2.sum({0, 1}, {2, 3}) << '\n'; // 61

kyopro::cumulative_sum_nd<long long, 2> zero({2, 3});
cout << zero.all_sum() << '\n';           // 0
```
