# Topological Sort

DAG の頂点を、すべての有向辺 `u -> v` について `u` が `v` より前に来るように並べます。

```cpp
#include "kyopro/topological_sort.hpp"
```

## topological_sort

```cpp
auto order = kyopro::topological_sort(graph);
```

`graph[v]` に頂点 `v` から出る辺の行き先を入れた隣接リストを渡します。
閉路がある場合は空の `vector<int>` を返します。

**計算量**

- $O(n + m)$

## 辺リストから使う

```cpp
vector<pair<int, int>> edges = {
    {0, 2},
    {1, 2},
};

auto order = kyopro::topological_sort(n, edges);
```

`edges` は `{from, to}` の形式です。

**計算量**

- $O(n + m)$

## 使用例

```cpp
int n = 4;
vector<pair<int, int>> edges = {
    {0, 1},
    {0, 2},
    {1, 3},
    {2, 3},
};

auto order = kyopro::topological_sort(n, edges);
if (order.empty() && n > 0) {
    cout << "cycle\n";
} else {
    for (int v : order) cout << v << '\n';
}
```
