# Tree Diameter

重み付き木の直径を求めます。
辺の重みは非負であることを想定しています。

```cpp
#include "kyopro/tree_diameter.hpp"
```

## tree_diameter_edge

```cpp
template <class T>
struct tree_diameter_edge {
    int to;
    T cost;
};
```

**メンバ変数**

- `int to`: 行き先の頂点
- `T cost`: 辺の重み

## tree_diameter_result

```cpp
template <class T>
struct tree_diameter_result {
    T diameter;
    int from;
    int to;
    vector<int> path;
};
```

**メンバ変数**

- `T diameter`: 直径の長さ
- `int from`: 直径の片方の端点
- `int to`: 直径のもう片方の端点
- `vector<int> path`: `from` から `to` への頂点列

頂点が 1 つの場合、`diameter = 0`, `from = 0`, `to = 0`, `path = {0}` です。
頂点が 0 個の場合、`diameter = 0`, `from = -1`, `to = -1`, `path = {}` です。

## tree_diameter

```cpp
template <class T>
tree_diameter_result<T> tree_diameter(
    const vector<vector<tree_diameter_edge<T>>>& graph
);
```

重み付き木 `graph` の直径を返します。

**引数**

- `const vector<vector<tree_diameter_edge<T>>>& graph`: 重み付き木の隣接リスト

**戻り値**

- 直径の長さ、両端、パスを持つ `tree_diameter_result<T>`

**制約**

- `graph` は無向木、または頂点数 0 の空グラフ
- `graph[v]` の各辺 `e` について `0 <= e.to < graph.size()`
- 辺は両方向に追加されている
- 辺の重みは非負
- `T` は `T(0)`, `operator+`, `operator<` が使える

**計算量**

- $O(n)$

## 辺リスト版

```cpp
template <class T>
tree_diameter_result<T> tree_diameter(
    int n,
    const vector<tuple<int, int, T>>& edges
);
```

辺リストから重み付き木の直径を返します。
辺は無向辺として扱います。

**引数**

- `int n`: 頂点数
- `const vector<tuple<int, int, T>>& edges`: `{u, v, cost}` 形式の辺リスト

**戻り値**

- 直径の長さ、両端、パスを持つ `tree_diameter_result<T>`

**制約**

- `0 <= n`
- `edges.size() == max(0, n - 1)`
- 各辺 `(u, v, cost)` について `0 <= u, v < n`
- 辺全体で木をなす
- `cost` は非負
- `T` は `T(0)`, `operator+`, `operator<` が使える

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
