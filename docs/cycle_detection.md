# Cycle Detection

グラフに含まれるサイクルを 1 つ見つけます。
サイクルがない場合は空の `vector<int>` を返します。

```cpp
#include "kyopro/cycle_detection.hpp"
```

## directed_cycle

```cpp
auto cycle = kyopro::directed_cycle(graph);
```

有向グラフのサイクルを検出します。
`graph[v]` に頂点 `v` から出る辺の行き先を入れた隣接リストを渡します。

返り値はサイクルに含まれる頂点列です。
`cycle[i] -> cycle[(i + 1) % cycle.size()]` がすべて辺になります。

**計算量**

- $O(n + m)$

## directed_cycle を辺リストから使う

```cpp
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {2, 0},
};

auto cycle = kyopro::directed_cycle(n, edges);
```

`edges` は `{from, to}` の形式です。

**計算量**

- $O(n + m)$

## undirected_cycle

```cpp
auto cycle = kyopro::undirected_cycle(graph);
```

無向グラフのサイクルを検出します。
`graph[v]` に頂点 `v` と隣接する頂点を入れた隣接リストを渡します。

返り値はサイクルに含まれる頂点列です。
`cycle[i]` と `cycle[(i + 1) % cycle.size()]` がすべて辺になります。

**計算量**

- $O(n + m)$

## undirected_cycle を辺リストから使う

```cpp
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {2, 0},
};

auto cycle = kyopro::undirected_cycle(n, edges);
```

`edges` は `{u, v}` の形式です。
辺リスト版は自己ループと多重辺も検出できます。

**計算量**

- $O(n + m)$

[Cycle Detection (Directed)](https://judge.yosupo.jp/problem/cycle_detection)  
[Cycle Detection (Undirected)](https://judge.yosupo.jp/problem/cycle_undetection)

## 使用例

```cpp
int n = 3;
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
    {2, 0},
};

auto cycle = kyopro::directed_cycle(n, edges);
if (cycle.empty()) {
    cout << "acyclic\n";
} else {
    for (int v : cycle) cout << v << ' ';
    cout << '\n';
}
```
