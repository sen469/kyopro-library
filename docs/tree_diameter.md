# Tree Diameter

[実装の解説](explanations/tree_diameter.md)

実装: [lib/kyopro/tree_diameter.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/tree_diameter.hpp)

```cpp
#include "kyopro/tree_diameter.hpp"
```

非負辺重みの無向木の直径を求めるクラスです。

## コンストラクタ

```cpp
tree_diameter<T> g(int n = 0);
tree_diameter<T> g(const vector<vector<tree_diameter_edge<T>>>& graph);
tree_diameter<T> g(int n, const vector<tuple<int, int, T>>& edges);
```

`tree_diameter_edge<T>` は `{int to; T cost;}` です。
辺リストは `{u,v,cost}` を1回ずつ、隣接リストは両方向に登録します。
コンストラクタは辺を保存するだけです。
`n >= 0`、端点は `[0,n)`、自己ループは不可、重みは非負です。
`T` は0からの構築、加算、比較、コピーができ、経路長が収まる型を使います。

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

2回の探索で直径を求めます。呼ぶ時点で、連結な無向木または空グラフであることが必要です。
非空なら辺数はn-1、自己ループ・多重辺・閉路は不可です。
同じ木に対して何度でも再実行できます。

**計算量**: 時間・追加空間ともに $O(n)$。再帰は使いません。

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

`diameter` は直径の長さ、`from,to` は両端、`path` はfromからtoまでの頂点列です。
空なら `{T(0),-1,-1,{}}`、1頂点なら `{T(0),0,0,{0}}` を返します。
最長パスが複数ある場合はそのうち1つです。返り値は再計算によって変更されません。

## 使用例

```cpp
kyopro::tree_diameter<long long> g(4);
g.add_edge(0, 1, 2);
g.add_edge(1, 2, 3);
g.add_edge(1, 3, 4);
auto result = g.build(); // diameter = 7
```
