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

## Topological Sort

```cpp
#include "kyopro/topological_sort.hpp"
```

DAG のトポロジカル順序を $O(n + m)$ で求めます。
隣接リストまたは `{from, to}` の辺リストから使えます。
閉路がある場合は空の `vector<int>` を返します。

詳しい使い方は [docs/topological_sort.md](docs/topological_sort.md) を参照してください。

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
