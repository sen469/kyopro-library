# 競プロライブラリ

## ディレクトリ構成

- `include/kyopro/`: 提出用ヘッダを置くディレクトリ
- `include/kyopro/all.hpp`: 全ライブラリをまとめて include するヘッダ
- `test/`: ライブラリの簡易テスト

コンパイル例:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude test/implicit_treap_test.cpp
```

## Implicit Treap

```cpp
#include "kyopro/implicit_treap.hpp"
```

ACL の `lazy_segtree` に近い形式で、`op`, `e`, `mapping`, `composition`, `id` を渡して使います。
`insert`, `erase`, `reverse`, `apply`, `prod`, `get`, `set`, `to_vector` が使えます。

詳しい使い方は [docs/implicit_treap.md](docs/implicit_treap.md) を参照してください。
