# Weighted Union Find

各頂点にポテンシャルを持つ Union-Find です。
制約 `potential[b] - potential[a] = w` を追加し、同じ連結成分内のポテンシャル差を取得できます。

```cpp
#include "kyopro/weighted_union_find.hpp"
```

## weighted_union_find

```cpp
template <class T>
struct weighted_union_find;
```

`T` はポテンシャル差の型です。

**制約**

- `T(0)`, `operator+`, `operator-`, 単項 `operator-`, `operator==` が使える
- 追加する制約に対する演算でオーバーフローしない

## コンストラクタ

```cpp
weighted_union_find<T> uf(int n);
```

`n` 頂点のデータ構造を作ります。最初、各頂点は異なる連結成分に属し、各頂点のポテンシャルは `0` です。

**制約**

- `0 <= n`

**計算量**

- $O(n)$

## merge

```cpp
bool uf.merge(int a, int b, T w);
```

制約 `potential[b] - potential[a] = w` を追加します。

`a`, `b` が別の連結成分に属する場合は連結し、`true` を返します。
すでに同じ連結成分に属する場合は、追加する制約が既存の制約と矛盾しなければ `true`、矛盾すれば `false` を返します。

**引数**

- `int a`: 頂点
- `int b`: 頂点
- `T w`: 追加するポテンシャル差

**戻り値**

- 制約を矛盾なく追加できるなら `true`
- 既存の制約と矛盾するなら `false`

**制約**

- `0 <= a < n`
- `0 <= b < n`

**計算量**

- ならし $O(\alpha(n))$

## same

```cpp
bool uf.same(int a, int b);
```

頂点 `a`, `b` が同じ連結成分に属するかを返します。

**制約**

- `0 <= a < n`
- `0 <= b < n`

**計算量**

- ならし $O(\alpha(n))$

## leader

```cpp
int uf.leader(int a);
```

頂点 `a` の属する連結成分の代表元を返します。

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## size

```cpp
int uf.size(int a);
```

頂点 `a` の属する連結成分のサイズを返します。

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## potential

```cpp
T uf.potential(int a);
```

頂点 `a` のポテンシャルを返します。
値は連結成分の代表元からの相対値です。同じ連結成分内の差分は一意ですが、絶対値は代表元の選び方に依存します。

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## diff

```cpp
T uf.diff(int a, int b);
```

`potential[b] - potential[a]` を返します。

**制約**

- `0 <= a < n`
- `0 <= b < n`
- `uf.same(a, b)` が `true`

**計算量**

- ならし $O(\alpha(n))$

## consistent

```cpp
bool uf.consistent(int a, int b, T w);
```

制約 `potential[b] - potential[a] = w` を追加しても矛盾しないか判定します。
連結関係とポテンシャル差は変更しません。

**制約**

- `0 <= a < n`
- `0 <= b < n`

**計算量**

- ならし $O(\alpha(n))$

## groups

```cpp
vector<vector<int>> uf.groups();
```

連結成分ごとの頂点リストを返します。
内側・外側の `vector` の順序は未定義です。

**計算量**

- $O(n \alpha(n))$

## 使用例

```cpp
kyopro::weighted_union_find<long long> uf(4);

uf.merge(0, 1, 3);  // potential[1] - potential[0] = 3
uf.merge(1, 2, 5);  // potential[2] - potential[1] = 5

cout << uf.diff(0, 2) << '\n'; // 8
cout << uf.same(0, 3) << '\n'; // 0

cout << uf.merge(0, 2, 8) << '\n'; // 1
cout << uf.merge(0, 2, 7) << '\n'; // 0
```

## 問題例
- [Unionfind with Potential](https://judge.yosupo.jp/problem/unionfind_with_potential)
