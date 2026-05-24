# Wavelet Matrix

静的配列に対して、区間 k 番目、区間内の値の個数、前駆・後継を求めます。
内部で座標圧縮するため、負数や `long long` も扱えます。

```cpp
#include "kyopro/wavelet_matrix.hpp"
```

## wavelet_matrix

```cpp
vector<long long> a = {5, 1, 4, 1, 3};
kyopro::wavelet_matrix<long long> wm(a);
```

`a` は構築後に変更できない静的配列として扱います。
区間はすべて半開区間 `[l, r)` です。

**計算量**

- 構築: $O(n \log \sigma)$
- クエリ: $O(\log \sigma)$

ここで `sigma` は異なる値の個数です。

## kth_smallest

```cpp
auto x = wm.kth_smallest(l, r, k);
```

区間 `[l, r)` の `k` 番目に小さい値を返します。
`k` は 0-indexed です。

```cpp
cout << wm.kth_smallest(1, 5, 2) << '\n'; // 3
```

## access

```cpp
auto x = wm.access(i);
auto y = wm[i];
```

元配列の `i` 番目の値を返します。

## kth_largest

```cpp
auto x = wm.kth_largest(l, r, k);
```

区間 `[l, r)` の `k` 番目に大きい値を返します。
`k` は 0-indexed です。

## range_freq

```cpp
int cnt1 = wm.range_freq(l, r, upper);
int cnt2 = wm.range_freq(l, r, lower, upper);
```

`range_freq(l, r, upper)` は、区間 `[l, r)` にある `upper` 未満の値の個数を返します。
`range_freq(l, r, lower, upper)` は、区間 `[l, r)` にある `[lower, upper)` の値の個数を返します。

## count

```cpp
int cnt = wm.count(l, r, x);
```

区間 `[l, r)` にある `x` の個数を返します。

## prev_value / next_value

```cpp
auto p = wm.prev_value(l, r, upper);
auto q = wm.next_value(l, r, lower);
```

`prev_value(l, r, upper)` は、区間 `[l, r)` にある `upper` 未満の最大値を返します。
`next_value(l, r, lower)` は、区間 `[l, r)` にある `lower` 以上の最小値を返します。

該当する値が存在しない場合は `assert` に失敗します。

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
