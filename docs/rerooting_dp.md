# Rerooting DP

[実装の解説](explanations/rerooting_dp.md)

実装: [lib/kyopro/rerooting_dp.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/rerooting_dp.hpp)

木の全方位 DP を行うクラスです。
`add_edge` で木を作り、`build(root)` で部分木 DP、`reroot()` で各頂点を根にした答えを求めます。

```cpp
#include "kyopro/rerooting_dp.hpp"
```

## コンストラクタ

```cpp
kyopro::rerooting_dp<R, M, op, e, f_ve, f_ev> g(int n = 0);
```

頂点数 `n`、辺数0のグラフを作ります。
ACL のように型と関数をテンプレート引数で渡します。型の順序は従来どおり `R, M` です。

- `R`: 頂点を根にした DP 値の型
- `M`: 隣接頂点から受け取る寄与の型
- `M op(M a, M b)`: 寄与を結合する演算
- `M e()`: `op` の単位元
- `M f_ve(R vertex_dp, int edge_id)`: 隣接頂点の DP 値に辺を付加して寄与へ変換
- `R f_ev(M merged, int vertex)`: 寄与の集約に頂点を付加して DP 値へ変換

**制約**

- `0 <= n`
- `op` は結合則を満たし、`e()` は単位元
- `R` はデフォルト構築可能、コピー構築・コピー代入可能
- `M` はコピー構築・コピー代入可能
- コールバックは同じ引数・同じ外部データに対して同じ値を返す

隣接辺は `add_edge` の追加順に結合します。可換性は要求しません。
以下の計算量は、コールバックと状態の構築・コピーが定数時間の場合です。

**計算量**

- 時間・空間 $O(n)$

## add_edge

```cpp
int g.add_edge(int u, int v);
int g.add_edge(int u, int v, int id);
int g.add_edge(int u, int v, int idx, int xdi);
```

無向辺を1本追加し、0始まりの追加順番号を返します。

- 2引数版: 両方向の辺IDを追加順番号にする
- 3引数版: 両方向の辺IDを `id` にする
- 4引数版: `u -> v` の辺IDを `idx`、`v -> u` の辺IDを `xdi` にする

返す追加順番号と、指定した辺IDは別物です。
辺IDは `f_ve` にそのまま渡され、重複しても構いません。
自動IDと明示IDを混在させる場合は、意図しないIDの重複に注意してください。

**辺IDの向き**

頂点 `u` が隣接頂点 `v` の情報を受け取るときは、
`f_ve(v側のDP値, uからvへの辺ID)` を使います。
したがって、`add_edge(u, v, idx, xdi)` では次の対応です。

- `u` の計算に `v` 側を取り込む: `idx`
- `v` の計算に `u` 側を取り込む: `xdi`

情報の伝播方向そのものとは逆なので注意してください。

**制約**

- `0 <= u, v < n`、`u != v`
- 非空の場合、合計でちょうど `n - 1` 本追加して連結な木にする
- 自己ループ・多重辺・閉路は不可
- 辺の追加は最初の `build` より前に行う
- 指定したIDをコールバックが扱えること

**計算量**

- ならし $O(1)$

## build

```cpp
vector<R> g.build(int root = 0);
```

`root` を根とした部分木 DP を計算し、内部に保存するとともに配列を返します。
返り値 `sub[v]` は、`v` 自身とその子孫だけを含む部分木の DP 値です。
全頂点を含む `root` の答えは `sub[root]` です。

各頂点では、親を除く隣接頂点について追加順に
`op(acc, f_ve(sub[to], edge_id))` を計算し、最後に `f_ev(acc, v)` を適用します。
葉の値は `f_ev(e(), v)` です。

**制約**

- 非空の場合、辺がちょうど `n - 1` 本で、全体が連結な木
- 非空の場合、`0 <= root < n`
- `n == 0` の場合は `root == 0` とし、空配列を返す

**再実行**

何度でも呼べます。根を変える場合も、外部の辺重み・頂点重み・法などを変更した場合も、
`build` を再実行すると部分木 DP を最初から計算し直します。
返された配列を書き換えても内部の DP は変わりません。

**計算量**

- 1回につき時間・追加空間 $O(n)$
- 再帰は使用しません

## reroot

```cpp
vector<R> g.reroot() const;
```

最後の `build` で保存した部分木 DP を使い、全方位 DP を行います。
返り値 `ans[v]` は木全体を頂点 `v` を根として見たときの DP 値です。
空グラフでは空配列を返します。

**制約**

- 先に `build` を呼んでいること
- 最後の `build` 以降、コールバックが参照する外部データを変更していないこと

何度でも呼べます。結果はキャッシュせず、その都度親側の寄与を計算します。
正しく定義されたコールバックなら、答えは `build` に指定した根に依存しません。

**計算量**

- 1回につき時間・追加空間 $O(n)$
- 再帰は使用しません

## 使用例
- [ABC220-F Destance Sum 2](https://atcoder.jp/contests/abc220/submissions/79829723)
- [EDPC-V Subtree](https://atcoder.jp/contests/dp/submissions/79829617)

各頂点から最も遠い頂点までの距離を求めます。辺長は非負とします。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include "kyopro/rerooting_dp.hpp"

std::vector<long long> cost;
long long op(long long a, long long b) { return std::max(a, b); }
long long e() { return 0; }
long long f_ve(long long vertex_dp, int edge_id) {
    return vertex_dp + cost[edge_id];
}
long long f_ev(long long merged, int) { return merged; }

int main() {
    kyopro::rerooting_dp<long long, long long, op, e, f_ve, f_ev> g(4);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    cost = {2, 3, 4};

    auto sub = g.build(0); // {6, 4, 0, 0}
    auto ans = g.reroot(); // {6, 4, 7, 7}
    for (long long x : ans) std::cout << x << ' ';
    std::cout << '\n';

    cost = {1, 1, 1};
    g.build(2);
    ans = g.reroot(); // {2, 1, 2, 2}
}
```
