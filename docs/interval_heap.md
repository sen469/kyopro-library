# Interval Heap

[実装の解説](explanations/interval_heap.md)

実装: [lib/kyopro/interval_heap.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/interval_heap.hpp)

両端優先度付きキューです。
最小値と最大値を $O(1)$ で取得し、挿入、最小値削除、最大値削除を $O(\log n)$ で行います。

```cpp
#include "kyopro/interval_heap.hpp"
```

## デバッグ出力

```cpp
#include "debug/interval_heap.hpp"

kyopro::interval_heap<int> heap = {5, 1, 3, 1};
dg(heap); // 値の表示: {1, 1, 3, 5}
```

`debug/interval_heap.hpp` はヒープ本体と `debug/debug.hpp` も include します。
既に `debug/debug.hpp` を include していても、その後に追加できます。
`operator<<` にも対応するため、`cout << heap` や `dg(vector_of_heaps)` も使えます。

ヒープのコピーから `min()` を取り出す順で表示します。
重複は保持し、`Compare = greater<T>` なら降順になります。
表示後も元のヒープの要素・比較関数は変わりません。
要素型はコピー可能で、`debug.hpp` で出力可能である必要があります。

現在の `multiset` 実装では、コピーと全要素の取り出しに合計 $O(n)$ 時間・
$O(n)$ 追加空間を使います（要素のコピー・出力が $O(1)$ の場合）。

## コンストラクタ

```cpp
interval_heap<T> heap;
interval_heap<T, Compare> heap;
interval_heap<T> heap(vector<T> v);
interval_heap<T, Compare> heap(vector<T> v, Compare comp);
interval_heap<T> heap({x0, x1, x2});
```

空の Interval Heap、または初期値を持つ Interval Heap を作ります。
`Compare` は `std::less<T>` と同じ形式の狭義弱順序です。
`std::less<T>` の場合、`min()` は最小値、`max()` は最大値を返します。

**計算量**

- 空の構築: $O(1)$
- 初期値からの構築: $O(n \log n)$

## size

```cpp
int heap.size() const;
```

現在の要素数を返します。

**戻り値**

- 要素数

**計算量**

- $O(1)$

## empty

```cpp
bool heap.empty() const;
```

空か判定します。

**戻り値**

- 空なら `true`
- そうでなければ `false`

**計算量**

- $O(1)$

## clear

```cpp
void heap.clear();
```

すべての要素を削除します。

**計算量**

- $O(n)$

## min / top_min

```cpp
const T& heap.min() const;
const T& heap.top_min() const;
```

最小側の値を返します。
`top_min()` は `min()` と同じです。

**戻り値**

- `Compare` で最も小さい要素への const 参照

**制約**

- `!heap.empty()`

**計算量**

- $O(1)$

## max / top_max

```cpp
const T& heap.max() const;
const T& heap.top_max() const;
```

最大側の値を返します。
`top_max()` は `max()` と同じです。

**戻り値**

- `Compare` で最も大きい要素への const 参照

**制約**

- `!heap.empty()`

**計算量**

- $O(1)$

## push

```cpp
void heap.push(const T& x);
void heap.push(T&& x);
```

値 `x` を追加します。

**引数**

- `const T& x`: 追加する値
- `T&& x`: 追加する値

**計算量**

- $O(\log n)$

## emplace

```cpp
template <class... Args>
void heap.emplace(Args&&... args);
```

`T(std::forward<Args>(args)...)` で値を構築して追加します。

**引数**

- `Args&&... args`: `T` のコンストラクタへ渡す引数

**計算量**

- $O(\log n)$

## pop_min

```cpp
void heap.pop_min();
```

最小側の値を 1 個削除します。

**制約**

- `!heap.empty()`

**計算量**

- $O(\log n)$

## pop_max

```cpp
void heap.pop_max();
```

最大側の値を 1 個削除します。

**制約**

- `!heap.empty()`

**計算量**

- $O(\log n)$

## 使用例

```cpp
kyopro::interval_heap<int> heap;
heap.push(5);
heap.push(1);
heap.push(4);

cout << heap.min() << '\n'; // 1
cout << heap.max() << '\n'; // 5

heap.pop_min();
cout << heap.min() << '\n'; // 4
```
## 問題
[Double-Ended Priority Queue](https://judge.yosupo.jp/problem/double_ended_priority_queue)
