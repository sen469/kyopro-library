# Dynamic Wavelet Matrix

[実装の解説](explanations/dynamic_wavelet_matrix.md)

実装: [lib/kyopro/dynamic_wavelet_matrix.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/dynamic_wavelet_matrix.hpp)

整数列の挿入・削除・点更新と、区間 Kth・頻度・前駆・後継をオンラインで処理します。
更新先の値の事前登録や座標圧縮は不要です。
負数、重複、整数型の最小値・最大値を扱えます。

```cpp
#include "kyopro/dynamic_wavelet_matrix.hpp"

kyopro::dynamic_wavelet_matrix<> wm({5, 1, 4, 1, 3});
cout << wm.kth_smallest(0, 5, 2) << '\n';  // 3
wm.set(1, 100);                          // [5, 100, 4, 1, 3]
wm.insert(2, -10);                       // [5, 100, -10, 4, 1, 3]
wm.erase(0);                            // [100, -10, 4, 1, 3]
cout << wm.kth_smallest(0, 5, 0) << '\n'; // -10
cout << wm.kth_largest(0, 5, 0) << '\n';  // 100
```

区間は半開区間 `[l, r)`、順位 `k` は **0-indexed** です。
挿入・削除後は、それより後ろの添字が 1 つずれます。
`n` は現在の列の長さ、`B` は値型のビット数です。

## コンストラクタ / build

```cpp
kyopro::dynamic_wavelet_matrix<T> wm;
kyopro::dynamic_wavelet_matrix<T> wm(const vector<T>& a);
void wm.build(const vector<T>& a);
```

既定の `T` は `long long` です。引数なしでは空の列を作ります。
`build` は配列 `a` から再構築します。

**制約**

- `T` は `bool` 以外の整数型
- 列の長さと各段の内部ノード数が `int` に収まる
- 必要なメモリを確保できる

**計算量**

- 構築: $O(B(n+1))$
- 空間: 最悪 $O(B(n+1))$ ワード

各段のビット列を 64 ビット単位の葉に詰めて AVL 木で管理します。
木の添字・長さ・個数などの管理情報も必要なので、厳密な簡潔データ構造ではありません。
削除後の不要ノードは再利用します。確保済みの領域は過去の最大ノード数に依存します。

## insert / push_back / erase / set

```cpp
void wm.insert(int i, T x);
void wm.push_back(T x);
void wm.erase(int i);
void wm.set(int i, T x);
```

- `insert(i, x)`: 位置 `i` の直前に `x` を挿入する。`i == n` なら末尾追加
- `push_back(x)`: 末尾に追加する
- `erase(i)`: 位置 `i` の要素を削除する
- `set(i, x)`: 位置 `i` の値を置き換える

**制約**

- `insert`: `0 <= i <= n`
- 挿入・追加前に `n < INT_MAX`
- `erase`, `set`: `0 <= i < n`

**計算量**

- 償却 $O(B\log(n+2))$（内部ノード配列の拡張を含む）

## access / get / operator[]

```cpp
T wm.access(int i) const;
T wm.get(int i) const;
T wm[i];
```

現在の位置 `i` の値を返します。

**制約**

- `0 <= i < n`

**計算量**

- $O(B\log(n+2))$

## kth_smallest / kth_largest

```cpp
T wm.kth_smallest(int l, int r, int k) const;
T wm.kth_largest(int l, int r, int k) const;
```

区間内の小さい順・大きい順で `k` 番目の値を返します。
重複する値もそれぞれ 1 要素として数えます。

**制約**

- `0 <= l <= r <= n`
- `0 <= k < r - l`

**計算量**

- $O(B\log(n+2))$

## range_freq / count

```cpp
int wm.range_freq(int l, int r, T upper) const;
int wm.range_freq(int l, int r, T lower, T upper) const;
int wm.count(int l, int r, T x) const;
```

それぞれ `v < upper`、`lower <= v < upper`、`v == x` を満たす区間内の
要素数を返します。空区間や `lower >= upper` の値域に対しては `0` を返します。

**制約**

- `0 <= l <= r <= n`

**計算量**

- $O(B\log(n+2))$

## prev_value / next_value

```cpp
T wm.prev_value(int l, int r, T upper) const;
T wm.next_value(int l, int r, T lower) const;
```

`prev_value` は区間内で `upper` 未満の最大値、
`next_value` は区間内で `lower` 以上の最小値を返します。

**制約**

- `0 <= l <= r <= n`
- 該当する値が存在する（存在しない場合は `assert`）

**計算量**

- $O(B\log(n+2))$

## size / empty

```cpp
int wm.size() const;
bool wm.empty() const;
```

現在の列の長さと、列が空かを返します。計算量は $O(1)$ です。

## Dynamic Range Kth との比較

両方とも整数のオンライン点更新と区間 Kth を扱えます。
Dynamic Wavelet Matrix は列への挿入・削除も扱い、各段に列全体のビットを 1 つずつ保存します。
Dynamic Range Kth は Fenwick Tree の複数の Trie に値を登録するため、最悪空間が
$O(nB\log(n+1))$ ノードです。一方、`get` は $O(1)$ で使えます。
