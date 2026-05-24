# Dijkstra

非負重みのグラフで、始点から各頂点への最短距離を求めます。

```cpp
#include "kyopro/dijkstra.hpp"
```

## dijkstra_edge

```cpp
vector<vector<kyopro::dijkstra_edge<long long>>> graph(n);
graph[0].push_back({1, 3});
```

`to` と `cost` を持つ辺です。
辺の重みは `int`, `long long` などで使えます。

## dijkstra

```cpp
auto res = kyopro::dijkstra(graph, start);
```

`res.dist[v]` に `start` から `v` への最短距離が入ります。
到達不能な頂点には `res.inf` が入ります。

`res.reachable(v)` で到達可能か判定できます。
`res.path(v)` で `start` から `v` への最短経路の頂点列を復元できます。
到達不能な場合は空の `vector<int>` を返します。

**計算量**

- $O((n + m) \log n)$

## 辺リストから使う

```cpp
vector<tuple<int, int, long long>> edges = {
    {0, 1, 3},
    {1, 2, 4},
};

auto res = kyopro::dijkstra(n, edges, start);
```

`edges` は `{from, to, cost}` の形式です。
デフォルトでは有向グラフとして扱います。
無向グラフの場合は第 4 引数に `false` を渡します。

```cpp
auto res = kyopro::dijkstra(n, edges, start, false);
```

**計算量**

- $O((n + m) \log n)$

## 使用例

```cpp
int n = 4;
vector<tuple<int, int, long long>> edges = {
    {0, 1, 2},
    {0, 2, 5},
    {1, 2, 1},
    {2, 3, 4},
};

auto res = kyopro::dijkstra(n, edges, 0);
cout << res.dist[3] << '\n'; // 7

for (int v : res.path(3)) {
    cout << v << ' ';
}
cout << '\n'; // 0 1 2 3
```
