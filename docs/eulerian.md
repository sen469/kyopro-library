# Eulerian Path / Cycle

[実装の解説](explanations/eulerian.md)

実装: [lib/kyopro/eulerian.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/eulerian.hpp)

全辺をちょうど1回ずつ通るオイラー路・オイラー閉路を求めます。
有向・無向グラフ、自己ループ、多重辺に対応します。

```cpp
#include "kyopro/eulerian.hpp"

kyopro::eulerian<false> g(3); // 無向グラフ
g.add_edge(0, 1); // 辺 ID 0
g.add_edge(1, 2); // 辺 ID 1
auto res = g.path();
// res.exists == true
// res.vertices == {0, 1, 2}
// res.edge_ids == {0, 1}
```

## コンストラクタ

```cpp
kyopro::eulerian<Directed> g(int n = 0);
kyopro::eulerian<Directed> g(const vector<vector<int>>& graph);
kyopro::eulerian<Directed> g(int n, const vector<pair<int, int>>& edges);
```

`Directed` の既定値は `true`（有向）です。無向は `false` を指定します。
頂点は `[0,n)`、頂点数・辺数は `int` に収まるものとします。
無向辺は辺リストでは1回、隣接リストでは両方向に同じ本数ずつ登録します。
隣接リストの自己ループは1回登録します。
構築は時間・空間ともに $O(n+m)$ です。

## add_edge / size / edge_count

```cpp
int g.add_edge(int u, int v);
int g.size() const;
int g.edge_count() const;
```

`add_edge` は辺を追加し、0-indexed の追加順 ID を返します。
有向版は `u` から `v` へ、無向版は両方向に登録します。
辺リストから構築した場合はその並び順、隣接リストから構築した場合は走査順に ID を割り当てます。
無向隣接リストの `u > v` 側は重複登録しません。
`size` は頂点数、`edge_count` は辺数を返します。
辺追加はならし $O(1)$、個数取得は $O(1)$ です。

## path / cycle

```cpp
eulerian_result g.path(int start = -1) const;
eulerian_result g.cycle(int start = -1) const;
```

`path` はオイラー路（閉路になる場合も含む）、`cycle` は始点へ戻るオイラー閉路を返します。
`start = -1` は始点を自動選択します。指定する場合は `0 <= start < n` とします。
その始点から条件を満たす路が存在しなければ `exists = false` です。
辺を持たない孤立点を訪問する必要はありません。

**計算量**: 各呼び出しで時間・追加空間ともに $O(n+m)$。再帰は使いません。

## eulerian_result

```cpp
struct eulerian_result {
    bool exists;
    vector<int> vertices;
    vector<int> edge_ids;
};
```

- `exists`: 条件を満たす路・閉路が存在するか
- `vertices`: 通る頂点列。通常は `m+1` 要素
- `edge_ids`: 使用する辺の ID 列。`m` 要素

`edge_ids[i]` は `vertices[i]` から `vertices[i+1]` へ通る辺です。
閉路では `vertices.front() == vertices.back()` となり、末尾にも始点を含めます。
解が複数ある場合はそのうち1つを返します。
存在しない場合は `exists = false`、両配列は空です。

辺が0本の場合は路・閉路とも存在するとし、非空グラフでは指定始点（省略時は0）だけを返します。
空グラフでは両配列を空として返します。この場合、始点指定は `-1` のみです。

## 閉路の例

```cpp
kyopro::eulerian<> g(3); // 有向グラフ
g.add_edge(0, 1);
g.add_edge(1, 2);
g.add_edge(2, 0);
auto res = g.cycle();
// res.vertices == {0, 1, 2, 0}
// res.edge_ids == {0, 1, 2}
```
