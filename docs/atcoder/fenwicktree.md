# Fenwick Tree

実装: [lib/atcoder/fenwicktree.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/atcoder/fenwicktree.hpp)

長さ $n$ の配列に対し、

- 要素の $1$ 点変更
- 区間の要素の総和

を $O(\log n)$ で求めることが出来るデータ構造です。

## コンストラクタ

```cpp
fenwick_tree<T> fw(int n)
```

- 長さ $n$ の配列 $a_0, a_1, \cdots, a_{n-1}$ を作ります。初期値はすべて $0$ です。

**制約**

- $T$ は `int / uint / ll / ull / modint`
- $0 \leq n \leq 10^8$

**計算量**

- $O(n)$

## add

```cpp
void fw.add(int p, T x)
```

`a[p] += x` を行います。

**制約**

- $0 \leq p < n$

**計算量**

- $O(\log n)$

## sum

```cpp
T fw.sum(int l, int r)
```

`a[l] + a[l + 1] + ... + a[r - 1]` を返します。
`T` が整数型(`int / uint / ll / ull`)の場合、答えがオーバーフローしたならば $\bmod 2^{\mathrm{bit}}$ で等しい値を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

## 使用例

[公式ドキュメントの使用例](https://atcoder.github.io/ac-library/production/document_ja/fenwicktree.html)
