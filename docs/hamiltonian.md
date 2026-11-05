# Hamiltonian Path / Cycle

[実装の解説](explanations/hamiltonian.md)

実装: [lib/kyopro/hamiltonian.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/hamiltonian.hpp)

全頂点をちょうど1回通るハミルトン路、または始点へ戻るハミルトン閉路を1つ求めます。
有向・無向グラフに対応します。部分集合 DP を使うため、頂点数は最大24です。

```cpp
#include "kyopro/hamiltonian.hpp"

kyopro::hamiltonian<false> g(4); // 無向グラフ
g.add_edge(0, 1);
g.add_edge(1, 2);
g.add_edge(2, 3);
auto path = g.path(0, 3); // {0, 1, 2, 3}
g.add_edge(3, 0);
auto cycle = g.cycle();  // 例: {0, 3, 2, 1}。末尾から先頭へ戻る
```

## コンストラクタ

```cpp
kyopro::hamiltonian<Directed> g(int n = 0);
kyopro::hamiltonian<Directed> g(const vector<vector<int>>& graph);
kyopro::hamiltonian<Directed> g(int n, const vector<pair<int, int>>& edges);
```

`Directed` の既定値は `true`（有向）です。無向は `false` を指定します。
`0 <= n <= 24`、端点は `[0, n)` とします。
無向辺は辺リストでは1回、隣接リストでは両方向に同じ本数ずつ登録します。
自己ループは隣接リストに1回登録します。構築は $O(n^2+m)$ です。

## add_edge / size

```cpp
void g.add_edge(int u, int v);
int g.size() const;
```

有向版は `u` から `v` へ、無向版は両方向へ辺を追加します。
自己ループ・多重辺に対応します。いずれも $O(1)$ です。

## path

```cpp
vector<int> g.path(int start = -1, int goal = -1) const;
```

ハミルトン路の頂点列を返します。始点・終点はそれぞれ `-1` で指定なしにできます。
`start`・`goal` は `-1` または `[0, n)` とします。

```cpp
auto any = g.path();            // 両端を指定しない
auto from_zero = g.path(0);     // 始点のみ指定
auto to_three = g.path(-1, 3);  // 終点のみ指定
auto fixed = g.path(0, 3);      // 両端を指定
```

存在しない場合と空グラフでは空配列を返します。1頂点では辺がなくても `{0}` です。
2頂点以上で始点と終点を同じに指定した場合、路は存在しません。

## cycle

```cpp
vector<int> g.cycle(int start = 0) const;
```

`start` から始まるハミルトン閉路の頂点列を返します。
先頭頂点は末尾に重複させず、末尾から先頭への辺で閉じます。
非空グラフでは `0 <= start < n`、空グラフでは `start = 0` とします。

存在しない場合と空グラフでは空配列を返します。
1頂点では自己ループが必要です。無向の2頂点では異なる2本の辺が必要で、
1本の辺の往復は閉路に含めません。
有向の2頂点では両方向の辺が必要です。

## 計算量

`path`・`cycle` はそれぞれ時間 $O(n2^n)$、追加空間 $O(2^n+n)$ です。
DP は状態ごとに32ビットを使い、20頂点で約4 MiB、24頂点で約64 MiBです。
各呼び出しで DP を計算します。解が複数ある場合、返す解の順序は保証しません。
