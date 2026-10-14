# Tree Centroid

[実装の解説](explanations/tree_centroid.md)

実装: [lib/kyopro/tree_centroid.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/tree_centroid.hpp)

```cpp
#include "kyopro/tree_centroid.hpp"
```

無向木の重心をすべて求めるクラスです。
重心とは、その頂点を除いた各連結成分の頂点数が、元の頂点数の半分以下になる頂点です。
非負の頂点重みを渡すと、頂点数の代わりに重みの合計で判定します。
辺重みは扱いません。直径の中央にある「木の中心」とは異なります。

## コンストラクタ

```cpp
tree_centroid g(int n = 0);
tree_centroid g(const vector<vector<int>>& graph);
tree_centroid g(int n, const vector<pair<int, int>>& edges);
```

辺を保存するだけで重心は計算しません。
`n >= 0`、端点は `[0,n)` とします。
無向辺を、辺リストでは1回、隣接リストでは両方向に登録してください。
頂点数のみなら $O(n)$、それ以外は $O(n+m)$ です。

## add_edge・size

```cpp
void g.add_edge(int u, int v);
int g.size() const;
```

無向辺を追加します。`u,v` は `[0,n)`、`u != v` とします。
追加はならし $O(1)$、頂点数取得は $O(1)$ です。

## build

```cpp
vector<int> g.build() const;

template <class T>
vector<int> g.build(const vector<T>& weights) const;
```

重心を頂点番号の昇順で返します。
重みなし版は全頂点の重みが1の場合と同じです。
`weights[v]` は頂点vの重みで、除去後の各成分の重みが、除去前の全体重みの半分以下かを判定します。

**制約**

- 計算時のグラフは連結な無向木、または空グラフ
- 自己ループ・多重辺・閉路は不可
- 重み付きでは `weights.size() == n`
- `T` はbool以外の整数型、各重みは非負
- 全重みの合計が `T` に収まる（自動拡張しない）

空グラフは空配列、1頂点は `{0}` を返します。
重みなしの非空木では重心は1個または2個です。
重み0を許す場合は3個以上になることがあり、全重み0なら全頂点が重心です。
同じ木に異なる重みを渡して何度でも再計算できます。
入力の重みと過去の返り値は変更しません。

**計算量**: 時間・追加空間ともに $O(n)$。再帰なし。`T` の演算は定数時間とします。

## 使用例

```cpp
kyopro::tree_centroid g(3);
g.add_edge(0, 1);
g.add_edge(1, 2);
auto a = g.build(); // {1}
std::vector<long long> weights = {10, 1, 1};
auto b = g.build(weights); // {0}
```

型Tは重み配列から推論し、クラスのテンプレート引数には指定しません。
