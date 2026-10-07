# 競プロライブラリ

[ドキュメントサイト](https://sen469.github.io/kyopro-library/)

[配置・ランテス・提出用展開のガイド](docs/usage.md)

[各ライブラリの実装解説](docs/explanations/index.md)

## ドキュメントの開発・公開

Python 3.12 以降を使用します。

```sh
python3 -m venv .venv-docs
source .venv-docs/bin/activate
python -m pip install -r requirements-docs.txt
python -m mkdocs serve
```

表示されたローカル URL でプレビューできます。公開用の検証は次のコマンドで実行します。

```sh
python -m mkdocs build --strict
python tool/check_docs.py
```

### ICPC などでのオフライン利用

Python 3.12 以降を事前にインストールし、以下はリポジトリのルートで実行します。
`uv` は不要です。

**ネット接続がある間の準備（当日使う PC で実行）**

```sh
python3 -m venv .venv-docs
source .venv-docs/bin/activate
python -m pip install -r requirements-docs.txt
python -m mkdocs build --strict -f mkdocs.local.yml
```

Windows の PowerShell では、仮想環境の有効化に
`.venv-docs\Scripts\Activate.ps1` を使います。

リポジトリ全体と `.venv-docs/` を当日の PC に残しておいてください。
生成された `site-local/` には本文・検索データ・数式用 JavaScript・フォント・
実装コードが含まれます。数式表示にも CDN への接続は不要です。

**ネット接続なしで再ビルド・閲覧**

```sh
source .venv-docs/bin/activate
python -m mkdocs build --strict -f mkdocs.local.yml
python -m http.server 8000 --bind 127.0.0.1 --directory site-local
```

ブラウザで <http://127.0.0.1:8000/> を開きます。
`localhost` 内の通信だけなのでインターネット接続は不要です。
8000 番ポートが使用中なら、例えば 8001 に変更してください。
終了は `Ctrl+C` です。`index.html` の直接オープンでは検索が動かないため、
上記のローカルサーバーを使ってください。

文書を編集しながら確認するときは、代わりに
`python -m mkdocs serve -f mkdocs.local.yml --dev-addr 127.0.0.1:8000` を使えます。
ビルド済みの `site-local/` を別の PC にコピーする場合は、閲覧用の Python だけでよく、
MkDocs のインストールや再ビルドは不要です。

ローカル版の「実装」リンクは同梱したコードをテキスト表示します。
問題サイト・ACL 公式の使用例などへの外部リンクはオフラインでは開けません。

**依存パッケージもオフラインで再インストールしたい場合**

接続できる間に、当日と同じ OS・CPU・Python バージョンの環境で保存します。

```sh
python -m pip download --only-binary=:all: -r requirements-docs.txt -d wheelhouse-docs
```

ネット接続のない環境で仮想環境を作成・有効化した後、次を実行します。

```sh
python -m pip install --no-index --find-links=wheelhouse-docs -r requirements-docs.txt
```

`.venv-docs/` は別 PC へ移植せず、その PC で作り直してください。
`site-local/` と `wheelhouse-docs/` は Git 管理対象外です。

### 文書の追加と GitHub Pages への公開

新しいページは `docs/` に Markdown で追加し、`mkdocs.yml` の `nav` と
`docs/index.md` の一覧に登録します。文書間リンクには `.md` の相対パスを、
実装へのリンクには GitHub 上の `main` ブランチの URL を使います。
数式には `$...$` または `$$...$$` を使えます。

ライブラリを追加・変更するときは、使用法とあわせて実装解説も更新します。
自作ライブラリの解説は `docs/explanations/<ヘッダ名>.md`、
同梱 ACL の解説は `docs/explanations/atcoder/<ヘッダ名>.md` に置き、
使用法ページと解説ページを相互リンクします。
解説は `docs/explanations/index.md` と `mkdocs.yml` の「実装の解説」にも登録してください。

初回は GitHub の **Settings > Pages > Build and deployment > Source** を
**GitHub Actions** に設定してください。`main` への push または
**Actions > Documentation > Run workflow** で公開されます。
Pull Request ではビルドとリンク検証だけを実行します。
公開先は `https://sen469.github.io/kyopro-library/` です。

## ディレクトリ構成

- `lib/kyopro/`: 提出用ヘッダを置くディレクトリ
- `lib/all`: 全ライブラリをまとめて include するヘッダ
- `docs/`: ドキュメントの Markdown
- `docs/explanations/`: 基本原理・内部実装・改造時の注意点の解説
- `test/`: ライブラリの簡易テスト

コンパイル例:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Ilib test/implicit_treap_test.cpp
```

## Convex Hull Trick

```cpp
#include "kyopro/convex_hull_trick.hpp"
```

傾きが単調な順序で直線を追加し、任意の順序の座標に対する最小値・最大値を求めます。
`add_line` はならし O(1)、`query` は O(log N) です。
最小値では傾きを非増加、最大値では非減少の順に追加してください。

詳しい使い方は [docs/convex_hull_trick.md](docs/convex_hull_trick.md) を参照してください。

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

## Union Find

```cpp
#include "kyopro/union_find.hpp"
```

通常の Union-Find です。
`merge` で追加された辺数を連結成分ごとに管理し、`edge_count`, `unique_edge_count`, `group_count` で取得できます。

詳しい使い方は [docs/union_find.md](docs/union_find.md) を参照してください。

## Dynamic Union Find

```cpp
#include "kyopro/dynamic_union_find.hpp"
```

出てきた要素だけを管理する Union-Find です。
`long long` や `string` などのキーをそのまま使え、未登録の要素は `merge`, `same`, `leader`, `size` で自動追加されます。

詳しい使い方は [docs/dynamic_union_find.md](docs/dynamic_union_find.md) を参照してください。

## Euler Tour

```cpp
#include "kyopro/euler_tour.hpp"
```

木または森の Euler Tour を作ります。
`in`, `out`, `subtree`, `order`, `parent`, `depth`, `is_ancestor` が使えます。
部分木の頂点集合は `order[in[v]], ..., order[out[v] - 1]` に対応します。

詳しい使い方は [docs/euler_tour.md](docs/euler_tour.md) を参照してください。

## Persistent Segtree

```cpp
#include "kyopro/persistent_segtree.hpp"
```

各更新後の版を残せるセグメント木です。
ACL の `segtree` に近い形式で、`op`, `e` を渡して使います。
`set` は元の木を変更せず、新しい版を返します。

詳しい使い方は [docs/persistent_segtree.md](docs/persistent_segtree.md) を参照してください。

## Segtree ND

```cpp
#include "kyopro/segtree_nd.hpp"
```

任意次元配列に対する点更新・直方体領域取得ができるセグメント木です。
`segtree_nd<S, D, op, e>` として、次元数をテンプレート引数で指定して使います。

詳しい使い方は [docs/segtree_nd.md](docs/segtree_nd.md) を参照してください。

## Persistent Union Find

```cpp
#include "kyopro/persistent_union_find.hpp"
```

各併合後の版を残せる Union-Find です。
`merge` は元の版を変更せず、併合後の版と実際に併合したかを返します。

詳しい使い方は [docs/persistent_union_find.md](docs/persistent_union_find.md) を参照してください。

## Sparse Table

```cpp
#include "kyopro/sparse_table.hpp"
```

静的配列に対して、`min`, `max`, `gcd` などの冪等な区間演算を $O(1)$ で求めます。
ACL の `segtree` に近い形式で、`op`, `e` を渡して使います。

詳しい使い方は [docs/sparse_table.md](docs/sparse_table.md) を参照してください。

## Cumulative Sum

```cpp
#include "kyopro/cumulative_sum.hpp"
```

任意次元の累積和です。
`cumulative_sum_nd<T, D>` として、次元数をテンプレート引数で指定して使います。

詳しい使い方は [docs/cumulative_sum.md](docs/cumulative_sum.md) を参照してください。

## Interval Heap

```cpp
#include "kyopro/interval_heap.hpp"
```

両端優先度付きキューです。
最小値と最大値を $O(1)$ で取得し、挿入、最小値削除、最大値削除を $O(\log n)$ で行います。

詳しい使い方は [docs/interval_heap.md](docs/interval_heap.md) を参照してください。

## Interval Set

```cpp
#include "kyopro/interval_set.hpp"
```

整数上の半開区間を、互いに重ならず隣接もしない区間へ統合して管理します。
区間の追加・削除、包含判定、包含区間の取得、`mex`、区間列挙ができます。

詳しい使い方は [docs/interval_set.md](docs/interval_set.md) を参照してください。

## Geometry

```cpp
#include "kyopro/geometry.hpp"
```

2次元幾何で使う点・直線・円と、公差 `eps` 付きの判定関数集です。
直線上判定、同一直線判定、円周上判定、円同士の位置関係などが使えます。

詳しい使い方は [docs/geometry.md](docs/geometry.md) を参照してください。

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

## Binary Search

```cpp
#include "kyopro/binary_search.hpp"
```

整数の半開区間 `[first, last)` で、単調な判定が真になる最初の位置を二分探索します。
該当する位置がなければ `last` を返します。

詳しい使い方は [docs/binary_search.md](docs/binary_search.md) を参照してください。

## Ternary Search

```cpp
#include "kyopro/ternary_search.hpp"
```

整数の半開区間 `[first, last)` で、単峰な関数の最小値または最大値を取る位置を三分探索します。
同じ最適値を取る位置が複数あれば最も左を返します。

詳しい使い方は [docs/ternary_search.md](docs/ternary_search.md) を参照してください。

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

## KMP

```cpp
#include "kyopro/kmp.hpp"
```

文字列や `vector` からパターンの出現位置を $O(n + m)$ で検索します。
重なりを含む全出現位置の取得、最初の出現位置の取得、prefix function の計算ができます。

詳しい使い方は [docs/kmp.md](docs/kmp.md) を参照してください。

## Rolling Hash

```cpp
#include "kyopro/rolling_hash.hpp"
```

$2^{61} - 1$ mod の Rolling Hash です。
文字列や整数列に対して、部分列ハッシュ、部分列一致判定、LCP を求められます。

詳しい使い方は [docs/rolling_hash.md](docs/rolling_hash.md) を参照してください。

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

## Bipartite Graph

```cpp
#include "kyopro/bipartite_graph.hpp"
```

無向グラフが二部グラフか $O(n + m)$ で判定し、二部グラフなら各頂点の 2 彩色を取得できます。
非連結グラフ、自己ループ、多重辺を扱えます。

詳しい使い方は [docs/bipartite_graph.md](docs/bipartite_graph.md) を参照してください。

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

## Mo's Algorithm

```cpp
#include "kyopro/mo.hpp"
```

静的配列に対するオフライン区間クエリを、区間の両端を移動させながら処理します。
左右共通の追加・削除処理だけでなく、左右で異なる処理も指定できます。

詳しい使い方は [docs/mo.md](docs/mo.md) を参照してください。

## Topological Sort

```cpp
#include "kyopro/topological_sort.hpp"
```

DAG のトポロジカル順序を $O(n + m)$ で求めます。
隣接リストまたは `{from, to}` の辺リストから使えます。
閉路がある場合は空の `vector<int>` を返します。

詳しい使い方は [docs/topological_sort.md](docs/topological_sort.md) を参照してください。

## Tree Centroid

```cpp
#include "kyopro/tree_centroid.hpp"
```

無向木の重心をすべて、頂点番号の昇順で返します。
`kyopro::tree_centroid(graph)` または `kyopro::tree_centroid(n, edges)` で使えます。
最後の引数に非負の整数の頂点重み配列 `weights` を渡すと、頂点数ではなく重みの合計で判定します。
時間・追加空間ともに $O(n)$ で、再帰は使用しません。

詳しい使い方は [docs/tree_centroid.md](docs/tree_centroid.md)、
実装の解説は [docs/explanations/tree_centroid.md](docs/explanations/tree_centroid.md) を参照してください。

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
ACL の `segtree` / `lazy_segtree` に近い形式で、`op`, `e`, `f_ve`, `f_ev` をテンプレート引数で渡して使います。
`kyopro::rerooting_dp<R, M, op, e, f_ve, f_ev> g(n)` を作り、`add_edge` で辺を追加します。
`g.build(root)` は部分木 DP の配列、`g.reroot()` は各頂点を根にしたときの答えを返します。
どちらも複数回呼べます。外部の重みなどを変更した場合は `build` を呼び直してください。
`add_edge(u, v, idx, xdi)` で方向別の辺 ID も指定できます。

詳しい使い方は [docs/rerooting_dp.md](docs/rerooting_dp.md) を参照してください。

## Binary Trie

```cpp
#include "kyopro/binary_trie.hpp"
```

符号なし整数の多重集合をビット Trie で管理します。
挿入・削除、XOR 最小/最大、昇順 k 番目、未満個数を $O(BIT\_SIZE)$、全要素への XOR を $O(1)$ で処理できます。

詳しい使い方は [docs/binary_trie.md](docs/binary_trie.md) を参照してください。

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
