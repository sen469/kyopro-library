# Tree Centroid

[実装の解説](explanations/tree_centroid.md)

実装: [lib/kyopro/tree_centroid.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/tree_centroid.hpp)

無向木の重心をすべて求めます。
重心とは、その頂点と接続する辺を取り除いたとき、残る各連結成分の頂点数が元の頂点数の半分以下になる頂点です。
重みなしの非空の木の重心は1個または2個です。
頂点重み付きの場合は、頂点数の代わりに非負の頂点重みの合計で判定します。
辺の重みは扱いません。
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

## 頂点重み付き版

```cpp
template <class T>
vector<int> tree_centroid(const vector<vector<int>>& graph,
                          const vector<T>& weights);

template <class T>
vector<int> tree_centroid(int n, const vector<pair<int, int>>& edges,
                          const vector<T>& weights);
```

`weights[v]` を頂点 `v` の重みとします。頂点を除いた後の各連結成分の重みの合計が、
除く前の全頂点の重みの合計の半分以下になる頂点を、番号の昇順で返します。
取り除いた頂点自身の重みは残存成分に含めません。入力は変更しません。
重みを省略する既存のAPIは、全頂点の重みを1とした場合と同じです。

**戻り値**

- 条件を満たす全頂点。空グラフでは空配列、1頂点なら `{0}`
- 重み0の頂点を許すため、重心が3個以上になることがあります
- 全頂点の重みが0なら、全頂点が重心です

**制約**

- 木の構造に関する制約は各重みなし版と同じ
- `weights.size() == n`（隣接リスト版では `n = graph.size()`）
- `T` は `bool` を除く整数型（例: `int`, `long long`, `unsigned long long`）
- `weights[v] >= 0`
- 全頂点の重みの合計が `T` に収まる

負の重み・浮動小数点数は対象外です。合計用の型への自動拡張は行いません。
合計が `int` に収まらない場合は、`vector<long long>` などを渡してください。

**計算量**

- 時間・追加空間ともに $O(n)$（`T` の演算を $O(1)$ とする）
- 再帰は使用しません

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

    std::vector<long long> weights = {10, 1, 1};
    auto weighted = kyopro::tree_centroid(graph, weights); // {0}
    std::vector<std::pair<int, int>> path = {{0, 1}, {1, 2}};
    auto weighted_edges = kyopro::tree_centroid(3, path, weights); // {0}
}
```

重心が1個あればよい場合は、非空の木について戻り値の `front()` を使えます。
森・一般グラフの重心や重心分解を行う関数ではありません。
