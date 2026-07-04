# Persistent Union Find

実装: [lib/kyopro/persistent_union_find.hpp](../lib/kyopro/persistent_union_find.hpp)

各併合後の版を残せる Union-Find です。
`merge` は元の版を変更せず、新しい版を返します。

```cpp
#include "kyopro/persistent_union_find.hpp"
```

## persistent_union_find

```cpp
class persistent_union_find;
```

頂点は `0` から `n - 1` までの整数です。
内部では永続配列で `parent_or_size` を持ち、経路圧縮はせず union by size で高さを抑えます。

## コンストラクタ

```cpp
persistent_union_find();
explicit persistent_union_find(int n);
```

- `persistent_union_find()` は空の Union-Find を作ります。
- `persistent_union_find(n)` は `n` 頂点の Union-Find を作ります。

**制約**

- `0 <= n`

**計算量**

- $O(1)$

## merge

```cpp
pair<persistent_union_find, bool> uf.merge(int a, int b) const;
```

頂点 `a`, `b` を併合した新しい版と、実際に併合したかを返します。
元の `uf` は変更されません。

**戻り値**

- `first`: 併合後の版
- `second`: 別成分だった場合は `true`、すでに同じ成分だった場合は `false`

**制約**

- `0 <= a < uf.size()`
- `0 <= b < uf.size()`

**計算量**

- $O(\log^2 n)$

## unite

```cpp
persistent_union_find uf2 = uf.unite(int a, int b) const;
```

`merge(a, b).first` を返すショートカットです。

**計算量**

- $O(\log^2 n)$

## same

```cpp
bool uf.same(int a, int b) const;
```

頂点 `a`, `b` が同じ連結成分に属するかを返します。

**制約**

- `0 <= a < uf.size()`
- `0 <= b < uf.size()`

**計算量**

- $O(\log^2 n)$

## leader

```cpp
int uf.leader(int a) const;
```

頂点 `a` の属する連結成分の代表元を返します。

**制約**

- `0 <= a < uf.size()`

**計算量**

- $O(\log^2 n)$

## component_size / size

```cpp
int uf.component_size(int a) const;
int uf.size(int a) const;
```

頂点 `a` の属する連結成分のサイズを返します。
`size(a)` は `component_size(a)` と同じです。

**制約**

- `0 <= a < uf.size()`

**計算量**

- $O(\log^2 n)$

## size / empty / group_count / node_count

```cpp
int uf.size() const;
bool uf.empty() const;
int uf.group_count() const;
int uf.node_count() const;
```

- `size()` は頂点数を返します。
- `empty()` は頂点数が `0` かを返します。
- `group_count()` は現在の連結成分数を返します。
- `node_count()` は共有している永続配列のノード数を返します。

**計算量**

- $O(1)$

## groups

```cpp
vector<vector<int>> uf.groups() const;
```

連結成分ごとの頂点リストを返します。
内側・外側の `vector` の順序は未定義です。

**計算量**

- $O(n \log^2 n)$

## 使用例

```cpp
kyopro::persistent_union_find uf(4);

auto [uf1, ok1] = uf.merge(0, 1);
auto [uf2, ok2] = uf1.merge(1, 2);

cout << uf.same(0, 2) << '\n';   // 0
cout << uf1.same(0, 2) << '\n';  // 0
cout << uf2.same(0, 2) << '\n';  // 1
cout << uf2.component_size(0) << '\n';  // 3
```
