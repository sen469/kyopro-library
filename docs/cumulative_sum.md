# Cumulative Sum

実装: [lib/kyopro/cumulative_sum.hpp](../lib/kyopro/cumulative_sum.hpp)

1次元・2次元・3次元の累積和です。
半開区間の和を $O(1)$ で取得できます。

```cpp
#include "kyopro/cumulative_sum.hpp"
```

## cumulative_sum

```cpp
template <class T>
class cumulative_sum;
```

1次元配列の累積和です。

**制約**

- `T(0)`, `operator+`, `operator-` が使える

## コンストラクタ

```cpp
cumulative_sum<T> cs;
cumulative_sum<T> cs(int n);
cumulative_sum<T> cs(const vector<T>& v);
```

- `cumulative_sum()` は空配列の累積和を作ります。
- `cumulative_sum(n)` は長さ `n` の全要素 `T(0)` の累積和を作ります。
- `cumulative_sum(v)` は配列 `v` から累積和を作ります。

**計算量**

- `cumulative_sum()`: $O(1)$
- `cumulative_sum(n)`: $O(n)$
- `cumulative_sum(v)`: $O(n)$

## build

```cpp
void cs.build(const vector<T>& v);
```

配列 `v` から再構築します。

**計算量**

- $O(n)$

## prefix_sum

```cpp
T cs.prefix_sum(int r) const;
```

区間 `[0, r)` の和を返します。

**制約**

- `0 <= r <= cs.size()`

**計算量**

- $O(1)$

## sum

```cpp
T cs.sum(int l, int r) const;
```

区間 `[l, r)` の和を返します。

**制約**

- `0 <= l <= r <= cs.size()`

**計算量**

- $O(1)$

## all_sum

```cpp
T cs.all_sum() const;
```

全体の和を返します。

**計算量**

- $O(1)$

## size / empty

```cpp
int cs.size() const;
bool cs.empty() const;
```

元配列の長さと、空かどうかを返します。

**計算量**

- $O(1)$

## cumulative_sum_2d

```cpp
template <class T>
class cumulative_sum_2d;
```

2次元配列の累積和です。
長方形領域は半開区間 `[y1, y2) x [x1, x2)` で指定します。

**制約**

- `T(0)`, `operator+`, `operator-` が使える
- 入力配列は長方形

## コンストラクタ

```cpp
cumulative_sum_2d<T> cs;
cumulative_sum_2d<T> cs(int h, int w);
cumulative_sum_2d<T> cs(const vector<vector<T>>& v);
```

- `cumulative_sum_2d()` は空の2次元累積和を作ります。
- `cumulative_sum_2d(h, w)` は `h x w` の全要素 `T(0)` の累積和を作ります。
- `cumulative_sum_2d(v)` は2次元配列 `v` から累積和を作ります。

**計算量**

- `cumulative_sum_2d()`: $O(1)$
- `cumulative_sum_2d(h, w)`: $O(hw)$
- `cumulative_sum_2d(v)`: $O(hw)$

## build

```cpp
void cs.build(const vector<vector<T>>& v);
```

2次元配列 `v` から再構築します。

**計算量**

- $O(hw)$

## prefix_sum

```cpp
T cs.prefix_sum(int y, int x) const;
```

長方形 `[0, y) x [0, x)` の和を返します。

**制約**

- `0 <= y <= cs.height()`
- `0 <= x <= cs.width()`

**計算量**

- $O(1)$

## sum

```cpp
T cs.sum(int y1, int x1, int y2, int x2) const;
```

長方形 `[y1, y2) x [x1, x2)` の和を返します。

**制約**

- `0 <= y1 <= y2 <= cs.height()`
- `0 <= x1 <= x2 <= cs.width()`

**計算量**

- $O(1)$

## all_sum

```cpp
T cs.all_sum() const;
```

全体の和を返します。

**計算量**

- $O(1)$

## height / width / empty

```cpp
int cs.height() const;
int cs.width() const;
bool cs.empty() const;
```

元配列の高さ・幅と、空かどうかを返します。

**計算量**

- $O(1)$

## cumulative_sum_3d

```cpp
template <class T>
class cumulative_sum_3d;
```

3次元配列の累積和です。
直方体領域は半開区間 `[z1, z2) x [y1, y2) x [x1, x2)` で指定します。

**制約**

- `T(0)`, `operator+`, `operator-` が使える
- 入力配列は直方体

## コンストラクタ

```cpp
cumulative_sum_3d<T> cs;
cumulative_sum_3d<T> cs(int d, int h, int w);
cumulative_sum_3d<T> cs(const vector<vector<vector<T>>>& v);
```

- `cumulative_sum_3d()` は空の3次元累積和を作ります。
- `cumulative_sum_3d(d, h, w)` は `d x h x w` の全要素 `T(0)` の累積和を作ります。
- `cumulative_sum_3d(v)` は3次元配列 `v` から累積和を作ります。

**計算量**

- `cumulative_sum_3d()`: $O(1)$
- `cumulative_sum_3d(d, h, w)`: $O(dhw)$
- `cumulative_sum_3d(v)`: $O(dhw)$

## build

```cpp
void cs.build(const vector<vector<vector<T>>>& v);
```

3次元配列 `v` から再構築します。

**計算量**

- $O(dhw)$

## prefix_sum

```cpp
T cs.prefix_sum(int z, int y, int x) const;
```

直方体 `[0, z) x [0, y) x [0, x)` の和を返します。

**制約**

- `0 <= z <= cs.depth()`
- `0 <= y <= cs.height()`
- `0 <= x <= cs.width()`

**計算量**

- $O(1)$

## sum

```cpp
T cs.sum(int z1, int y1, int x1, int z2, int y2, int x2) const;
```

直方体 `[z1, z2) x [y1, y2) x [x1, x2)` の和を返します。

**制約**

- `0 <= z1 <= z2 <= cs.depth()`
- `0 <= y1 <= y2 <= cs.height()`
- `0 <= x1 <= x2 <= cs.width()`

**計算量**

- $O(1)$

## all_sum

```cpp
T cs.all_sum() const;
```

全体の和を返します。

**計算量**

- $O(1)$

## depth / height / width / empty

```cpp
int cs.depth() const;
int cs.height() const;
int cs.width() const;
bool cs.empty() const;
```

元配列の奥行き・高さ・幅と、空かどうかを返します。

**計算量**

- $O(1)$

## 使用例

```cpp
kyopro::cumulative_sum<long long> cs(vector<long long>{1, 2, 3, 4});
cout << cs.sum(1, 3) << '\n'; // 5

vector<vector<long long>> a = {
    {1, 2, 3},
    {4, 5, 6},
};
kyopro::cumulative_sum_2d<long long> cs2(a);
cout << cs2.sum(0, 1, 2, 3) << '\n'; // 16

vector<vector<vector<long long>>> b = {
    {
        {1, 2},
        {3, 4},
    },
    {
        {5, 6},
        {7, 8},
    },
};
kyopro::cumulative_sum_3d<long long> cs3(b);
cout << cs3.sum(0, 0, 0, 2, 2, 2) << '\n'; // 36
```
