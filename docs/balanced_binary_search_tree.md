# Balanced Binary Search Tree

実装: [lib/kyopro/balanced_binary_search_tree.hpp](../lib/kyopro/balanced_binary_search_tree.hpp)

重複を許す平衡二分探索木です。
内部実装は treap で、挿入、削除、検索、lower_bound、upper_bound、k 番目の値の取得、値未満の要素数の取得をならし $O(\log n)$ で行えます。

```cpp
#include "kyopro/balanced_binary_search_tree.hpp"
```

## コンストラクタ

```cpp
kyopro::balanced_binary_search_tree<T> s;
kyopro::balanced_binary_search_tree<T, Compare> s;
kyopro::balanced_binary_search_tree<T> s(vector<T> v);
kyopro::balanced_binary_search_tree<T, Compare> s(vector<T> v, Compare comp);
```

- 型 `T`
- 比較関数 `Compare`

`Compare` は `std::less<T>` と同じ形式の狭義弱順序である必要があります。

**計算量**

- 空の木の構築: $O(1)$
- `vector` からの構築: ならし $O(n \log n)$

## size

```cpp
int s.size();
```

現在の要素数を返します。
重複する値は個数分数えます。

**計算量**

- $O(1)$

## empty

```cpp
bool s.empty();
```

空なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

## insert

```cpp
void s.insert(T x);
```

`x` を 1 個追加します。

**計算量**

- ならし $O(\log n)$

## erase

```cpp
bool s.erase(T x);
```

`x` が存在するなら 1 個削除し、`true` を返します。
存在しないなら何もせず `false` を返します。

**計算量**

- ならし $O(\log n)$

## erase_all

```cpp
int s.erase_all(T x);
```

`x` をすべて削除し、削除した個数を返します。
存在しないなら `0` を返します。

**計算量**

- ならし $O(\log n)$

## count

```cpp
int s.count(T x);
```

`x` の個数を返します。

**計算量**

- ならし $O(\log n)$

## contains

```cpp
bool s.contains(T x);
```

`x` が 1 個以上存在するなら `true` を返します。

**計算量**

- ならし $O(\log n)$

## kth

```cpp
const T& s.kth(int k);
```

昇順で `k` 番目の値を返します。
`k` は 0-indexed です。
重複する値は個数分並んでいるものとして扱います。

**制約**

- `0 <= k < s.size()`

**計算量**

- ならし $O(\log n)$

## order_of_key

```cpp
int s.order_of_key(T x);
```

`x` より小さい要素の個数を返します。
重複する値は個数分数えます。

**計算量**

- ならし $O(\log n)$

## iterator

```cpp
auto it = s.begin();
auto last = s.end();
```

昇順に要素を指すイテレータを返します。
重複する値は個数分並びます。
`it - s.begin()` で 0-indexed の位置を求められます。
`*it` は `const T&` を返します。

**注意**

- `insert`, `erase`, `erase_all`, `clear` の後は、既存のイテレータを使わないでください。
- `end()` は参照できません。

**計算量**

- `begin`, `end`: $O(1)$
- `*it`: ならし $O(\log n)$
- `++it`, `--it`, `it + k`, `it - other`: $O(1)$

## lower_bound

```cpp
auto it = s.lower_bound(T x);
```

`x` 以上で最小の値を指すイテレータを返します。
存在しない場合は `s.end()` を返します。
`lower_bound(x) - begin()` は `x` より小さい要素数、つまり `order_of_key(x)` と一致します。

値を `optional<T>` で受け取りたい場合は `lower_bound_value(x)` が使えます。

**計算量**

- ならし $O(\log n)$

## upper_bound

```cpp
auto it = s.upper_bound(T x);
```

`x` より大きい最小の値を指すイテレータを返します。
存在しない場合は `s.end()` を返します。

値を `optional<T>` で受け取りたい場合は `upper_bound_value(x)` が使えます。

**計算量**

- ならし $O(\log n)$

## min

```cpp
optional<T> s.min();
```

最小値を返します。
空なら `nullopt` を返します。

**計算量**

- ならし $O(\log n)$

## max

```cpp
optional<T> s.max();
```

最大値を返します。
空なら `nullopt` を返します。

**計算量**

- ならし $O(\log n)$

## to_vector

```cpp
vector<T> s.to_vector();
```

現在の要素を昇順に並べた `vector` を返します。
重複する値は個数分含まれます。

**計算量**

- $O(n)$

## clear

```cpp
void s.clear();
```

すべての要素を削除します。

**計算量**

- $O(n)$

[問題例 ABC458-D Chalkboard Median](https://atcoder.jp/contests/abc458/tasks/abc458_d)  
[Ordered Set](https://judge.yosupo.jp/problem/ordered_set)
## 使用例

```cpp
kyopro::balanced_binary_search_tree<long long> s;
s.insert(5);
s.insert(1);
s.insert(5);

assert(s.count(5) == 2);
assert(s.kth(1) == 5);
assert(s.order_of_key(5) == 1);
auto it = s.lower_bound(2);
assert(it != s.end());
assert(*it == 5);
assert(it - s.begin() == 1);
```
