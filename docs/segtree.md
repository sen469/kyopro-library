# Segtree

[実装の解説](explanations/segtree.md)

実装: [lib/kyopro/segtree.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/segtree.hpp)

点更新・区間積・境界探索を行うセグメント木です。
ACL の `segtree` と同じ公開メンバ関数を持つ独立実装に、`size()` と `to_vec()` を追加しています。
継承や ACL ヘッダへの依存はありません。C++17 以降で使えます。

```cpp
#include "kyopro/segtree.hpp"

long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }

std::vector<long long> a = {1, 2, 3, 4, 5};
kyopro::segtree<long long, op, e> seg(a);
seg.set(2, 10);
cout << seg.prod(1, 4) << '\n'; // 16
cout << seg.all_prod() << '\n'; // 22
cout << seg.size() << '\n';     // 5
auto values = seg.to_vec();     // {1, 2, 10, 4, 5}
// debug/debug.hpp を include していれば dg(seg.to_vec()) と書けます。
```

区間はすべて半開区間 `[l, r)`、添字は 0-indexed です。
テンプレート引数は `segtree<S, op, e>` です。
`op` は結合律を満たす演算、`e()` はその単位元である必要があります。
可換性は不要で、文字列の連結や行列積にも使えます。

## コンストラクタ

```cpp
kyopro::segtree<S, op, e> seg;
kyopro::segtree<S, op, e> seg(int n);
kyopro::segtree<S, op, e> seg(const vector<S>& a);
```

順に、空の列、長さ `n` の単位元列、配列 `a` から構築します。
列の長さは構築後に変わりません。

**制約**

- `0 <= n`、配列長が `int` に収まる
- 必要なメモリを確保できる
- `S` はコピー構築・コピー代入が可能
- `op` は `S(S, S)`、`e` は `S()` として呼び出せる

**計算量**

- 時間・空間ともに $O(n+1)$

## set / get

```cpp
void seg.set(int p, S x);
S seg.get(int p) const;
```

`set` は位置 `p` を `x` に置き換え、`get` は現在の値を返します。

**制約**

- `0 <= p < n`

**計算量**

- `set`: $O(\log(n+1))$
- `get`: $O(1)$

## prod / all_prod

```cpp
S seg.prod(int l, int r) const;
S seg.all_prod() const;
```

`prod` は `[l, r)`、`all_prod` は列全体の積を添字順で返します。
空区間・空の列の積は `e()` です。

**制約**

- `prod`: `0 <= l <= r <= n`

**計算量**

- `prod`: $O(\log(n+1))$（空の列では $O(1)$）
- `all_prod`: $O(1)$

## max_right / min_left

```cpp
int seg.max_right<f>(int l) const;
int seg.max_right(int l, F f) const;
int seg.min_left<f>(int r) const;
int seg.min_left(int r, F f) const;
```

関数ポインタをテンプレート引数で渡す形式と、ラムダなどを引数で渡す形式の両方に対応します。

- `max_right`: `f(prod(l, r))` が真のまま伸ばせる最も右の `r` を返す
- `min_left`: `f(prod(l, r))` が真のまま伸ばせる最も左の `l` を返す

範囲内で最後まで真なら、順に `n`・`0` を返します。
`max_right(n, f)` は `n`、`min_left(0, f)` は `0` です。

```cpp
// 上の例では値がすべて非負。先頭からの和が 12 以下の最大区間は [0, 2)。
int r = seg.max_right(0, [](long long sum) { return sum <= 12; }); // 2
```

**制約**

- `max_right`: `0 <= l <= n`
- `min_left`: `0 <= r <= n`
- `f(e()) == true`
- `f` は同じ値に対して同じ結果を返し、副作用を持たない
- 最大／最小の境界を得るには、区間を伸ばすと判定が `true` から `false` へ単調に変化する

**計算量**

- $O(\log(n+1))$（空の列では $O(1)$）

## size / to_vec

```cpp
int seg.size() const;
vector<S> seg.to_vec() const;
```

- `size()`: 論理的な列の長さ `n` を返す。木の内部容量ではない
- `to_vec()`: 現在の全要素を添字順にコピーして返す。内部の埋め草は含まない

`to_vec()` の返り値を変更しても、セグ木には影響しません。
どちらも `const` オブジェクトから呼び出せます。

**計算量**

- `size`: $O(1)$
- `to_vec`: 時間・追加空間ともに $O(n)$

各計算量は `op`・`e`・`f` と `S` のコピーが $O(1)$ としたものです。
添字や範囲などの事前条件違反は `assert` で検出します。
`using namespace atcoder;` と `using namespace kyopro;` を併用する場合、
`segtree` が曖昧になるため `kyopro::segtree` と明示してください。
