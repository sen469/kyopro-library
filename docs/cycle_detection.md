# Cycle Detection

[実装の解説](explanations/cycle_detection.md)

実装: [lib/kyopro/cycle_detection.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/cycle_detection.hpp)

```cpp
#include "kyopro/cycle_detection.hpp"
```

有向・無向グラフから閉路を1つ求めるクラスです。探索は明示的なスタックで行います。

## コンストラクタ

```cpp
directed_cycle g(int n = 0);
directed_cycle g(const vector<vector<int>>& graph);
directed_cycle g(int n, const vector<pair<int, int>>& edges);

undirected_cycle g(int n = 0);
undirected_cycle g(const vector<vector<int>>& graph);
undirected_cycle g(int n, const vector<pair<int, int>>& edges);
```

`directed_cycle` は `cycle_detection<true>`、
`undirected_cycle` は `cycle_detection<false>` の別名です。
構築時には登録だけを行います。`n >= 0`、端点は `[0,n)` とします。
コンストラクタは頂点数のみなら $O(n)$、それ以外は $O(n+m)$ です。

無向の辺リストは各辺を1回、隣接リストは両方向に同じ本数ずつ渡してください。
隣接リストからは `u < v` の側を登録し、逆向きは自動生成します。
自己ループは現れるごとに登録します。自己ループを両方向相当の2回渡しても検出結果に影響しません。

## add_edge・size

```cpp
int g.add_edge(int u, int v);
int g.size() const;
```

辺を追加し、0始まりの追加順番号を返します。
有向版はuからv、無向版は両方向へ登録します。
自己ループ・多重辺を許します。追加はならし $O(1)$、頂点数取得は $O(1)$ です。

## build

```cpp
vector<int> g.build() const;
```

閉路の頂点列を返します。末尾と先頭の間にも辺があり、先頭頂点は末尾に重複させません。
閉路がなければ空配列です。自己ループは1頂点、多重辺による無向閉路は2頂点で返します。
無向版は隣接リスト・辺リスト・逐次追加のいずれも多重辺に対応します。
探索順に応じた1つの閉路を返します。列の開始位置は保証しません。

何度でも再計算でき、計算後に辺を追加しても以前の返り値は変わりません。
**計算量**: 時間 $O(n+m)$、追加空間 $O(n)$。再帰は使用しません。

## 使用例

```cpp
kyopro::directed_cycle g(3);
g.add_edge(0, 1);
g.add_edge(1, 2);
auto before = g.build(); // {}
g.add_edge(2, 0);
auto after = g.build(); // {0, 1, 2}
```

無向版も同様です。
