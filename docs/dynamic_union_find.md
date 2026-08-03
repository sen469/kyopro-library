# Dynamic Union Find

実装: [lib/kyopro/dynamic_union_find.hpp](../lib/kyopro/dynamic_union_find.hpp)

出てきた要素だけを管理する Union-Find です。
頂点番号を事前に座標圧縮せず、`long long` や `string` などのキーをそのまま使えます。

```cpp
#include "kyopro/dynamic_union_find.hpp"
```

## dynamic_union_find

```cpp
template <class Key = long long, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>>
class dynamic_union_find;
```

`Key` は各要素を表す型です。
`Hash` と `Pred` を指定すると、内部の `std::unordered_map` のハッシュ関数と等価比較を差し替えられます。

**制約**

- `Key` は `std::unordered_map` のキーとして使える

## コンストラクタ

```cpp
dynamic_union_find<Key> uf;
dynamic_union_find<Key> uf(reserve_size);
```

空の Union-Find を作ります。
`reserve_size` を指定した場合は、内部配列とハッシュテーブルの容量を予約します。
要素は作られません。

**計算量**

- $O(1)$

## reserve

```cpp
void uf.reserve(int n);
```

内部配列とハッシュテーブルの容量を予約します。

**制約**

- `0 <= n`

**計算量**

- $O(n)$

## add

```cpp
int uf.add(Key x);
```

要素 `x` を追加し、内部番号を返します。
すでに存在する場合は既存の内部番号を返します。

**計算量**

- ならし $O(1)$

## contains

```cpp
bool uf.contains(Key x) const;
```

要素 `x` が登録済みかを返します。

**計算量**

- ならし $O(1)$

## merge

```cpp
bool uf.merge(Key a, Key b);
```

要素 `a`, `b` を連結します。
未登録の要素は自動で追加されます。

**戻り値**

- 別成分だった場合は `true`
- すでに同じ成分だった場合は `false`

**計算量**

- ならし $O(\alpha(n))$

## same

```cpp
bool uf.same(Key a, Key b);
```

要素 `a`, `b` が同じ連結成分に属するかを返します。
未登録の要素は自動で追加されます。

**計算量**

- ならし $O(\alpha(n))$

## leader

```cpp
Key uf.leader(Key a);
```

要素 `a` の属する連結成分の代表元を返します。
未登録の要素は自動で追加されます。

**計算量**

- ならし $O(\alpha(n))$

## leader_id

```cpp
int uf.leader_id(Key a);
```

要素 `a` の属する連結成分の代表元の内部番号を返します。
未登録の要素は自動で追加されます。
`uf.key(uf.leader_id(a)) == uf.leader(a)` です。

**計算量**

- ならし $O(\alpha(n))$

## size

```cpp
int uf.size() const;
int uf.size(Key a);
```

`uf.size()` は登録済み要素数を返します。
`uf.size(a)` は要素 `a` の属する連結成分のサイズを返します。
未登録の要素は自動で追加されます。

**計算量**

- `uf.size()`: $O(1)$
- `uf.size(a)`: ならし $O(\alpha(n))$

## group_count

```cpp
int uf.group_count() const;
```

現在の連結成分数を返します。

**計算量**

- $O(1)$

## index / key

```cpp
int uf.index(Key x) const;
const Key& uf.key(int i) const;
```

`index(x)` は要素 `x` の内部番号を返します。
`key(i)` は内部番号 `i` に対応する要素を返します。

**制約**

- `x` は登録済み
- `0 <= i < uf.size()`

**計算量**

- `index(x)`: ならし $O(1)$
- `key(i)`: $O(1)$

## groups

```cpp
vector<vector<Key>> uf.groups();
```

連結成分ごとの要素リストを返します。
内側・外側の `vector` の順序は未定義です。

**計算量**

- $O(n \alpha(n))$

## 使用例

```cpp
kyopro::dynamic_union_find<long long> uf;

uf.merge(1000000000000LL, 5);
uf.merge(5, -3);

cout << uf.same(1000000000000LL, -3) << '\n'; // 1
cout << uf.size(5) << '\n';                   // 3
cout << uf.group_count() << '\n';             // 1
```
