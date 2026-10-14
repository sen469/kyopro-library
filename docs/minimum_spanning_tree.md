# Minimum Spanning Tree

[実装の解説](explanations/minimum_spanning_tree.md) / [共通API](graph_tree.md)

実装: [lib/kyopro/minimum_spanning_tree.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/minimum_spanning_tree.hpp)

無向グラフの最小全域木（MST）をKruskal法で求めるクラスです。
`add_edge` で辺を追加し、`build` で計算します。
非連結の場合は、各連結成分の最小全域木を合わせた最小全域森を返します。
負の辺重み・多重辺・自己ループに対応します。自己ループは採用しません。

```cpp
#include "kyopro/minimum_spanning_tree.hpp"
```

## mst_result

```cpp
template <class T>
struct mst_result {
    T cost;
    vector<int> edge_ids;
    int component_count;
    bool is_connected() const;
};
```

- `cost`: 採用した辺の重みの合計。非連結の場合は最小全域森全体の合計
- `edge_ids`: 採用した辺のID（追加順番号）。重みの昇順、同じ重みならIDの昇順
- `component_count`: 入力グラフの連結成分数（孤立点を含む）
- `is_connected()`: `component_count <= 1` を返す $O(1)$ の関数

採用辺数は常に `n - component_count` です。
空グラフでは `cost = T(0)`、`edge_ids = {}`、`component_count = 0` とし、
便宜上 `is_connected() == true` とします。
1頂点では自己ループの有無によらず、コスト0・採用辺なし・成分数1です。
非空の連結グラフだけを認めたい場合は `n > 0 && result.is_connected()` で判定してください。

## コンストラクタ

```cpp
minimum_spanning_tree<T> g(int n = 0);
minimum_spanning_tree<T> g(int n, const vector<tuple<int, int, T>>& edges);
```

頂点数 `n` のグラフを作ります。頂点数だけを渡す場合、辺数は0です。
辺リスト版は `{u, v, cost}` 形式の無向辺を入力順に追加します。
各無向辺は1回だけ渡します。入力配列はコピーして保持し、変更しません。
この段階では最小全域木の計算は行いません。

**制約**

- `0 <= n`
- 辺数 `m = edges.size()` は `int` に収まる
- 各辺について `0 <= u, v < n`
- `T` はコピー可能で、`T(0)`、加算、代入、`operator<` が使える数値型
- 比較が重みの順序を表すこと。浮動小数点型では有限値を使い、NaNを含めない
- `build` のコスト計算のすべての中間和・最終和が `T` に収まる

合計用の型への自動拡張はしません。`int` に収まらない場合は入力を `long long` にしてください。
負の重みと正の重みがある場合、最終和だけでなく昇順に加算した途中の値にも注意してください。
浮動小数点型では通常の丸め誤差があります。
有向グラフの最小全域有向木は扱いません。

**計算量**

- 頂点数のみ: 時間・空間 $O(1)$
- 辺リスト版: 時間・空間 $O(m)$

## add_edge

```cpp
int g.add_edge(int u, int v, T cost);
```

重み `cost` の無向辺を追加し、0始まりの追加順番号を辺IDとして返します。
自己ループ・多重辺もそれぞれ1本として保存します。
辺リストを渡したコンストラクタの後に呼ぶ場合、IDはそのリストの長さから続きます。

**制約**

- `0 <= u, v < n`
- 追加後の総辺数が `int` に収まる
- 重みの制約はコンストラクタと同じ

**計算量**

- ならし $O(1)$

## build

```cpp
mst_result<T> g.build() const;
```

現在の辺全体から最小全域木・森を計算して返します。
辺の追加順やIDは変更しません。
何度でも呼べて、計算後に辺を追加することもできます。
毎回最初から計算し、結果をキャッシュしたり差分更新したりはしません。
過去に返した結果は、新しい辺の追加や再計算で変更されません。

**計算量**

- 時間 $O(n + m\log(m+1) + m\alpha(n))$
- 追加空間 $O(n+m)$
- 上記は `T` の比較・加算・コピーが定数時間の場合

内部の連結判定には同梱ACLの `atcoder::dsu` を使用します。

## グラフ情報

```cpp
int g.size() const;
int g.edge_count() const;
tuple<int, int, T> g.get_edge(int id) const;
```

`size()` は頂点数、`edge_count()` は自己ループ・多重辺を含む登録辺数です。
`get_edge(id)` はその辺の `{u,v,cost}` をコピーして返します。
採用辺は `g.get_edge(result.edge_ids[i])` で取得できます。
`get_edge` では `0 <= id < edge_count()` が必要です。
いずれも $O(1)$（`T` のコピーが定数時間の場合）で、`build` 前にも呼べます。

## 使用例

 - [Minimum Spanning Tree (yosupo judge)](https://judge.yosupo.jp/submission/410353)

```cpp
#include <iostream>
#include <tuple>
#include <vector>
#include "kyopro/minimum_spanning_tree.hpp"

int main() {
    kyopro::minimum_spanning_tree<long long> g(4);
    g.add_edge(0, 1, 4); // ID 0
    g.add_edge(0, 2, 1); // ID 1
    g.add_edge(1, 2, 2); // ID 2
    g.add_edge(1, 3, 1); // ID 3
    g.add_edge(2, 3, 5); // ID 4
    auto result = g.build();
    if (!result.is_connected()) {
        std::cout << "Disconnected\n";
        return 0;
    }
    std::cout << result.cost << '\n'; // 4
    for (int id : result.edge_ids) std::cout << id << ' '; // 1 3 2
    std::cout << '\n';
}
```

非連結の場合も結果は有効です。例えば5頂点に辺 `(0,1,2)` と `(2,3,-1)` だけがある場合、
コストは1、成分数は3で、`is_connected()` は `false` になります。
