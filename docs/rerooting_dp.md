# Rerooting DP

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
template <class M, class Op, class FVE, class FEV>
vector<R> rerooting_dp(
    const vector<vector<rerooting_edge>>& graph,
    M e,
    Op op,
    FVE f_ve,
    FEV f_ev
);
```

隣接リストで表された木に対して全方位 DP を行います。

`M` は隣接頂点から受け取る寄与の型、`R` は各頂点を根にした DP 値の型です。
`R` は `f_ev(M, int)` の戻り値の型です。

**引数**

- `const vector<vector<rerooting_edge>>& graph`: 木の隣接リスト
- `M e`: `op` の単位元
- `Op op`: 複数の寄与をまとめる二項演算 `M op(M a, M b)`
- `FVE f_ve`: 頂点 DP 値を辺方向の寄与に変換する関数 `M f_ve(R vertex_dp, int edge_id)`
- `FEV f_ev`: まとめた寄与を頂点 DP 値に変換する関数 `R f_ev(M merged, int vertex)`

**戻り値**

- `res[v]` は頂点 `v` を根にしたときの DP 値

**制約**

- `graph` は無向木、または頂点数 0 の空グラフ
- `graph[v]` の各辺 `edge` について `0 <= edge.to < graph.size()`
- 各無向辺は両方向に追加されている
- 両方向の辺は同じ `id` を持つ
- `op` は結合法則を満たす
- `e` は `op` の単位元
- `M` はコピー可能
- `R` はデフォルト構築可能、コピー可能

**計算量**

- $O(n)$ 回の `op`, `f_ve`, `f_ev`

## 辺リスト版

```cpp
template <class M, class Op, class FVE, class FEV>
vector<R> rerooting_dp(
    int n,
    const vector<pair<int, int>>& edges,
    M e,
    Op op,
    FVE f_ve,
    FEV f_ev
);
```

辺リストから木を作って全方位 DP を行います。
辺 ID は `edges` の添字です。

**引数**

- `int n`: 頂点数
- `const vector<pair<int, int>>& edges`: `{u, v}` 形式の無向辺リスト
- `M e`: `op` の単位元
- `Op op`: 複数の寄与をまとめる二項演算
- `FVE f_ve`: 頂点 DP 値を辺方向の寄与に変換する関数
- `FEV f_ev`: まとめた寄与を頂点 DP 値に変換する関数

**戻り値**

- `res[v]` は頂点 `v` を根にしたときの DP 値

**制約**

- `0 <= n`
- `edges.size() == max(0, n - 1)`
- 各辺 `(u, v)` について `0 <= u, v < n`
- 辺全体で木をなす
- `op` は結合法則を満たす
- `e` は `op` の単位元
- `M` はコピー可能
- `R` はデフォルト構築可能、コピー可能

**計算量**

- $O(n)$ 回の `op`, `f_ve`, `f_ev`

## 使用例

各頂点から最も遠い頂点までの距離を求めます。

```cpp
int n = 4;
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {1, 3},
};
vector<long long> cost = {2, 3, 4};

auto ans = kyopro::rerooting_dp(
    n,
    edges,
    0LL,
    [](long long a, long long b) {
        return max(a, b);
    },
    [&](long long vertex_dp, int edge_id) {
        return vertex_dp + cost[edge_id];
    },
    [](long long merged, int vertex) {
        return merged;
    }
);

for (long long x : ans) {
    cout << x << ' ';
}
cout << '\n'; // 6 4 7 7
```
