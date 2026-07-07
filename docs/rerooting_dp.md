# Rerooting DP

実装: [lib/kyopro/rerooting_dp.hpp](../lib/kyopro/rerooting_dp.hpp)

木の全方位 DP を行います。
各頂点を根にしたときの DP 値をまとめて求めます。

```cpp
#include "kyopro/rerooting_dp.hpp"
```

## rerooting_edge

```cpp
struct rerooting_edge {
    int to;
    int id;
};
```

**メンバ変数**

- `int to`: 行き先の頂点
- `int id`: 辺 ID

## rerooting_dp

```cpp
template <class R, class M, auto op, auto e, auto f_ve, auto f_ev>
vector<R> rerooting_dp(
    const vector<vector<rerooting_edge>>& graph
);
```

隣接リストで表された木に対して全方位 DP を行います。

ACL の `segtree` / `lazy_segtree` と同じように、型と関数をテンプレート引数で渡して使います。
`R` は各頂点を根にした DP 値の型、`M` は隣接頂点から受け取る寄与の型です。

**引数**

- `const vector<vector<rerooting_edge>>& graph`: 木の隣接リスト

**テンプレート引数**

- `R`: 各頂点を根にした DP 値の型
- `M`: 隣接頂点から受け取る寄与の型
- `op`: 複数の寄与をまとめる二項演算 `M op(M a, M b)`
- `e`: `op` の単位元を返す関数 `M e()`
- `f_ve`: 頂点 DP 値を辺方向の寄与に変換する関数 `M f_ve(R vertex_dp, int edge_id)`
- `f_ev`: まとめた寄与を頂点 DP 値に変換する関数 `R f_ev(M merged, int vertex)`

**戻り値**

- `res[v]` は頂点 `v` を根にしたときの DP 値

**制約**

- `graph` は無向木、または頂点数 0 の空グラフ
- `graph[v]` の各辺 `edge` について `0 <= edge.to < graph.size()`
- 各無向辺は両方向に追加されている
- 両方向の辺は同じ `id` を持つ
- `op` は結合法則を満たす
- `e()` は `op` の単位元
- `M` はコピー可能
- `R` はデフォルト構築可能、コピー可能

**計算量**

- $O(n)$ 回の `op`, `f_ve`, `f_ev`

## 辺リスト版

```cpp
template <class R, class M, auto op, auto e, auto f_ve, auto f_ev>
vector<R> rerooting_dp(
    int n,
    const vector<pair<int, int>>& edges
);
```

辺リストから木を作って全方位 DP を行います。
辺 ID は `edges` の添字です。

**引数**

- `int n`: 頂点数
- `const vector<pair<int, int>>& edges`: `{u, v}` 形式の無向辺リスト

**テンプレート引数**

- `R`: 各頂点を根にした DP 値の型
- `M`: 隣接頂点から受け取る寄与の型
- `op`: 複数の寄与をまとめる二項演算 `M op(M a, M b)`
- `e`: `op` の単位元を返す関数 `M e()`
- `f_ve`: 頂点 DP 値を辺方向の寄与に変換する関数 `M f_ve(R vertex_dp, int edge_id)`
- `f_ev`: まとめた寄与を頂点 DP 値に変換する関数 `R f_ev(M merged, int vertex)`

**戻り値**

- `res[v]` は頂点 `v` を根にしたときの DP 値

**制約**

- `0 <= n`
- `edges.size() == max(0, n - 1)`
- 各辺 `(u, v)` について `0 <= u, v < n`
- 辺全体で木をなす
- `op` は結合法則を満たす
- `e()` は `op` の単位元
- `M` はコピー可能
- `R` はデフォルト構築可能、コピー可能

**計算量**

- $O(n)$ 回の `op`, `f_ve`, `f_ev`

## 使用例

各頂点から最も遠い頂点までの距離を求めます。

```cpp
long long op(long long a, long long b) {
    return max(a, b);
}

long long e() {
    return 0;
}

vector<long long> cost;

long long f_ve(long long vertex_dp, int edge_id) {
    return vertex_dp + cost[edge_id];
}

long long f_ev(long long merged, int vertex) {
    return merged;
}

int n = 4;
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {1, 3},
};
cost = {2, 3, 4};

auto ans = kyopro::rerooting_dp<long long, long long, op, e, f_ve, f_ev>(n, edges);

for (long long x : ans) {
    cout << x << ' ';
}
cout << '\n'; // 6 4 7 7
```
