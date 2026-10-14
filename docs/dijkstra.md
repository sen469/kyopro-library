# Dijkstra

[実装の解説](explanations/dijkstra.md) / [共通API](graph_tree.md)

実装: [lib/kyopro/dijkstra.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/dijkstra.hpp)

```cpp
#include "kyopro/dijkstra.hpp"
```

非負辺重みの最短経路を求めるクラスです。構築時には探索せず、`build(start)` のたびに計算します。

## コンストラクタ

```cpp
dijkstra<T> g(int n = 0, bool directed = true);
dijkstra<T> g(const vector<vector<dijkstra_edge<T>>>& graph, bool directed = true);
dijkstra<T> g(int n, const vector<tuple<int, int, T>>& edges, bool directed = true);
```

`dijkstra_edge<T>` は `{int to; T cost;}`、辺リストは `{from,to,cost}` 形式です。
`directed == false` の場合、辺リストと `add_edge` は両方向へ登録します。
隣接リストはそのままコピーするため、無向なら既に両方向の辺を含めてください。
`directed` はその後の `add_edge` の動作にも適用されます。

**制約**: `n >= 0`、端点は `[0,n)`、重みは非負の有限値。
`T` は0からの構築、加減算、比較、コピーが可能な数値型です。
コンストラクタは頂点数のみなら $O(n)$、グラフ・辺リスト版は $O(n+m)$ です。

## add_edge・size

```cpp
void g.add_edge(int from, int to, T cost);
int g.size() const;
```

辺の追加はならし $O(1)$、頂点数取得は $O(1)$ です。
自己ループ・多重辺を許します。計算後も追加でき、次の `build` に反映されます。

## build

```cpp
dijkstra_result<T> g.build(int start = 0,
                         T inf = numeric_limits<T>::max() / 4) const;
```

毎回、距離配列を初期化してDijkstra法を実行します。
`0 <= start < n`、`inf > 0` が必要です。
空グラフでは `start == 0` とし、距離・復元配列が空の結果を返します。
距離が `inf` 以上の頂点は到達不能扱いになるため、必要な距離より大きい `inf` を指定してください。
加算前に `cost < inf - distance` を確認し、非負整数での距離加算のオーバーフローを避けます。

**計算量**: 時間 $O(n + m\log(m+1))$、追加空間 $O(n+m)$。
重みの演算・比較を定数時間とします。返した結果は辺追加・再計算によって変わりません。

## dijkstra_result

```cpp
template <class T>
struct dijkstra_result {
    vector<T> dist;
    vector<int> prev;
    T inf;
    bool reachable(int v) const;
    vector<int> path(int goal) const;
};
```

`dist[v]` は距離、`prev[v]` は最短経路上の直前の頂点です。
始点・到達不能頂点の `prev` は-1、到達不能頂点の距離は `inf` です。
`reachable(v)` は $O(1)$。始点からgoalまでの頂点列を返す `path(goal)` は経路長に比例し、
到達不能なら空配列を返します。頂点引数は `[0,n)` としてください。

## 使用例

```cpp
kyopro::dijkstra<long long> g(3);
g.add_edge(0, 1, 5);
g.add_edge(1, 2, 2);
auto first = g.build(0); // dist = {0, 5, 7}
g.add_edge(0, 2, 1);
auto second = g.build(0); // dist = {0, 5, 1}
auto from_one = g.build(1); // dist[2] = 2
```
