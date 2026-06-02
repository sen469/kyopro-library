# Tree Diameter

重み付き木の直径を求めます。
辺の重みは非負であることを想定しています。

```cpp
#include "kyopro/tree_diameter.hpp"
```

## tree_diameter_edge

```cpp
vector<vector<kyopro::tree_diameter_edge<long long>>> graph(n);
graph[0].push_back({1, 3});
graph[1].push_back({0, 3});
```

`to` と `cost` を持つ辺です。
辺の重みは `int`, `long long` などで使えます。

## tree_diameter

```cpp
auto res = kyopro::tree_diameter(graph);
```

`res.diameter` に直径の長さが入ります。
`res.from`, `res.to` は直径の両端の頂点です。
`res.path` は `from` から `to` への頂点列です。

頂点が 1 つの場合、直径は `0`、両端は `0`、パスは `{0}` です。
頂点が 0 個の場合、直径は `0`、両端は `-1`、パスは空です。

**計算量**

- $O(n)$

## 辺リストから使う

```cpp
vector<tuple<int, int, long long>> edges = {
    {0, 1, 3},
    {1, 2, 4},
};

auto res = kyopro::tree_diameter(n, edges);
```

`edges` は `{u, v, cost}` の形式です。
無向辺として扱います。

**計算量**

- $O(n)$

## 使用例

```cpp
int n = 5;
vector<tuple<int, int, long long>> edges = {
    {0, 1, 2},
    {1, 2, 4},
    {1, 3, 1},
    {3, 4, 7},
};

auto res = kyopro::tree_diameter(n, edges);
cout << res.diameter << '\n'; // 12

for (int v : res.path) {
    cout << v << ' ';
}
cout << '\n'; // 4 3 1 2
```
