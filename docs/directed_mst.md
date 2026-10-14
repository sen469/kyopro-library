# Directed MST

[実装の解説](explanations/directed_mst.md)

実装: [lib/kyopro/directed_mst.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/directed_mst.hpp)

根を指定した最小全域有向木（minimum spanning arborescence）をChu–Liu/Edmonds法で求めます。
根から全頂点へ到達でき、根以外の各頂点の入次数が1になる辺集合のうち、合計重みが最小のものを返します。
根の入次数は0、採用辺数は `n-1` です。最短経路木を求めるものではありません。

```cpp
#include "kyopro/directed_mst.hpp"
```

無向版の [Minimum Spanning Tree](minimum_spanning_tree.md) とは別クラスです。
非連結・到達不能の場合に部分的な森を返すことはありません。
「全頂点から根へ到達する木」が必要なら、辺をすべて反転して使います。

## directed_mst_result

```cpp
template <class T>
struct directed_mst_result {
    bool exists;
    T cost;
    vector<int> edge_ids;
    vector<int> parents;
    int parent(int v) const;
};
```

- `exists`: 指定根の最小全域有向木が存在するか
- `cost`: 採用した辺の重みの合計
- `edge_ids`: 採用辺IDの昇順配列。復元には `get_edge(id)` を使う
- `parents`: 長さnの親配列。`parents[v]` は採用された有向木における頂点vの親で、根は `-1`
- `parent(v)`: `parents[v]` を返す $O(1)$ の関数。`exists == true` かつ `0 <= v < n` が必要

失敗時は `exists == false`、`cost == T(0)`、`edge_ids` と `parents` は空です。
成功時もコスト0になり得るため、成否は必ず `exists` で判断してください。
1頂点では自己ループの有無にかかわらず成功し、コスト0・採用辺なし・`parents = {-1}` です。
空グラフには根がないため、`build(0)` は失敗を返します。

## コンストラクタ

```cpp
directed_mst<T> g(int n = 0);
directed_mst<T> g(int n, const vector<tuple<int, int, T>>& edges);
```

頂点数 `n` の有向グラフを作ります。
辺リスト版は `{from, to, cost}` 形式の辺を入力順に登録します。
入力配列はコピーし、変更しません。逆方向の辺は自動追加しません。

**制約**

- `0 <= n <= INT_MAX / 2`、辺数は `int` に収まる（縮約頂点にも整数IDを割り当てる）
- 各端点は `[0,n)`
- `T` はコピー可能で、`T(0)`、加算、減算、代入、大小比較が使える数値型
- 比較は重みの順序を表すこと。浮動小数点型では有限値のみを使う
- 縮約中の重みの減算・遅延値の加算・合計の加算を含め、すべての中間値が `T` に収まる

負の辺重み・多重辺・自己ループを許します。自己ループと根への入辺は採用しません。
浮動小数点型では丸め誤差があります。整数で表せる重みには整数型を推奨します。
合計用の型へ自動拡張しないため、必要に応じて `long long` などを指定してください。
最終コストが収まっていても、辺重みの差があふれる入力は対象外です。

**計算量**

- 頂点数のみ: 時間・空間 $O(1)$
- 辺リスト版: 時間・空間 $O(m)$

## add_edge

```cpp
int g.add_edge(int from, int to, T cost);
```

`from -> to` の有向辺を追加し、0始まりの追加順番号を辺IDとして返します。
多重辺・自己ループも1本ずつ登録します。
辺リスト版のコンストラクタを使った場合は、そのリストの長さからIDが続きます。

**制約**

- `0 <= from, to < n`
- 追加後の総辺数が `int` に収まる
- 重みの制約はコンストラクタと同じ

**計算量**

- ならし $O(1)$

## build

```cpp
directed_mst_result<T> g.build(int root) const;
```

登録されている全辺から、`root` を根とする最小全域有向木を計算します。
根から到達できない頂点が1つでもあれば失敗を返します。
各成分の入辺ヒープで補正後の重みが同じ場合は、元の辺IDが小さい方を優先します。
これは最適解全体の辺ID列が辞書順最小になる保証ではありません。

**制約**

- 非空グラフでは `0 <= root < n`
- 空グラフでは `root == 0` とし、失敗を返す

何度でも実行でき、根を変えることや計算後に辺を追加することもできます。
各呼び出しで最初から計算し、以前に返した結果と登録辺は変更しません。
動的な差分更新や結果のキャッシュは行いません。

**計算量**

- 時間 $O((n+m)\log(n+m+1))$
- 追加空間 $O(n+m)$
- 空グラフは $O(1)$
- `T` の演算・コピーを定数時間とする

入辺の管理には遅延減算付きleftist heap、成分管理には経路圧縮付きUnion-Findを使います。
復元用に縮約木を保存しますが、縮約のたびに全辺をコピーすることはありません。
ヒープの併合と縮約木の走査を含め、再帰は使用しません。

[Library CheckerのDirected MST](https://judge.yosupo.jp/problem/directedmst) では、
根の親を根自身として出力します。本ライブラリの根の親は `-1` なので、
`result.parents[v] == -1 ? v : result.parents[v]` と変換してください。
同問題では全頂点への到達が保証されていますが、一般の入力では `exists` を確認してから親を参照します。

## グラフ情報

```cpp
int g.size() const;
int g.edge_count() const;
tuple<int, int, T> g.get_edge(int id) const;
```

頂点数、登録辺数、指定辺の `{from,to,cost}` を返します。
`get_edge` はコピーを返し、`0 <= id < edge_count()` が必要です。
いずれも $O(1)$（`T` のコピーが定数時間の場合）で、`build` 前にも呼べます。

## 使用例

- [Directed MST (yosupo judge)](https://judge.yosupo.jp/submission/410357)

```cpp
#include <iostream>
#include "kyopro/directed_mst.hpp"

int main() {
    kyopro::directed_mst<long long> g(3);
    g.add_edge(0, 1, 5); // ID 0
    g.add_edge(0, 2, 6); // ID 1
    g.add_edge(1, 2, 1); // ID 2
    g.add_edge(2, 1, 1); // ID 3
    auto result = g.build(0);
    if (!result.exists) {
        std::cout << "Impossible\n";
        return 0;
    }
    std::cout << result.cost << '\n'; // 6
    std::cout << result.parent(2) << '\n'; // 1
    // result.parents == {-1, 0, 1}
    for (int id : result.edge_ids) std::cout << id << ' '; // 0 2
    std::cout << '\n';
}
```
