# Persistent Segtree

各更新後の版を残せるセグメント木です。
`set` は元の木を変更せず、新しい版を返します。

```cpp
#include "kyopro/persistent_segtree.hpp"
```

## persistent_segtree

```cpp
template <class S, S (*op)(S, S), S (*e)()>
class persistent_segtree;
```

ACL の `segtree` と同じように、モノイド `(S, op, e)` を渡して使います。
区間はすべて半開区間 `[l, r)` です。

## コンストラクタ

```cpp
persistent_segtree();
explicit persistent_segtree(int n);
explicit persistent_segtree(const vector<S>& v);
```

- `persistent_segtree()` は空のセグ木を作ります。
- `persistent_segtree(n)` は長さ `n` の全要素 `e()` のセグ木を作ります。
- `persistent_segtree(v)` は配列 `v` からセグ木を作ります。

**制約**

- `0 <= n`

**計算量**

- `persistent_segtree()`: $O(1)$
- `persistent_segtree(n)`: $O(1)$
- `persistent_segtree(v)`: $O(n)$

## set

```cpp
persistent_segtree seg2 = seg.set(int p, S x);
```

`p` 番目の値を `x` にした新しい版を返します。
元の `seg` は変更されません。

**制約**

- `0 <= p < seg.size()`

**計算量**

- $O(\log n)$

## get / operator[]

```cpp
S seg.get(int p) const;
S seg.operator[](int p) const;
```

`p` 番目の値を返します。

**制約**

- `0 <= p < seg.size()`

**計算量**

- $O(\log n)$

## prod

```cpp
S seg.prod(int l, int r) const;
```

区間 `[l, r)` の値を `op` で畳み込んだ結果を返します。

**制約**

- `0 <= l <= r <= seg.size()`

**計算量**

- $O(\log n)$

## all_prod

```cpp
S seg.all_prod() const;
```

全体の値を返します。

**計算量**

- $O(1)$

## max_right

```cpp
int seg.max_right(int l, F f) const;
int seg.max_right<f>(int l) const;
```

ACL の `segtree::max_right` と同じ形式です。
`f(prod(l, r))` が `true` になる最大の `r` を返します。

**制約**

- `0 <= l <= seg.size()`
- `f(e()) == true`

**計算量**

- $O(\log n)$

## min_left

```cpp
int seg.min_left(int r, F f) const;
int seg.min_left<f>(int r) const;
```

ACL の `segtree::min_left` と同じ形式です。
`f(prod(l, r))` が `true` になる最小の `l` を返します。

**制約**

- `0 <= r <= seg.size()`
- `f(e()) == true`

**計算量**

- $O(\log n)$

## size / empty / node_count

```cpp
int seg.size() const;
bool seg.empty() const;
int seg.node_count() const;
```

`size()` は長さ、`empty()` は長さが `0` かを返します。
`node_count()` は共有しているノードプール内のノード数を返します。

**計算量**

- $O(1)$

## 使用例

```cpp
long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }

kyopro::persistent_segtree<long long, op, e> seg(vector<long long>{1, 2, 3});

auto seg2 = seg.set(1, 10);

cout << seg.prod(0, 3) << '\n';   // 6
cout << seg2.prod(0, 3) << '\n';  // 14
```
