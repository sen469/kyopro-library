# Euler Tour

実装: [lib/kyopro/euler_tour.hpp](../lib/kyopro/euler_tour.hpp)

木または森の Euler Tour を作ります。
各頂点 `v` について、部分木の頂点集合は `order[in[v]], ..., order[out[v] - 1]` に対応します。
この区間を Fenwick Tree や Segment Tree に載せることで、部分木クエリを扱えます。

```cpp
#include "kyopro/euler_tour.hpp"
```

## euler_tour_edge

```cpp
struct euler_tour_edge {
    int to;
    int id;
};
```

**メンバ変数**

- `int to`: 行き先の頂点
- `int id`: 辺 ID

## コンストラクタ

```cpp
euler_tour();
```

頂点数 0 の空の Euler Tour 構造体を作ります。

**計算量**

- $O(1)$

```cpp
explicit euler_tour(int n);
```

頂点数 `n` の Euler Tour 構造体を作ります。

**引数**

- `int n`: 頂点数

**制約**

- `0 <= n`

**計算量**

- $O(n)$

```cpp
explicit euler_tour(const vector<vector<int>>& graph);
```

無向グラフの隣接リストから Euler Tour 構造体を作ります。
辺 ID は `-1` になります。

**引数**

- `const vector<vector<int>>& graph`: 無向木または森の隣接リスト

**制約**

- `graph[v]` の各 `to` について `0 <= to < graph.size()`
- 各辺は両方向に追加されている

**計算量**

- $O(n + m)$

```cpp
explicit euler_tour(const vector<vector<euler_tour_edge>>& graph);
```

辺 ID 付きの隣接リストから Euler Tour 構造体を作ります。

**引数**

- `const vector<vector<euler_tour_edge>>& graph`: 無向木または森の隣接リスト

**制約**

- `graph[v]` の各 `edge.to` について `0 <= edge.to < graph.size()`
- 各辺は両方向に追加されている

**計算量**

- $O(n + m)$

```cpp
euler_tour(int n, const vector<pair<int, int>>& edges);
```

辺リストから Euler Tour 構造体を作ります。
辺は無向辺として扱います。
辺 ID は `edges` の添字です。

**引数**

- `int n`: 頂点数
- `const vector<pair<int, int>>& edges`: `{u, v}` 形式の辺リスト

**制約**

- `0 <= n`
- 各辺 `(u, v)` について `0 <= u, v < n`

**計算量**

- $O(n + m)$

## add_edge

```cpp
void et.add_edge(int u, int v, int id = -1);
```

無向辺 `(u, v)` を追加します。
追加後にクエリを使う場合は、もう一度 `build` してください。

**引数**

- `int u`: 辺の片端
- `int v`: 辺のもう片端
- `int id`: 辺 ID

**制約**

- `0 <= u, v < et.size()`

**計算量**

- 償却 $O(1)$

## build

```cpp
void et.build(int root = 0);
```

`root` を含む連結成分を `root` 根の木として Euler Tour を作ります。
他の連結成分も、それぞれ未訪問の最小頂点を根として作ります。

**引数**

- `int root`: 優先して根にする頂点

**制約**

- `et.size() == 0`、または `0 <= root < et.size()`
- グラフは森

**計算量**

- $O(n + m)$

```cpp
void et.build(const vector<int>& roots);
```

`roots` の順に、未訪問の連結成分をその頂点根の木として Euler Tour を作ります。
残りの連結成分も、未訪問の最小頂点を根として作ります。

**引数**

- `const vector<int>& roots`: 優先して根にする頂点列

**制約**

- 各 `root` について `0 <= root < et.size()`
- グラフは森

**計算量**

- $O(n + m)$

## size

```cpp
int et.size() const;
```

頂点数を返します。

**計算量**

- $O(1)$

## empty

```cpp
bool et.empty() const;
```

頂点数が 0 なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

## in

```cpp
int et.in(int v) const;
```

頂点 `v` に最初に到達した時刻を返します。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## out

```cpp
int et.out(int v) const;
```

頂点 `v` の部分木を抜けた直後の時刻を返します。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## subtree

```cpp
pair<int, int> et.subtree(int v) const;
```

頂点 `v` の部分木に対応する半開区間 `[in[v], out[v])` を返します。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## get / operator[]

```cpp
int et.get(int k) const;
int et.operator[](int k) const;
```

Euler Tour の `k` 番目の頂点を返します。
`et.get(k) == et.order()[k]` です。

**制約**

- `build` 済み
- `0 <= k < et.size()`

**計算量**

- $O(1)$

## order

```cpp
const vector<int>& et.order() const;
```

Euler Tour の頂点列を返します。
長さは `et.size()` です。

**制約**

- `build` 済み

**計算量**

- $O(1)$

## parent

```cpp
int et.parent(int v) const;
```

根付き森での頂点 `v` の親を返します。
各根では `-1` を返します。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## parent_edge

```cpp
int et.parent_edge(int v) const;
```

根付き森での頂点 `v` と親を結ぶ辺 ID を返します。
各根では `-1` を返します。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## depth

```cpp
int et.depth(int v) const;
```

根付き森での頂点 `v` の深さを返します。
根の深さは 0 です。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## component

```cpp
int et.component(int v) const;
```

頂点 `v` が属する連結成分 ID を返します。
`build` で訪問された連結成分の順に、0 から番号が振られます。

**制約**

- `build` 済み
- `0 <= v < et.size()`

**計算量**

- $O(1)$

## same_tree

```cpp
bool et.same_tree(int u, int v) const;
```

頂点 `u`, `v` が同じ連結成分に属するなら `true`、そうでなければ `false` を返します。

**制約**

- `build` 済み
- `0 <= u, v < et.size()`

**計算量**

- $O(1)$

## is_ancestor

```cpp
bool et.is_ancestor(int ancestor, int v) const;
```

根付き森で `ancestor` が `v` の祖先なら `true`、そうでなければ `false` を返します。
`ancestor == v` のときも `true` です。

**制約**

- `build` 済み
- `0 <= ancestor, v < et.size()`

**計算量**

- $O(1)$

## 使用例

```cpp
int n = 5;
vector<pair<int, int>> edges = {
    {0, 1},
    {0, 2},
    {1, 3},
    {1, 4},
};

kyopro::euler_tour et(n, edges);
et.build(0);

auto [l, r] = et.subtree(1);
for (int i = l; i < r; i++) {
    cout << et[i] << ' ';
}
cout << '\n'; // 1 3 4
```
