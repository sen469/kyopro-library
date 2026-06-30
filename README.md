# 競プロライブラリ

## ディレクトリ構成

- `include/kyopro/`: 提出用ヘッダを置くディレクトリ
- `include/kyopro/all.hpp`: 全ライブラリをまとめて include するヘッダ
- `test/`: ライブラリの簡易テスト

コンパイル例:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Ilib test/implicit_treap_test.cpp
```

## Implicit Treap

```cpp
#include "kyopro/implicit_treap.hpp"
```

ACL の `lazy_segtree` に近い形式で、`op`, `e`, `mapping`, `composition`, `id` を渡して使います。
`insert`, `erase`, `reverse`, `apply`, `prod`, `get`, `set`, `to_vector` が使えます。

詳しい使い方は [docs/implicit_treap.md](docs/implicit_treap.md) を参照してください。

## Dynamic Segtree

```cpp
#include "kyopro/dynamic_segtree.hpp"
```

必要な頂点だけを作るセグメント木です。
ACL の `segtree` に近い形式で、`op`, `e` を渡して使います。
座標が大きい場合でも、点更新、1 点取得、区間取得、`max_right`, `min_left` ができます。

詳しい使い方は [docs/dynamic_segtree.md](docs/dynamic_segtree.md) を参照してください。

## Interval Heap

```cpp
#include "kyopro/interval_heap.hpp"
```

両端優先度付きキューです。
最小値と最大値を $O(1)$ で取得し、挿入、最小値削除、最大値削除を $O(\log n)$ で行います。

詳しい使い方は [docs/interval_heap.md](docs/interval_heap.md) を参照してください。

## Angular Sort

```cpp
#include "kyopro/angular_sort.hpp"
```

点を正の x 軸方向から反時計回りの偏角順に $O(n \log n)$ でソートします。
同じ偏角の点は原点から近い順、原点 `(0, 0)` は先頭に並びます。
元の配列を変更せず添字列を返す `angular_sorted_indices` もあります。

詳しい使い方は [docs/angular_sort.md](docs/angular_sort.md) を参照してください。

## Arbitrary Mod Convolution

```cpp
#include "kyopro/arbitrary_mod_convolution.hpp"
```

任意 mod で畳み込みを $O(n \log n)$ で行います。
3 つの NTT friendly prime で計算し、CRT で指定した mod に復元します。

詳しい使い方は [docs/arbitrary_mod_convolution.md](docs/arbitrary_mod_convolution.md) を参照してください。

## Balanced Binary Search Tree

```cpp
#include "kyopro/balanced_binary_search_tree.hpp"
```

重複を許す平衡二分探索木です。
`insert`, `erase`, `count`, `contains`, `lower_bound`, `upper_bound`, `kth`, `order_of_key`, `to_vector` が使えます。

詳しい使い方は [docs/balanced_binary_search_tree.md](docs/balanced_binary_search_tree.md) を参照してください。

## Binomial

```cpp
#include "kyopro/binomial.hpp"
```

素数 mod で `nCk` を計算します。
階乗と逆階乗を前計算し、`comb(n, k)` または `operator()(n, k)` で二項係数を返します。

詳しい使い方は [docs/binomial.md](docs/binomial.md) を参照してください。

## BigInt

```cpp
#include "kyopro/bigint.hpp"
```

符号付き多倍長整数です。
`cin` で読み込んで `+`, `-`, `*`, 比較、`__int128` などの整数型との相互変換ができます。

詳しい使い方は [docs/bigint.md](docs/bigint.md) を参照してください。

## Run Length Encoding

```cpp
#include "kyopro/run_length_encoding.hpp"
```

連続する同じ値を `(値, 個数)` にまとめるランレングス圧縮です。
文字列、`vector`、iterator 範囲に対して使えます。

詳しい使い方は [docs/run_length_encoding.md](docs/run_length_encoding.md) を参照してください。

## Manacher

```cpp
#include "kyopro/manacher.hpp"
```

各中心の最長回文半径を $O(n)$ で求めます。
奇数長は `manacher(s)`、偶数長は `manacher_even(s)`、半開区間 `[l, r)` の回文判定は `palindrome_radii(s).is_palindrome(l, r)` が使えます。

詳しい使い方は [docs/manacher.md](docs/manacher.md) を参照してください。

## Sieve

```cpp
#include "kyopro/sieve.hpp"
```

エラトステネスの篩です。
素数判定、素数列挙、最小素因数、素因数分解、約数列挙ができます。
`kyopro::sieve<long long>` のように、素因数分解や約数列挙の値の型を指定できます。
`factorize(x)` は篩の上限を超える値も Miller-Rabin と Pollard Rho で分解します。

詳しい使い方は [docs/sieve.md](docs/sieve.md) を参照してください。

## Factorial Mod

```cpp
#include "kyopro/factorial_mod.hpp"
```

素数 mod で `n! mod p` を求めます。
`n < p` では 0 側または `p - 1` 側の近い方から計算し、`n >= p` では `0` を返します。
計算量は `n < p` で `O(min(n, p - 1 - n) + log p)` です。

詳しい使い方は [docs/factorial_mod.md](docs/factorial_mod.md) を参照してください。

## Compress

```cpp
#include "kyopro/compress.hpp"
```

座標圧縮を $O(n \log n)$ で行います。
圧縮後の配列と、圧縮後の値から元の値へ戻すための重複なし昇順配列を返します。
後から `get`, `lower_bound`, `upper_bound`, `contains` を使える `compressor` 型もあります。

詳しい使い方は [docs/compress.md](docs/compress.md) を参照してください。

## Cycle Detection

```cpp
#include "kyopro/cycle_detection.hpp"
```

有向グラフと無向グラフのサイクルを $O(n + m)$ で検出します。
見つかったサイクルの頂点列を返し、サイクルがない場合は空の `vector<int>` を返します。
辺リスト版の無向グラフでは自己ループと多重辺も検出できます。

詳しい使い方は [docs/cycle_detection.md](docs/cycle_detection.md) を参照してください。

## Dijkstra

```cpp
#include "kyopro/dijkstra.hpp"
```

非負重みのグラフで、始点から各頂点への最短距離を $O((n + m) \log n)$ で求めます。
隣接リストまたは `{from, to, cost}` の辺リストから使えます。
到達判定と最短経路復元もできます。

詳しい使い方は [docs/dijkstra.md](docs/dijkstra.md) を参照してください。

## Doubling

```cpp
#include "kyopro/doubling.hpp"
```

関数グラフ上で同じ遷移を何回も適用した結果を $O(\log k)$ で求めます。
遷移先がない場合は `-1` を使えます。
辺の値を畳み込みながら移動する `doubling_monoid` もあります。

詳しい使い方は [docs/doubling.md](docs/doubling.md) を参照してください。

## Lowest Common Ancestor

```cpp
#include "kyopro/lowest_common_ancestor.hpp"
```

木の 2 頂点の最小共通祖先をダブリングで求めます。
`lca`, `dist`, `kth_ancestor` が使え、森では別成分の `lca` と `dist` は `-1` を返します。

詳しい使い方は [docs/lowest_common_ancestor.md](docs/lowest_common_ancestor.md) を参照してください。

## Matrix

```cpp
#include "kyopro/matrix.hpp"
```

行列の加算、減算、乗算、累乗を行います。
`long long` や `atcoder::modint` など、`+`, `-`, `*`, `T(0)`, `T(1)` が使える型で利用できます。

詳しい使い方は [docs/matrix.md](docs/matrix.md) を参照してください。

## Topological Sort

```cpp
#include "kyopro/topological_sort.hpp"
```

DAG のトポロジカル順序を $O(n + m)$ で求めます。
隣接リストまたは `{from, to}` の辺リストから使えます。
閉路がある場合は空の `vector<int>` を返します。

詳しい使い方は [docs/topological_sort.md](docs/topological_sort.md) を参照してください。

## Tree Diameter

```cpp
#include "kyopro/tree_diameter.hpp"
```

重み付き木の直径を $O(n)$ で求めます。
直径の長さ、両端の頂点、両端を結ぶパスを返します。

詳しい使い方は [docs/tree_diameter.md](docs/tree_diameter.md) を参照してください。

## Rerooting DP

```cpp
#include "kyopro/rerooting_dp.hpp"
```

木の全方位 DP を $O(n)$ で行います。
`op`, `f_ve`, `f_ev` を渡して、各頂点を根にしたときの DP 値を求めます。
辺重みなどは辺 ID から外部配列を参照できます。

詳しい使い方は [docs/rerooting_dp.md](docs/rerooting_dp.md) を参照してください。

## Trie

```cpp
#include "kyopro/trie.hpp"
```

文字列集合を Trie 木で管理します。
`insert`, `erase`, `count`, `contains`, `starts_with`, `prefix_count` が使えます。

詳しい使い方は [docs/trie.md](docs/trie.md) を参照してください。

## Wavelet Matrix

```cpp
#include "kyopro/wavelet_matrix.hpp"
```

静的配列に対して、区間 k 番目、区間内の値の個数、前駆・後継を $O(\log \sigma)$ で求めます。
内部で座標圧縮するため、負数や `long long` も扱えます。

詳しい使い方は [docs/wavelet_matrix.md](docs/wavelet_matrix.md) を参照してください。

## Weighted Union Find

```cpp
#include "kyopro/weighted_union_find.hpp"
```

各頂点にポテンシャルを持つ Union-Find です。
制約 `potential[b] - potential[a] = w` の追加、矛盾検出、同一連結成分内のポテンシャル差取得ができます。

詳しい使い方は [docs/weighted_union_find.md](docs/weighted_union_find.md) を参照してください。

## 提出用 include 展開

```sh
python3 expander.py main.cpp
python3 expander.py main.cpp -o submit.cpp
python3 expander.py main.cpp --console
```

`#include "lib/all"` や `#include "lib/kyopro/..."` など、リポジトリ内のローカル include を再帰的に展開します。
`#include <bits/stdc++.h>` のような通常のシステム include はそのまま残し、同梱している `atcoder/...` は展開します。
展開したヘッダの開始コメントには GitHub 上の参照 URL も出力します。
出力先を指定しない場合は `combined.cpp` を作成します。

```cpp
// begin: kyopro/binomial.hpp (https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/binomial.hpp)
```

URL を変える場合は `--github-base` または `KYOPRO_GITHUB_BASE` を指定します。
空文字を指定すると URL 出力を無効にできます。

## セットアップ

`setup.sh` の上部にある `COMPETITIVE_PROGRAMMING_DIR` を普段使う競プロディレクトリに書き換えてから実行します。

```sh
./setup.sh
```

競プロディレクトリには `lib` へのシンボリックリンクを作り、`expander.py` と `rantes.sh` をコピーします。`generate.py` と `ans.cpp` がない場合は、ランテス用の雛形も作成します。
一時的に別のディレクトリへ入れる場合は引数でも指定できます。

```sh
./setup.sh /path/to/procon
```
