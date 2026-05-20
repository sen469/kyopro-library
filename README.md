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

## 提出用 include 展開

```sh
python3 expander.py main.cpp
python3 expander.py main.cpp -o submit.cpp
python3 expander.py main.cpp --console
```

`#include "lib/all"` や `#include "lib/kyopro/..."` など、リポジトリ内のローカル include を再帰的に展開します。
`#include <bits/stdc++.h>` のような通常のシステム include はそのまま残し、同梱している `atcoder/...` は展開します。
出力先を指定しない場合は `combined.cpp` を作成します。

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
