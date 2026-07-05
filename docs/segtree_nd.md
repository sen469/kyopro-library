# Segtree ND

実装: [lib/kyopro/segtree_nd.hpp](../lib/kyopro/segtree_nd.hpp)

`D` 次元配列に対する点更新・直方体領域取得ができるセグメント木です。
領域は各次元について半開区間 `[l[i], r[i])` で指定します。

```cpp
#include "kyopro/segtree_nd.hpp"
```

## segtree_nd

```cpp
template <class S, int D, S (*op)(S, S), S (*e)()>
class segtree_nd;
```

可換モノイド `(S, op, e)` を渡して使います。
`vector<S>`, `vector<vector<S>>`, `vector<vector<vector<S>>>` のように、次元数に対応するネストした `vector` から構築できます。

**制約**

- `1 <= D`
- `op` は結合律と交換法則を満たす
- `e()` は単位元を返す
- ネストした `vector` から構築する場合、入力配列は各次元で長方形

## コンストラクタ

```cpp
segtree_nd<S, D, op, e> seg;
segtree_nd<S, D, op, e> seg(const vector<int>& sizes);
segtree_nd<S, D, op, e> seg(const array<int, D>& sizes);
segtree_nd<S, D, op, e> seg(const vector<...>& v);
segtree_nd<S, D, op, e> seg(const vector<int>& sizes, const vector<S>& v);
segtree_nd<S, D, op, e> seg(const array<int, D>& sizes, const vector<S>& v);
```

- `segtree_nd()` は各次元の長さが 0 のセグ木を作ります。
- `segtree_nd(sizes)` は形状 `sizes` の全要素 `e()` のセグ木を作ります。
- `segtree_nd(v)` は `D` 次元ベクトル `v` からセグ木を作ります。
- `segtree_nd(sizes, v)` は行優先順に平坦化された配列 `v` からセグ木を作ります。

**計算量**

- `segtree_nd()`: $O(2^D)$
- `segtree_nd(sizes)`: $O(DN)$
- `segtree_nd(v)`: $O(DN)$
- `segtree_nd(sizes, v)`: $O(DN)$

ここで `N = \prod_i H_i`、`H_i` は `sizes[i]` 以上の最小の2冪の2倍です。

## build

```cpp
void seg.build(const vector<int>& sizes);
void seg.build(const vector<...>& v);
void seg.build(const vector<int>& sizes, const vector<S>& v);
```

セグ木を再構築します。

**計算量**

- $O(DN)$

## set

```cpp
void seg.set(const vector<int>& p, S value);
```

座標 `p` の値を `value` にします。
`seg.set({y, x}, value)` のようにも呼べます。

**制約**

- `p.size() == D`
- 全ての `i` に対して `0 <= p[i] < seg.size(i)`

**計算量**

- $O(\prod_i \log H_i)$

## get

```cpp
S seg.get(const vector<int>& p) const;
```

座標 `p` の値を返します。
`seg.get({y, x})` のようにも呼べます。

**制約**

- `p.size() == D`
- 全ての `i` に対して `0 <= p[i] < seg.size(i)`

**計算量**

- $O(D)$

## prod

```cpp
S seg.prod(const vector<int>& l, const vector<int>& r) const;
```

領域 `[l[0], r[0]) x ... x [l[D - 1], r[D - 1])` の値を返します。
`seg.prod({0, 1}, {2, 3})` のようにも呼べます。

**制約**

- `l.size() == r.size() == D`
- 全ての `i` に対して `0 <= l[i] <= r[i] <= seg.size(i)`

**計算量**

- $O(\prod_i \log H_i)$

## all_prod

```cpp
S seg.all_prod() const;
```

全体の値を返します。

**計算量**

- $O(1)$

## dimension / size / sizes / total_size / empty

```cpp
int seg.dimension() const;
int seg.size(int axis) const;
vector<int> seg.sizes() const;
size_t seg.total_size() const;
bool seg.empty() const;
```

次元数、各軸の長さ、形状、元配列の要素数、空かどうかを返します。

**計算量**

- `dimension`, `size`, `total_size`: $O(1)$
- `sizes`: $O(D)$
- `empty`: $O(1)$

## 使用例

```cpp
long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }

vector<vector<long long>> a = {
    {1, 2, 3},
    {4, 5, 6},
};

kyopro::segtree_nd<long long, 2, op, e> seg(a);

cout << seg.prod({0, 1}, {2, 3}) << '\n'; // 16

seg.set({1, 1}, 50);
cout << seg.get({1, 1}) << '\n';          // 50
cout << seg.prod({0, 1}, {2, 3}) << '\n'; // 61
```
