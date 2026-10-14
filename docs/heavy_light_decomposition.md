# Heavy Light Decomposition

[実装の解説](explanations/heavy_light_decomposition.md) / [共通API](graph_tree.md)

実装: [lib/kyopro/heavy_light_decomposition.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/heavy_light_decomposition.hpp)

木・森を重軽分解し、頂点を1次元配列に並べます。
パスを $O(\log n)$ 個の区間、部分木を1個の区間で表し、セグメント木や遅延セグメント木と組み合わせます。
値の保持・集約・更新はこのクラス自身では行いません。

```cpp
#include "kyopro/heavy_light_decomposition.hpp"
```

`kyopro::hld` は `kyopro::heavy_light_decomposition` の別名です。
頂点番号・配列添字は0始まりで、区間はすべて半開区間です。

## コンストラクタ

```cpp
heavy_light_decomposition(int n = 0);
heavy_light_decomposition(const vector<vector<int>>& graph);
heavy_light_decomposition(int n, const vector<pair<int, int>>& edges);
```

頂点数だけ、隣接リスト、または無向辺リストから作成します。まだ分解は行いません。

**制約**

- `0 <= n`、頂点数は `int` に収まる
- 無向木・森・空グラフであること。自己ループ・多重辺・閉路は不可
- 隣接リストでは各無向辺を両方向に1回ずつ、辺リストでは1回だけ渡す
- 辺の端点は `[0,n)`

**計算量**

- 時間・空間 $O(n)$（入力が森の場合）

## add_edge

```cpp
void h.add_edge(int u, int v);
```

無向辺を追加します。追加後も森である必要があります。
構築済みの情報は無効になるため、再び `build` を呼んでください。
両端は `[0,n)`、`u != v` が必要です。ならし $O(1)$ です。

## build

```cpp
void h.build(int root = 0);
void h.build(const vector<int>& roots);
```

指定された根を優先して分解します。
`roots` は先頭から処理し、既に訪問した成分の根は無視します。
未指定の成分は、未訪問頂点の番号が小さい順に根を選びます。
各成分内では、最大サイズの子部分木を重い辺で結びます。

**制約**

- 非空グラフで指定する根は `[0,n)`
- 空グラフでは `build()` または `build(vector<int>{})` を使う

**計算量**

- `build(root)`: 時間・追加空間 $O(n)$
- `build(roots)`: 時間 $O(n + |roots|)$、追加空間 $O(n)$
- 再帰は使わず、何度でも実行可能

根を変更すると配列添字や辺の格納先も変わります。組み合わせるセグメント木も作り直してください。
以下の問い合わせは、`size`・`empty` を除き、最後の辺追加後に `build` を実行していることが必要です。

## 頂点と配列の対応

```cpp
int h.size() const;
bool h.empty() const;
int h.in(int v) const;
int h.out(int v) const;
int h.get(int k) const;
int h.operator[](int k) const;
const vector<int>& h.order() const;
int h.parent(int v) const;
int h.depth(int v) const;
int h.component(int v) const;
int h.head(int v) const;
```

- `in(v)`: 頂点 `v` の配列添字
- `out(v)`: 部分木区間の右端
- `get(k)`・`operator[]`: 添字 `k` に対応する頂点。特に `get(in(v)) == v`
- `order()`: 添字から頂点への対応配列
- `parent(v)`: 親。成分の根なら `-1`
- `depth(v)`: 成分の根からの辺数
- `component(v)`: 成分番号。根の処理順に0から割り当てる
- `head(v)`: `v` の重いパスの最も浅い頂点

すべて $O(1)$ です。頂点 `v`、添字 `k` は `[0,n)` とします。
`order()` の参照は再構築後も以前の内容を保持するものではありません。

## 部分木・祖先判定

```cpp
pair<int, int> h.subtree(int v, bool edge = false) const;
bool h.same_tree(int u, int v) const;
bool h.is_ancestor(int ancestor, int v) const;
```

