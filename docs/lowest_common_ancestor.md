# Lowest Common Ancestor

木の 2 頂点の最小共通祖先をダブリングで求めます。
森にも対応しています。

```cpp
#include "kyopro/lowest_common_ancestor.hpp"
```

## コンストラクタ

```cpp
lowest_common_ancestor();
```

頂点数 0 の空の LCA 構造体を作ります。

**計算量**

- $O(1)$

```cpp
explicit lowest_common_ancestor(int n);
```

頂点数 `n` の LCA 構造体を作ります。

**引数**

- `int n`: 頂点数

**制約**

- `0 <= n`

**計算量**

- $O(n \log n)$

```cpp
explicit lowest_common_ancestor(const vector<vector<int>>& graph);
```

無向グラフの隣接リストから LCA 構造体を作ります。
前計算は `build` で行います。

**引数**

- `const vector<vector<int>>& graph`: 無向木または森の隣接リスト

**制約**

- `graph[v]` の各 `to` について `0 <= to < graph.size()`
- 各辺は両方向に追加されている

**計算量**

- $O(n \log n)$

```cpp
lowest_common_ancestor(int n, const vector<pair<int, int>>& edges);
```

辺リストから LCA 構造体を作ります。
辺は無向辺として扱います。
前計算は `build` で行います。

**引数**

- `int n`: 頂点数
- `const vector<pair<int, int>>& edges`: `{u, v}` 形式の辺リスト

**制約**

- `0 <= n`
- 各辺 `(u, v)` について `0 <= u, v < n`

**計算量**

- $O((n + m) \log n)$

## add_edge

```cpp
void lca.add_edge(int u, int v);
```

無向辺 `(u, v)` を追加します。
追加後にクエリを使う場合は、もう一度 `build` してください。

**引数**

- `int u`: 辺の片端
- `int v`: 辺のもう片端

**制約**

- `0 <= u, v < n`

**計算量**

- 償却 $O(1)$

## build

```cpp
void lca.build(int root = 0);
```

`root` を含む連結成分を `root` 根の木として前計算します。
他の連結成分も、それぞれ未訪問の最小頂点を根として前計算します。

**引数**

- `int root`: 優先して根にする頂点

**制約**

- `n > 0`
- `0 <= root < n`
- グラフ全体が森である

**計算量**

- $O(n \log n)$

```cpp
void lca.build(const vector<int>& roots);
```

`roots` に含まれる頂点を、それぞれの連結成分の根として優先して前計算します。
`roots` に含まれない連結成分も、未訪問の最小頂点を根として前計算します。

**引数**

- `const vector<int>& roots`: 優先して根にする頂点列

**制約**

- `roots` の各 `root` について `0 <= root < n`
- グラフ全体が森である

**計算量**

- $O(n \log n)$

## kth_ancestor

```cpp
int lca.kth_ancestor(int v, long long k) const;
```

頂点 `v` から親方向へ `k` 個上った頂点を返します。

**引数**

- `int v`: 開始頂点
- `long long k`: 上る辺数

**戻り値**

- 存在する場合は、`v` の `k` 個上の祖先
- 存在しない場合は `-1`

**制約**

- `0 <= v < n`
- `0 <= k`
- `build` 済み

**計算量**

- $O(\log n)$

## lca

```cpp
int lca.lca(int u, int v) const;
```

頂点 `u`, `v` の最小共通祖先を返します。

**引数**

- `int u`: 頂点
- `int v`: 頂点

**戻り値**

- `u`, `v` が同じ連結成分にある場合は最小共通祖先
- `u`, `v` が別の連結成分にある場合は `-1`
- `build` していない場合は `-1`

**制約**

- `0 <= u, v < n`

**計算量**

- $O(\log n)$

## dist

```cpp
int lca.dist(int u, int v) const;
```

頂点 `u`, `v` の辺数距離を返します。

**引数**

- `int u`: 頂点
- `int v`: 頂点

**戻り値**

- `u`, `v` が同じ連結成分にある場合は距離
- `u`, `v` が別の連結成分にある場合は `-1`
- `build` していない場合は `-1`

**制約**

- `0 <= u, v < n`

**計算量**

- $O(\log n)$

## depth

```cpp
int lca.depth(int v) const;
```

根から頂点 `v` までの深さを返します。

**引数**

- `int v`: 頂点

**戻り値**

- 頂点 `v` の深さ

**制約**

- `0 <= v < n`
- `build` 済み

**計算量**

- $O(1)$

## parent

```cpp
int lca.parent(int v) const;
```

頂点 `v` の親を返します。

**引数**

- `int v`: 頂点

**戻り値**

- 頂点 `v` の親
- 根の場合は `-1`

**制約**

- `0 <= v < n`
- `build` 済み

**計算量**

- $O(1)$

## same_tree

```cpp
bool lca.same_tree(int u, int v) const;
```

頂点 `u`, `v` が同じ連結成分にあるか判定します。

**引数**

- `int u`: 頂点
- `int v`: 頂点

**戻り値**

- `build` 済みで、`u`, `v` が同じ連結成分にある場合は `true`
- そうでなければ `false`

**制約**

- `0 <= u, v < n`

**計算量**

- $O(1)$

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

cout << lca.lca(3, 4) << '\n';          // 1
cout << lca.lca(3, 6) << '\n';          // 0
cout << lca.dist(3, 6) << '\n';         // 4
cout << lca.kth_ancestor(6, 2) << '\n'; // 0
```
