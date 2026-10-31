# Tree Diameter

[実装の解説](explanations/tree_diameter.md)

実装: [lib/kyopro/tree_diameter.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/tree_diameter.hpp)

```cpp
#include "kyopro/tree_diameter.hpp"
```

非負辺重みの無向木について、直径・直径経路・中心を求めます。

## コンストラクタ

```cpp
tree_diameter<T> g(int n = 0);
tree_diameter<T> g(const vector<vector<tree_diameter_edge<T>>>& graph);
tree_diameter<T> g(int n, const vector<tuple<int, int, T>>& edges);
```

`tree_diameter_edge<T>` は `{int to; T cost;}` です。
辺リストは `{u,v,cost}` を1回ずつ、隣接リストは両方向に登録します。
`n >= 0`、端点は `[0,n)`、自己ループは不可、重みは非負です。
`T` は0からの構築、加算、減算、比較、コピーができ、経路長が収まる型を使います。

コンストラクタは頂点数のみなら $O(n)$、それ以外は $O(n+m)$ です。

## add_edge・size

```cpp
void g.add_edge(int u, int v, T cost = T(1));
int g.size() const;
```

無向辺を追加します。重みを省略すると1です。
辺追加はならし $O(1)$、頂点数取得は $O(1)$ です。

## build

```cpp
tree_diameter_result<T> g.build() const;
```

直径と中心を求めます。グラフは連結な無向木、または空グラフであることが必要です。
自己ループ・多重辺・閉路は扱いません。

**計算量**: 時間・追加空間ともに $O(n)$。

## tree_diameter_result

```cpp
template <class T>
struct tree_diameter_result {
    T diameter;
    int from;
    int to;
    vector<int> path;
    vector<int> center;
};
```

- `diameter`: 直径の長さ
- `from`, `to`: 直径の両端
- `path`: `from` から `to` までの頂点列
- `center`: 中心点 `{v}` または中心辺の両端 `{u, v}`

直径が複数ある場合はそのうち1つを返します。
空グラフでは `diameter = T(0)`、`from = to = -1`、`path`・`center` は空です。
1頂点では `path = center = {0}` です。

## center

`result.center` は重み付き直径の中点を表します。
中点が頂点上なら `{v}`、辺の内部ならその両端 `{u, v}` を格納します。
中心辺の端点は `path` 上の順です。

例えば重み `2, 8` の道 `0-1-2` では、中点は辺 `(1, 2)` の内部にあります。
0重みの辺により複数の頂点が中点に重なる場合は、そのうち1頂点を返します。
浮動小数点型では許容誤差を設けずに判定します。

## 使用例

```cpp
kyopro::tree_diameter<long long> g(4);
g.add_edge(0, 1, 2);
g.add_edge(1, 2, 3);
g.add_edge(1, 3, 4);
auto result = g.build(); // diameter = 7
auto center = result.center; // {3, 1} または {1, 3}
```