`subtree(v)` は `[in(v), out(v))` を返します。
`edge == true` なら `[in(v)+1, out(v))` です。
これは部分木内部の辺に対応し、`v` とその親を結ぶ境界の辺は含みません。葉では空区間です。

`same_tree` は同一成分かを返します。
`is_ancestor` は自身を含めた祖先判定で、異なる成分なら `false` です。
頂点はすべて `[0,n)`、計算量は $O(1)$ です。

## パス分解

```cpp
struct hld_path_segment {
    int l;
    int r;
    bool reversed;
};
vector<hld_path_segment> h.path(int u, int v, bool edge = false) const;
```

`u` から `v` へのパスを、進む順に並んだ区間列として返します。
各要素の区間は `[l,r)` で、空区間は含みません。

- `reversed == false`: 添字 `l, l+1, ..., r-1` の順
- `reversed == true`: 添字 `r-1, r-2, ..., l` の順
- 頂点モードでは両端とLCAをそれぞれ1回含む
- 辺モードではLCAの位置を除く。`u == v` なら空配列

**制約**

- `u,v` は `[0,n)` で、同じ成分に属する
- 辺モードでは、辺 `(parent(x),x)` の値を `in(x)` に置く

**計算量**

- 時間・返却配列の空間 $O(\log(n+1))$
- 区間ごとに $O(\log n)$ のセグメント木操作を行うと全体で $O(\log^2 n)$

和・最大値など可換な集約や一様な区間更新では `reversed` を無視できます。
文字列結合・行列積など非可換な集約では、各区間の逆順集約も取得できるようにしてください。
例えばセグメント木に正順値と逆順値のペアを持たせ、
結合時に正順は `op(a.forward,b.forward)`、逆順は `op(b.backward,a.backward)` とし、
`reversed` に応じて使い分けます。区間の処理順も維持してください。
辺モードでも `reversed` は配列の走査方向です。向きで値が異なる辺は別途両方向の値を保持します。

## LCA・距離・頂点取得

```cpp
int h.lca(int u, int v) const;
int h.dist(int u, int v) const;
int h.kth_ancestor(int v, long long k) const;
int h.kth_on_path(int u, int v, long long k) const;
```

- `lca`: 最小共通祖先。別成分なら `-1`
- `dist`: パスの辺数。別成分なら `-1`。重み付き距離ではない
- `kth_ancestor`: `k` 段上の祖先。`k == 0` なら自身、存在しなければ `-1`
- `kth_on_path`: `u` から `v` に向かって0始まりで `k` 番目の頂点。範囲外・別成分なら `-1`

頂点は `[0,n)`、`k >= 0` とします。各 $O(\log(n+1))$ 時間、追加空間 $O(1)$ です。

## 使用例

頂点の値の更新・パス和・部分木和を求めます。

```cpp
#include <iostream>
#include <vector>
#include <atcoder/segtree>
#include "kyopro/heavy_light_decomposition.hpp"

long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }

int main() {
    kyopro::hld h(5);
    h.add_edge(0, 1);
    h.add_edge(1, 2);
    h.add_edge(1, 3);
    h.add_edge(3, 4);
    h.build(0);

    std::vector<long long> a(5);
    for (int v = 0; v < 5; v++) a[h.in(v)] = v + 1;
    atcoder::segtree<long long, op, e> seg(a);
    auto path_sum = [&](int u, int v) {
        long long sum = 0;
        for (auto s : h.path(u, v)) sum += seg.prod(s.l, s.r);
        return sum;
    };
    std::cout << path_sum(2, 4) << '\n'; // 14
    auto [l, r] = h.subtree(3);
    std::cout << seg.prod(l, r) << '\n'; // 9
    seg.set(h.in(3), 10);
    std::cout << path_sum(2, 4) << '\n'; // 20
}
```

辺の値を置くときは、各辺の深い方の端点 `x` を選び `a[h.in(x)]` に格納します。
各成分の根の位置には単位元を置き、`path(u,v,true)` または `subtree(v,true)` を使います。
