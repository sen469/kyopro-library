# Dynamic Segtree

必要な頂点だけを作るセグメント木です。
座標が大きい場合や、座標圧縮せずにオンラインで点更新・区間取得をしたい場合に使えます。

```cpp
#include "kyopro/dynamic_segtree.hpp"
```

## dynamic_segtree

```cpp
template <class S, S (*op)(S, S), S (*e)()>
class dynamic_segtree;
```

ACL の `segtree` と同じように、モノイド `(S, op, e)` を渡して使います。
区間はすべて半開区間 `[l, r)` です。

## コンストラクタ

```cpp
dynamic_segtree();
explicit dynamic_segtree(long long n);
dynamic_segtree(long long l, long long r);
```

- `dynamic_segtree()` は空のセグ木を作ります。
- `dynamic_segtree(n)` は `[0, n)` を管理します。
- `dynamic_segtree(l, r)` は `[l, r)` を管理します。

**制約**

- `l <= r`
- 再帰の深さが大きくなりすぎない範囲で使う

**計算量**

- $O(1)$

## set

```cpp
void seg.set(long long p, S x);
```

`p` 番目の値を `x` にします。

**制約**

- `left_bound() <= p < right_bound()`

**計算量**

- $O(\log W)$

ここで `W = right_bound() - left_bound()` です。

## get / operator[]

```cpp
S seg.get(long long p) const;
S seg.operator[](long long p) const;
```

`p` 番目の値を返します。
一度も `set` していない位置は `e()` として扱います。

**制約**

- `left_bound() <= p < right_bound()`

**計算量**

- $O(\log W)$

## prod

```cpp
S seg.prod(long long l, long long r) const;
```

区間 `[l, r)` の値を `op` で畳み込んだ結果を返します。

**制約**

- `left_bound() <= l <= r <= right_bound()`

**計算量**

- $O(\log W + k)$

ここで `k` は訪問する存在済み頂点数です。

## all_prod

```cpp
S seg.all_prod() const;
```

管理区間全体の値を返します。

**計算量**

- $O(1)$

## max_right

```cpp
long long seg.max_right(long long l, F f) const;
long long seg.max_right<f>(long long l) const;
```

ACL の `segtree::max_right` と同じ形式です。
`f(prod(l, r))` が `true` になる最大の `r` を返します。

**制約**

- `left_bound() <= l <= right_bound()`
- `f(e()) == true`

**計算量**

- $O(\log W)$

## min_left

```cpp
long long seg.min_left(long long r, F f) const;
long long seg.min_left<f>(long long r) const;
```

ACL の `segtree::min_left` と同じ形式です。
`f(prod(l, r))` が `true` になる最小の `l` を返します。

**制約**

- `left_bound() <= r <= right_bound()`
- `f(e()) == true`

**計算量**

- $O(\log W)$

## left_bound / right_bound / size / empty

```cpp
long long seg.left_bound() const;
long long seg.right_bound() const;
long long seg.size() const;
bool seg.empty() const;
```

管理区間とサイズを返します。

**計算量**

- $O(1)$

## 問題
- [Point Set Range Composite (Large Array)](https://judge.yosupo.jp/problem/point_set_range_composite_large_array)


## 使用例

```cpp
long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }

kyopro::dynamic_segtree<long long, op, e> seg(0, 1000000000000000000LL);

seg.set(5, 10);
seg.set(1000000000000LL, 20);

cout << seg.prod(0, 10) << '\n';                 // 10
cout << seg.prod(0, 1000000000000000000LL) << '\n';  // 30
```
