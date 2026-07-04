# Dijkstra

実装: [lib/kyopro/dijkstra.hpp](../lib/kyopro/dijkstra.hpp)

非負重みのグラフで、始点から各頂点への最短距離を求めます。

```cpp
#include "kyopro/dijkstra.hpp"
```

## dijkstra_edge

```cpp
template <class T>
struct dijkstra_edge {
    int to;
    T cost;
};
```

**メンバ変数**

- `int to`: 行き先の頂点
- `T cost`: 辺の重み

## dijkstra_result

```cpp
template <class T>
struct dijkstra_result {
    vector<T> dist;
    vector<int> prev;
    T inf;
};
```

**メンバ変数**

- `vector<T> dist`: `dist[v]` は始点から頂点 `v` への最短距離。到達不能なら `inf`
- `vector<int> prev`: 最短経路木での直前の頂点。始点や到達不能な頂点は `-1`
- `T inf`: 到達不能を表す値

## reachable

```cpp
bool res.reachable(int v) const;
```

頂点 `v` が始点から到達可能か判定します。

**引数**

- `int v`: 調べる頂点

**戻り値**

- `dist[v] != inf` なら `true`
- そうでなければ `false`

**制約**

- `0 <= v < dist.size()`

**計算量**

- $O(1)$

## path

```cpp
vector<int> res.path(int goal) const;
```

始点から `goal` への最短経路の頂点列を復元します。

**引数**

- `int goal`: 終点

**戻り値**

- 到達可能な場合は、始点から `goal` までの頂点列
- 到達不能な場合は空の `vector<int>`

**制約**

- `0 <= goal < dist.size()`

**計算量**

- $O(L)$

`L` は返すパスの頂点数です。

## dijkstra

```cpp
template <class T>
dijkstra_result<T> dijkstra(
    const vector<vector<dijkstra_edge<T>>>& graph,
    int start,
    T inf = numeric_limits<T>::max() / 4
);
```

隣接リストで表されたグラフに対して Dijkstra 法を実行します。

**引数**

- `const vector<vector<dijkstra_edge<T>>>& graph`: グラフの隣接リスト
- `int start`: 始点
- `T inf`: 到達不能を表す値

**戻り値**

- 最短距離、経路復元用の直前頂点、`inf` を持つ `dijkstra_result<T>`

**制約**

- `0 <= start < graph.size()`
- 各辺 `e` について `0 <= e.to < graph.size()`
- 各辺の重みは非負
- `inf` は取りうる最短距離より大きい
- `T` は `T(0)`, `operator+`, `operator<`, `operator!=` が使える

**計算量**

- $O((n + m) \log n)$

## 辺リスト版

```cpp
template <class T>
dijkstra_result<T> dijkstra(
    int n,
    const vector<tuple<int, int, T>>& edges,
    int start,
    bool directed = true,
    T inf = numeric_limits<T>::max() / 4
);
```

辺リストからグラフを作って Dijkstra 法を実行します。

**引数**

- `int n`: 頂点数
- `const vector<tuple<int, int, T>>& edges`: `{from, to, cost}` 形式の辺リスト
- `int start`: 始点
- `bool directed`: `true` なら有向グラフ、`false` なら無向グラフ
- `T inf`: 到達不能を表す値

**戻り値**

- 最短距離、経路復元用の直前頂点、`inf` を持つ `dijkstra_result<T>`

**制約**

- `0 <= n`
- `0 <= start < n`
- 各辺 `(from, to, cost)` について `0 <= from, to < n`
- 各辺の重みは非負
- `inf` は取りうる最短距離より大きい
- `T` は `T(0)`, `operator+`, `operator<`, `operator!=` が使える

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
