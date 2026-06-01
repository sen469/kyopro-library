# Lowest Common Ancestor

木の 2 頂点の最小共通祖先をダブリングで求めます。

```cpp
#include "kyopro/lowest_common_ancestor.hpp"
```

## lowest_common_ancestor

```cpp
kyopro::lowest_common_ancestor lca(n);
lca.add_edge(0, 1);
lca.add_edge(1, 2);
lca.build(0);
```

`add_edge(u, v)` で無向辺を追加し、`build(root)` で根を指定して前計算します。
辺リストや隣接リストからも構築できます。

```cpp
vector<pair<int, int>> edges = {
    {0, 1},
    {1, 2},
};
kyopro::lowest_common_ancestor lca(n, edges);
lca.build(0);
```

```cpp
vector<vector<int>> graph(n);
kyopro::lowest_common_ancestor lca(graph);
lca.build(0);
```

## クエリ

```cpp
int w = lca.lca(u, v);
int d = lca.dist(u, v);
int p = lca.kth_ancestor(v, k);
```

- `lca(u, v)`: `u` と `v` の最小共通祖先を返します。
- `dist(u, v)`: `u` と `v` の辺数距離を返します。
- `kth_ancestor(v, k)`: `v` から親方向へ `k` 個上った頂点を返します。
- `depth(v)`: 根からの深さを返します。
- `parent(v)`: 親を返します。根の親は `-1` です。
- `same_tree(u, v)`: 同じ連結成分か判定します。

森にも対応しています。
別成分の `lca(u, v)` と `dist(u, v)` は `-1` を返します。
存在しない祖先への `kth_ancestor(v, k)` も `-1` を返します。

**計算量**

- 前計算: $O(n \log n)$
- クエリ: $O(\log n)$

## 使用例

```cpp
int n = 7;
kyopro::lowest_common_ancestor lca(n);
lca.add_edge(0, 1);
lca.add_edge(0, 2);
lca.add_edge(1, 3);
lca.add_edge(1, 4);
lca.add_edge(2, 5);
lca.add_edge(2, 6);
lca.build(0);

cout << lca.lca(3, 4) << '\n';       // 1
cout << lca.lca(3, 6) << '\n';       // 0
cout << lca.dist(3, 6) << '\n';      // 4
cout << lca.kth_ancestor(6, 2) << '\n'; // 0
```
