# Topological Sort

[実装の解説](explanations/topological_sort.md) / [共通API](graph_tree.md)

実装: [lib/kyopro/topological_sort.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/topological_sort.hpp)

```cpp
#include "kyopro/topological_sort.hpp"
```

有向グラフのトポロジカル順序を求めるクラスです。

## コンストラクタ

```cpp
topological_sort g(int n = 0);
topological_sort g(const vector<vector<int>>& graph);
topological_sort g(int n, const vector<pair<int, int>>& edges);
```

隣接リスト・辺リストを保存するだけで、探索は行いません。
`n >= 0`、端点は `[0,n)` とします。
頂点数のみなら $O(n)$、それ以外は $O(n+m)$ です。

## add_edge・size

```cpp
void g.add_edge(int from, int to);
int g.size() const;
```

有向辺を追加します。自己ループ・多重辺も扱います。
辺追加はならし $O(1)$、頂点数取得は $O(1)$ です。

## build

```cpp
vector<int> g.build() const;
```

入次数を毎回計算し直し、Kahn法で頂点の順序を返します。
DAGなら全頂点を1回ずつ含み、すべての辺の始点が終点より前になります。
閉路がある場合は空配列です。空グラフも空配列を返すため、両者の区別には頂点数を使います。
同時に処理可能な頂点が複数ある場合の順序は辞書順最小とは限りません。
計算後に辺を追加して再実行でき、以前の返り値は変更されません。

**計算量**: 時間 $O(n+m)$、追加空間 $O(n)$。

## 使用例

```cpp
kyopro::topological_sort g(3);
g.add_edge(0, 1);
g.add_edge(1, 2);
auto order = g.build(); // {0, 1, 2}
g.add_edge(2, 0);
auto failed = g.build(); // {}
```
