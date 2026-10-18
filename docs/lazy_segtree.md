# Lazy Segtree

[実装の解説](explanations/lazy_segtree.md)

実装: [lib/kyopro/lazy_segtree.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/lazy_segtree.hpp)

点更新・区間更新・区間積・境界探索を行う遅延セグメント木です。
ACL の `lazy_segtree` と同じ操作を持つ独立実装に、`size()` と `to_vec()` を追加しています。
継承や ACL ヘッダへの依存はありません。C++17 以降で使えます。

```cpp
#include "kyopro/lazy_segtree.hpp"

struct S { long long sum; int length; };
S op(S a, S b) { return {a.sum + b.sum, a.length + b.length}; }
S e() { return {0, 0}; }

using F = long long;
S mapping(F f, S x) { return {x.sum + f * x.length, x.length}; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

std::vector<S> a = {{1, 1}, {2, 1}, {3, 1}};
kyopro::lazy_segtree<S, op, e, F, mapping, composition, id> seg(a);
seg.apply(0, 2, 5);              // {6, 7, 3}
cout << seg.prod(0, 3).sum;      // 16
cout << seg.size();              // 3
auto values = seg.to_vec();      // {{6, 1}, {7, 1}, {3, 1}}
seg.apply(2, 10);                // 位置 2 に加算。全要素取得後も操作できる
```

区間は半開区間 `[l, r)`、添字は 0-indexed です。
集約値型 `S` と更新型 `F`、以下の操作を渡します。

| 引数 | 意味 |
| --- | --- |
| `op(S, S)` | 結合律を満たす区間集約。可換性は不要 |
| `e()` | `op` の単位元 |
| `mapping(F, S)` | 更新を集約値へ作用させる |
| `composition(F f, F g)` | `g` の後に `f` を行う更新を返す |
| `id()` | 何もしない更新 |

`mapping(id(), x) == x`、
`mapping(composition(f, g), x) == mapping(f, mapping(g, x))`、
`mapping(f, op(x, y)) == op(mapping(f, x), mapping(f, y))` が必要です。
`composition` は結合律と単位元 `id()` を持つ演算です。
区間長が必要な更新では、上の例のように `S` に長さを含めます。

## コンストラクタ

```cpp
kyopro::lazy_segtree<S, op, e, F, mapping, composition, id> seg;
kyopro::lazy_segtree<S, op, e, F, mapping, composition, id> seg(int n);
kyopro::lazy_segtree<S, op, e, F, mapping, composition, id> seg(const vector<S>& a);
```

順に、空の列、長さ `n` の単位元列、配列 `a` から構築します。
列の長さは構築後に変わりません。

**制約**

- `0 <= n`、配列長が `int` に収まる
- 必要なメモリを確保できる
- `S`・`F` はコピー構築・代入が可能

長さ指定のコンストラクタは各要素を `e()` にします。
上の例で `n` 個のゼロ値を用意する場合は、長さを 1 にした
`vector<S>(n, {0, 1})` を渡してください。

**計算量**

- 時間・空間ともに $O(n+1)$

## set / get

```cpp
void seg.set(int p, S x);
S seg.get(int p);
```

`set` は位置 `p` を `x` に置き換え、`get` は現在の値を返します。
必要な遅延伝播をしてから処理します。

**制約**

- `0 <= p < n`

**計算量**

- $O(\log(n+1))$

## apply

```cpp
void seg.apply(int p, F f);
void seg.apply(int l, int r, F f);
```

位置 `p` または区間 `[l, r)` の各要素に更新 `f` を作用させます。
空区間では何もしません。

**制約**

- 点更新: `0 <= p < n`
- 区間更新: `0 <= l <= r <= n`

**計算量**

- $O(\log(n+1))$（空の列では $O(1)$）

## prod / all_prod

```cpp
S seg.prod(int l, int r);
S seg.all_prod() const;
```

`prod` は `[l, r)`、`all_prod` は列全体の積を添字順に返します。
空区間・空の列の積は `e()` です。

**制約**

- `prod`: `0 <= l <= r <= n`

**計算量**

- `prod`: $O(\log(n+1))$（空の列では $O(1)$）
- `all_prod`: $O(1)$

## max_right / min_left

```cpp
int seg.max_right<g>(int l);
int seg.max_right(int l, G g);
int seg.min_left<g>(int r);
int seg.min_left(int r, G g);
```

関数ポインタをテンプレート引数で渡す形式と、ラムダなどを引数で渡す形式に対応します。

- `max_right`: `g(prod(l, r))` が真のまま伸ばせる最も右の `r` を返す
- `min_left`: `g(prod(l, r))` が真のまま伸ばせる最も左の `l` を返す

範囲内で最後まで真なら順に `n`・`0` を返します。
`max_right(n, g)` は `n`、`min_left(0, g)` は `0` です。

**制約**

- `max_right`: `0 <= l <= n`
- `min_left`: `0 <= r <= n`
- `g(e()) == true`
- `g` は同じ値に対して同じ結果を返し、副作用を持たない
- 最大／最小の境界を得るには、区間を伸ばすと判定が `true` から `false` へ単調に変化する

**計算量**

- $O(\log(n+1))$（空の列では $O(1)$）

## size / to_vec

```cpp
int seg.size() const;
vector<S> seg.to_vec();
```

`size` は列の長さを返します。
`to_vec` は遅延更新を葉まで伝播し、現在の全要素を添字順にコピーして返します。
内部の埋め草は含みません。返り値を変更しても元の木には影響しません。

`to_vec` は内部の遅延状態を整理しますが、論理的な値や集約結果は変えません。
取得後も更新・クエリを続けられます。
`get`・`prod`・境界探索と同様に、遅延伝播を伴うため非 `const` 関数です。
`size`・`all_prod` は `const` 対応です。

**計算量**

- `size`: $O(1)$
- `to_vec`: 時間・追加空間ともに $O(n)$

`to_vec` は `vector<S>` を返します。`dg(seg.to_vec())` を使うには、
`S` 自体が `debug.hpp` で出力可能である必要があります。
上の例のような独自構造体の場合は、`operator<<` などの出力処理を用意してください。

各計算量は `op`・`e`・`mapping`・`composition`・`id`・述語と、値のコピーが
$O(1)$ としたものです。添字や範囲などの事前条件違反は `assert` で検出します。
ACL と併用して名前が曖昧になる場合は `kyopro::lazy_segtree` と明示してください。
