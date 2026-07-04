# Sparse Table

実装: [lib/kyopro/sparse_table.hpp](../lib/kyopro/sparse_table.hpp)

静的配列に対して、冪等な区間演算を $O(1)$ で求めるデータ構造です。
`min`, `max`, `gcd` などに使えます。

```cpp
#include "kyopro/sparse_table.hpp"
```

## sparse_table

```cpp
template <class S, S (*op)(S, S), S (*e)()>
class sparse_table;
```

ACL の `segtree` と同じように、`op`, `e` を渡して使います。
`prod(l, r)` は長さ `2^k` の2区間を重ねて計算するため、`op` は冪等である必要があります。

**制約**

- `op` は結合的
- `op(x, x) == x`
- `e()` は空区間の戻り値として使える

## コンストラクタ

```cpp
sparse_table();
explicit sparse_table(int n);
explicit sparse_table(const vector<S>& v);
```

- `sparse_table()` は空の Sparse Table を作ります。
- `sparse_table(n)` は長さ `n` の全要素 `e()` の Sparse Table を作ります。
- `sparse_table(v)` は配列 `v` から構築します。

**制約**

- `0 <= n`

**計算量**

- `sparse_table()`: $O(1)$
- `sparse_table(n)`: $O(n \log n)$
- `sparse_table(v)`: $O(n \log n)$

## build

```cpp
void st.build(const vector<S>& v);
```

配列 `v` から再構築します。

**計算量**

- $O(n \log n)$

## get / operator[]

```cpp
S st.get(int p) const;
S st.operator[](int p) const;
```

`p` 番目の値を返します。

**制約**

- `0 <= p < st.size()`

**計算量**

- $O(1)$

## prod

```cpp
S st.prod(int l, int r) const;
```

区間 `[l, r)` の値を `op` で畳み込んだ結果を返します。
`l == r` の場合は `e()` を返します。

**制約**

- `0 <= l <= r <= st.size()`

**計算量**

- $O(1)$

## all_prod

```cpp
S st.all_prod() const;
```

全体の値を返します。
空の場合は `e()` を返します。

**計算量**

- $O(1)$

## size / empty

```cpp
int st.size() const;
bool st.empty() const;
```

元配列の長さと、空かどうかを返します。

**計算量**

- $O(1)$

## 使用例

```cpp
int op(int a, int b) { return min(a, b); }
int e() { return (int)1e9; }

kyopro::sparse_table<int, op, e> st(vector<int>{5, 2, 7, 3});

cout << st.prod(1, 4) << '\n'; // 2
```
