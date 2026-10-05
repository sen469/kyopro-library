# Tree Centroid

[実装の解説](explanations/tree_centroid.md)

実装: [lib/kyopro/tree_centroid.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/tree_centroid.hpp)

無向木の重心をすべて求めます。
重心とは、その頂点と接続する辺を取り除いたとき、残る各連結成分の頂点数が元の頂点数の半分以下になる頂点です。
非空の木の重心は1個または2個です。辺の重みや頂点の重みは扱いません。
直径の中央にある「木の中心」とは異なります。

```cpp
#include "kyopro/tree_centroid.hpp"
```

## tree_centroid

```cpp
vector<int> tree_centroid(const vector<vector<int>>& graph);
```

隣接リスト `graph` で表された木の重心を、頂点番号の昇順で返します。
入力は変更しません。頂点数を $n$ とします。

**戻り値**

- 重心の頂点番号を格納した `vector<int>`
- $n=0$ の場合は空配列
- $n=1$ の場合は `{0}`

**制約**

- 頂点数は `int` に収まる
- `graph` は連結な無向木、または頂点数0の空グラフ
- 各隣接頂点 `to` は `0 <= to < n`
- 無向辺は両方向に1回ずつ追加されている
- 自己ループ・多重辺はない

**計算量**

- 時間 $O(n)$
- 追加空間 $O(n)$
- 再帰は使用しません

## 辺リスト版

```cpp
vector<int> tree_centroid(int n, const vector<pair<int, int>>& edges);
```

頂点数 `n` と無向辺 `{u, v}` のリストから、重心を頂点番号の昇順で返します。
各無向辺は1回だけ渡します。戻り値は隣接リスト版と同じです。

**制約**

- `0 <= n`
- `edges.size() == max(0, n - 1)`
- 各辺について `0 <= u, v < n`
- 辺全体で連結な無向木をなす（`n == 0` の場合は空の辺リスト）

**計算量**

- 時間・追加空間ともに $O(n)$

## 使用例

```cpp
#include <iostream>
#include <utility>
#include <vector>
#include "kyopro/tree_centroid.hpp"

int main() {
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}};
    auto centroids = kyopro::tree_centroid(4, edges);
    for (int v : centroids) std::cout << v << ' ';
    std::cout << '\n'; // 1 2

    std::vector<std::vector<int>> graph = {{1}, {0, 2}, {1}};
    auto c = kyopro::tree_centroid(graph); // {1}
}
```

重心が1個あればよい場合は、非空の木について戻り値の `front()` を使えます。
森・一般グラフの重心や重心分解を行う関数ではありません。
