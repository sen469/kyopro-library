# Wavelet Matrix

静的配列に対して、区間 k 番目、区間内の値の個数、前駆・後継を求めます。
内部で座標圧縮するため、負数や `long long` も扱えます。

```cpp
#include "kyopro/wavelet_matrix.hpp"
```

## wavelet_matrix

```cpp
template <class T>
class wavelet_matrix;
```

区間はすべて半開区間 `[l, r)` です。
`sigma` は異なる値の個数を表します。

## コンストラクタ

```cpp
wavelet_matrix();
```

空の Wavelet Matrix を作ります。

**計算量**

- $O(1)$

```cpp
explicit wavelet_matrix(const vector<T>& a);
```

配列 `a` から Wavelet Matrix を構築します。

**引数**

- `const vector<T>& a`: 元配列

**制約**

- `T` は `operator<`, `operator==` が使える
- `a.size()` が `int` に収まる

**計算量**

- $O(n \log \sigma)$

## build

```cpp
void wm.build(const vector<T>& a);
```

配列 `a` から Wavelet Matrix を再構築します。

**引数**

- `const vector<T>& a`: 元配列

**制約**

- `T` は `operator<`, `operator==` が使える
- `a.size()` が `int` に収まる

**計算量**

- $O(n \log \sigma)$

## size

```cpp
int wm.size() const;
```

元配列の長さを返します。

**戻り値**

- 元配列の長さ

**計算量**

- $O(1)$

## distinct_size

```cpp
int wm.distinct_size() const;
```

元配列に含まれる異なる値の個数を返します。

**戻り値**

- `sigma`

**計算量**

- $O(1)$

## empty

```cpp
bool wm.empty() const;
```

元配列が空か判定します。

**戻り値**

- `size() == 0` なら `true`
- そうでなければ `false`

**計算量**

- $O(1)$

## sorted_values

```cpp
const vector<T>& wm.sorted_values() const;
```

元配列に含まれる値を重複なし昇順にした配列を返します。

**戻り値**

- 重複なし昇順の値列への const 参照

**計算量**

- $O(1)$

## access / operator[]

```cpp
T wm.access(int i) const;
T wm.operator[](int i) const;
```

元配列の `i` 番目の値を返します。
`wm[i]` は `wm.access(i)` と同じです。

**引数**

- `int i`: 添字

**戻り値**

- 元配列の `i` 番目の値

**制約**

- `0 <= i < size()`

**計算量**

- $O(\log \sigma)$

## kth_smallest

```cpp
T wm.kth_smallest(int l, int r, int k) const;
```

区間 `[l, r)` の `k` 番目に小さい値を返します。
`k` は 0-indexed です。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `int k`: 何番目か

**戻り値**

- 区間 `[l, r)` の `k` 番目に小さい値

**制約**

- `0 <= l <= r <= size()`
- `0 <= k < r - l`

**計算量**

- $O(\log \sigma)$

## kth_largest

```cpp
T wm.kth_largest(int l, int r, int k) const;
```

区間 `[l, r)` の `k` 番目に大きい値を返します。
`k` は 0-indexed です。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `int k`: 何番目か

**戻り値**

- 区間 `[l, r)` の `k` 番目に大きい値

**制約**

- `0 <= l <= r <= size()`
- `0 <= k < r - l`

**計算量**

- $O(\log \sigma)$

## range_freq

```cpp
int wm.range_freq(int l, int r, const T& upper) const;
```

区間 `[l, r)` にある `upper` 未満の値の個数を返します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `const T& upper`: 上限値

**戻り値**

- `x < upper` を満たす区間内の要素数

**制約**

- `0 <= l <= r <= size()`
- `T` は `operator<` が使える

**計算量**

- $O(\log \sigma)$

```cpp
int wm.range_freq(int l, int r, const T& lower, const T& upper) const;
```

区間 `[l, r)` にある `[lower, upper)` の値の個数を返します。
`lower < upper` でない場合は `0` を返します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `const T& lower`: 下限値
- `const T& upper`: 上限値

**戻り値**

- `lower <= x < upper` を満たす区間内の要素数

**制約**

- `0 <= l <= r <= size()`
- `T` は `operator<` が使える

**計算量**

- $O(\log \sigma)$

## count

```cpp
int wm.count(int l, int r, const T& x) const;
```

区間 `[l, r)` にある `x` の個数を返します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `const T& x`: 数える値

**戻り値**

- 区間 `[l, r)` に含まれる `x` の個数

**制約**

- `0 <= l <= r <= size()`
- `T` は `operator<`, `operator==` が使える

**計算量**

- $O(\log \sigma)$

## prev_value

```cpp
T wm.prev_value(int l, int r, const T& upper) const;
```

区間 `[l, r)` にある `upper` 未満の最大値を返します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `const T& upper`: 上限値

**戻り値**

- 区間 `[l, r)` に含まれる `upper` 未満の最大値

**制約**

- `0 <= l <= r <= size()`
- `upper` 未満の値が区間内に 1 個以上存在する
- `T` は `operator<` が使える

**計算量**

- $O(\log \sigma)$

## next_value

```cpp
T wm.next_value(int l, int r, const T& lower) const;
```

区間 `[l, r)` にある `lower` 以上の最小値を返します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端
- `const T& lower`: 下限値

**戻り値**

- 区間 `[l, r)` に含まれる `lower` 以上の最小値

**制約**

- `0 <= l <= r <= size()`
- `lower` 以上の値が区間内に 1 個以上存在する
- `T` は `operator<` が使える

**計算量**

- $O(\log \sigma)$

## 問題

- [Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)

## 使用例

```cpp
vector<long long> a = {5, 1, 4, 1, 3};
kyopro::wavelet_matrix<long long> wm(a);

cout << wm.kth_smallest(1, 5, 2) << '\n'; // 3
cout << wm.range_freq(1, 5, 4LL) << '\n'; // 3
cout << wm.range_freq(1, 5, 2LL, 5LL) << '\n'; // 2
```
