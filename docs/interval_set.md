# Interval Set

実装: [lib/kyopro/interval_set.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/interval_set.hpp)

整数上の半開区間を、互いに重ならず隣接もしない区間へ統合して管理します。
たとえば `[1, 4)` と `[4, 7)` を追加すると `[1, 7)` として保持します。

```cpp
#include "kyopro/interval_set.hpp"
```

## テンプレート引数

```cpp
template <class T>
class interval_set;
```

- `T`: 区間の端点に使う整数型

**制約**

- `T` は整数型

## コンストラクタ

```cpp
interval_set<T> intervals;
```

区間を持たない空の Interval Set を作ります。

**計算量**

- $O(1)$

## add

```cpp
void intervals.add(T left, T right);
```

半開区間 `[left, right)` を追加します。
既存区間と重なる場合、または隣接する場合は一つの区間へ統合します。
`left >= right` の場合は何もしません。

**引数**

- `T left`: 追加する区間の左端
- `T right`: 追加する区間の右端

**計算量**

統合される既存区間数を $k$、現在の区間数を $m$ として $O((k + 1)\log m)$ です。

## erase

```cpp
void intervals.erase(T left, T right);
```

半開区間 `[left, right)` に含まれる整数を削除します。
既存区間の中央を削除すると、区間が二つに分割されます。
`left >= right` の場合は何もしません。

**引数**

- `T left`: 削除する区間の左端
- `T right`: 削除する区間の右端

**計算量**

影響を受ける既存区間数を $k$、現在の区間数を $m$ として $O((k + 1)\log m)$ です。

## find

```cpp
optional<pair<T, T>> intervals.find(T x) const;
```

整数 `x` を含む区間を返します。
そのような区間がなければ `nullopt` を返します。

**計算量**

- $O(\log m)$

## contains

```cpp
bool intervals.contains(T x) const;
```

整数 `x` がいずれかの区間に含まれるか判定します。

**計算量**

- $O(\log m)$

```cpp
bool intervals.contains(T left, T right) const;
```

半開区間 `[left, right)` 全体が保持されているか判定します。
`left >= right` の場合は空区間として `true` を返します。

**計算量**

- $O(\log m)$

## mex

```cpp
T intervals.mex(T x = T(0)) const;
```

`x` 以上で、どの区間にも含まれない最小の整数を返します。

**制約**

- 答えが `T` で表現できる

**計算量**

- $O(\log m)$

## size / interval_count

```cpp
int intervals.size() const;
int intervals.interval_count() const;
```

現在保持している統合済み区間の個数を返します。
保持している整数の個数ではありません。
`interval_count()` は `size()` と同じです。

**計算量**

- $O(1)$

## empty

```cpp
bool intervals.empty() const;
```

区間を一つも保持していなければ `true` を返します。

**計算量**

- $O(1)$

## clear

```cpp
void intervals.clear();
```

すべての区間を削除します。

**計算量**

- $O(m)$

## intervals

```cpp
vector<pair<T, T>> intervals.intervals() const;
```

保持している半開区間を左端の昇順で返します。
返される区間同士は重ならず、隣接もしません。

**計算量**

- $O(m)$

## 使用例

```cpp
kyopro::interval_set<int> intervals;
intervals.add(1, 4);
intervals.add(6, 9);
intervals.add(4, 6);

// intervals.intervals() == {{1, 9}}
cout << intervals.contains(5) << '\n'; // 1
cout << intervals.mex(2) << '\n';      // 9

intervals.erase(3, 7);
// intervals.intervals() == {{1, 3}, {7, 9}}
```
