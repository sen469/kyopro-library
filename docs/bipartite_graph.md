# Bipartite Graph

[実装の解説](explanations/bipartite_graph.md)

実装: [lib/kyopro/bipartite_graph.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/bipartite_graph.hpp)

無向グラフが二部グラフか判定し、二部グラフなら各頂点の 2 彩色を取得できます。
非連結グラフ、自己ループ、多重辺を扱えます。
コンストラクタでは探索しません。最後の辺追加後に明示的に `build()` を呼んでください。
[共通API](graph_tree.md)

```cpp
#include "kyopro/bipartite_graph.hpp"
```

## コンストラクタ

```cpp
bipartite_graph graph();
```

頂点数 0 の空のグラフを作ります。

**計算量**

- $O(1)$

```cpp
explicit bipartite_graph(int n);
```

頂点数 `n`、辺数 0 の無向グラフを作ります。
頂点番号は `0` から `n - 1` です。

**引数**

- `int n`: 頂点数

**制約**

- `0 <= n`

**計算量**

- $O(n)$

```cpp
explicit bipartite_graph(const vector<vector<int>>& graph);
```

無向グラフの隣接リストからグラフを作ります。

**引数**

- `const vector<vector<int>>& graph`: 無向グラフの隣接リスト

**制約**

- `graph[v]` の各 `to` について `0 <= to < graph.size()`
- 各辺は両方向に追加されている

**計算量**

- $O(n + m)$

```cpp
bipartite_graph(int n, const vector<pair<int, int>>& edges);
```

頂点数と辺リストから無向グラフを作ります。

**引数**

- `int n`: 頂点数
- `const vector<pair<int, int>>& edges`: `{u, v}` 形式の無向辺リスト

**制約**

- `0 <= n`
- 各辺 `(u, v)` について `0 <= u, v < n`

**計算量**

- $O(n + m)$

## add_edge

```cpp
void graph.add_edge(int u, int v);
```

無向辺 `(u, v)` を追加します。
自己ループと多重辺も追加できます。
追加すると、それ以前の判定結果と彩色結果は破棄されます。

**引数**

- `int u`: 辺の片端
- `int v`: 辺のもう片端

**制約**

- `0 <= u, v < graph.size()`

**計算量**

- 償却 $O(1)$

## size

```cpp
int graph.size() const;
```

頂点数を返します。

**計算量**

- $O(1)$

## empty

```cpp
bool graph.empty() const;
```

頂点数が 0 なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

## build

```cpp
bool graph.build();
```

毎回、彩色配列を初期化して二部グラフ判定を行い、判定結果を返します。
空グラフではtrueです。結果と彩色は内部にも保存され、次の辺追加で無効になります。
何度でも実行できます。時間・追加空間はそれぞれ $O(n+m)$・$O(n)$ です。

## is_bipartite

```cpp
bool graph.is_bipartite() const;
```

グラフが二部グラフなら `true`、そうでなければ `false` を返します。
空グラフも二部グラフとして扱います。

判定結果は次に `add_edge` が呼ばれるまで保存されます。

**計算量**

- $O(1)$。最後の辺追加後に `build` 済みであることが必要です。

## color

```cpp
int graph.color(int v) const;
```

頂点 `v` の色を `0` または `1` で返します。
すべての辺 `(u, v)` について `graph.color(u) != graph.color(v)` です。
非連結成分ごとに色を反転する可能性があるため、色自体は一意ではありません。

**制約**

- `0 <= v < graph.size()`
- グラフは二部グラフ

**計算量**

- $O(1)$。最後の辺追加後に `build` 済みであることが必要です。

## colors

```cpp
const vector<int>& graph.colors() const;
```

全頂点の色を格納した長さ `graph.size()` の配列を返します。
返り値の `v` 番目は `graph.color(v)` と等しいです。

**制約**

- グラフは二部グラフ

**計算量**

- $O(1)$。最後の辺追加後に `build` 済みであることが必要です。

## 使用例

```cpp
int n = 4;
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},
};

kyopro::bipartite_graph graph(n, edges);
if (graph.build()) {
    for (int v = 0; v < n; v++) {
        cout << graph.color(v) << '\n';
    }
}
```
