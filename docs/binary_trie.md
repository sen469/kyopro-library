# Binary Trie

実装: [lib/kyopro/binary_trie.hpp](../lib/kyopro/binary_trie.hpp)

符号なし整数の多重集合を Binary Trie で管理します。
同じ値を複数回挿入できます。

```cpp
#include "kyopro/binary_trie.hpp"
```

## テンプレート引数

```cpp
template <class T = uint32_t, int BIT_SIZE = numeric_limits<T>::digits>
class binary_trie;
```

- `T`: 格納する符号なし整数型
- `BIT_SIZE`: 使用する下位ビット数

**制約**

- `T` は符号なし整数型
- `0 < BIT_SIZE <= numeric_limits<T>::digits`

## コンストラクタ

```cpp
binary_trie();
```

空の Binary Trie を作ります。

**計算量**

- $O(1)$

## insert

```cpp
void trie.insert(T x);
```

値 `x` を 1 個追加します。

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(BIT\_SIZE)$

## erase

```cpp
bool trie.erase(T x);
```

値 `x` を 1 個削除します。
`x` が存在しない場合は何も変更しません。

**戻り値**

- 削除できた場合は `true`
- `x` が存在しない場合は `false`

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(BIT\_SIZE)$

## count

```cpp
int trie.count(T x) const;
```

値 `x` が何個挿入されているか返します。

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(BIT\_SIZE)$

## contains

```cpp
bool trie.contains(T x) const;
```

値 `x` が 1 個以上挿入されているか判定します。

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(BIT\_SIZE)$

## size

```cpp
int trie.size() const;
```

挿入されている値の個数を返します。重複も数えます。

**計算量**

- $O(1)$

## empty

```cpp
bool trie.empty() const;
```

値が 1 個も挿入されていなければ `true` を返します。

**計算量**

- $O(1)$

## kth_element

```cpp
T trie.kth_element(int k) const;
```

小さい方から 0-indexed で `k` 番目の値を返します。
重複する値はそれぞれ 1 個として数えます。

**制約**

- `0 <= k < trie.size()`

**計算量**

- $O(BIT\_SIZE)$

## min_element

```cpp
T trie.min_element() const;
```

最小の値を返します。

**制約**

- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## max_element

```cpp
T trie.max_element() const;
```

最大の値を返します。

**制約**

- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## count_less

```cpp
int trie.count_less(T x) const;
```

`x` 未満の値の個数を返します。重複も数えます。

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(BIT\_SIZE)$

## min_xor_element

```cpp
T trie.min_xor_element(T x) const;
```

挿入されている値のうち、`x` との XOR が最小になる値を返します。

**制約**

- `0 <= x < 2^BIT_SIZE`
- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## max_xor_element

```cpp
T trie.max_xor_element(T x) const;
```

挿入されている値のうち、`x` との XOR が最大になる値を返します。

**制約**

- `0 <= x < 2^BIT_SIZE`
- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## min_xor

```cpp
T trie.min_xor(T x) const;
```

挿入されている値 `y` に対する `x ^ y` の最小値を返します。
対応する `y` が必要な場合は `min_xor_element(x)` を使います。

**制約**

- `0 <= x < 2^BIT_SIZE`
- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## max_xor

```cpp
T trie.max_xor(T x) const;
```

挿入されている値 `y` に対する `x ^ y` の最大値を返します。
対応する `y` が必要な場合は `max_xor_element(x)` を使います。

**制約**

- `0 <= x < 2^BIT_SIZE`
- `!trie.empty()`

**計算量**

- $O(BIT\_SIZE)$

## apply_xor

```cpp
void trie.apply_xor(T x);
```

挿入されているすべての値 `y` を `y ^ x` に置き換えます。
以後の挿入・削除・検索は、置き換え後の値に対して行われます。

**制約**

- `0 <= x < 2^BIT_SIZE`

**計算量**

- $O(1)$

## node_count

```cpp
int trie.node_count() const;
```

内部で確保されたノード数を返します。
削除してもノードを解放しないため、値を削除しても減りません。

**計算量**

- $O(1)$

## 使用例

```cpp
kyopro::binary_trie<unsigned, 30> trie;
trie.insert(3);
trie.insert(10);
trie.insert(10);

cout << trie.count(10) << '\n';         // 2
cout << trie.min_xor(6) << '\n';        // 5  (6 ^ 3)
cout << trie.max_xor_element(6) << '\n'; // 10
cout << trie.kth_element(1) << '\n';     // 10

trie.apply_xor(7); // {3, 10, 10} -> {4, 13, 13}
cout << trie.min_element() << '\n'; // 4
```
