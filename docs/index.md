# 競プロライブラリ

C++ 用ライブラリの API・制約・計算量・使用例。

## データ構造

| ライブラリ | 用途 |
| --- | --- |
| [Union Find](union_find.md) | 連結判定・成分ごとの辺数・連結成分数 |
| [Dynamic Union Find](dynamic_union_find.md) | 任意のキーを使った連結管理 |
| [Weighted Union Find](weighted_union_find.md) | 重み付きの連結管理 |
| [Persistent Union Find](persistent_union_find.md) | 過去の版を保持する連結管理 |
| [Dynamic Segtree](dynamic_segtree.md) | 大きな座標範囲の点更新・区間集約 |
| [Persistent Segtree](persistent_segtree.md) | 過去の版を保持する点更新・区間集約 |
| [Segtree ND](segtree_nd.md) | 多次元の点更新・区間集約 |
| [Implicit Treap](implicit_treap.md) | 列への挿入・削除・反転・区間操作 |
| [Balanced Binary Search Tree](balanced_binary_search_tree.md) | 順序付き集合 |
| [Sparse Table](sparse_table.md) | 静的な区間クエリ |
| [Wavelet Matrix](wavelet_matrix.md) | 区間内の順位・頻度クエリ |
| [Cumulative Sum](cumulative_sum.md) | 多次元累積和 |
| [Interval Set](interval_set.md) | 整数区間の集合管理 |
| [Interval Heap](interval_heap.md) | 両端優先度付きキュー |
| [Binary Trie](binary_trie.md) | 整数をビット単位で管理 |

## グラフ・木

| ライブラリ | 用途 |
| --- | --- |
| [Dijkstra](dijkstra.md) | 非負辺重みの最短経路 |
| [Topological Sort](topological_sort.md) | 有向グラフのトポロジカル順序 |
| [Cycle Detection](cycle_detection.md) | 閉路検出 |
| [Bipartite Graph](bipartite_graph.md) | 二部グラフ判定 |
| [Euler Tour](euler_tour.md) | 部分木を連続区間に変換 |
| [Lowest Common Ancestor](lowest_common_ancestor.md) | 最小共通祖先 |
| [Tree Diameter](tree_diameter.md) | 木の直径 |
| [Rerooting DP](rerooting_dp.md) | 全方位木 DP |
| [Doubling](doubling.md) | 遷移の繰り返し |

## 文字列

| ライブラリ | 用途 |
| --- | --- |
| [Rolling Hash](rolling_hash.md) | 部分文字列のハッシュ |
| [KMP](kmp.md) | パターン検索 |
| [Manacher](manacher.md) | 回文の検出 |
| [Trie](trie.md) | 接頭辞を共有する文字列集合 |
| [Run Length Encoding](run_length_encoding.md) | 連続する同一要素の圧縮 |

## 数学

| ライブラリ | 用途 |
| --- | --- |
| [Sieve](sieve.md) | 素数・素因数分解 |
| [Binomial](binomial.md) | 二項係数 |
| [Factorial Mod](factorial_mod.md) | 素数を法とする階乗 |
| [Arbitrary Mod Convolution](arbitrary_mod_convolution.md) | 任意の法での畳み込み |
| [Matrix](matrix.md) | 行列演算 |
| [Bigint](bigint.md) | 多倍長整数 |
| [Geometry](geometry.md) | 幾何演算 |
| [Angular Sort](angular_sort.md) | 偏角ソート |

## その他

| ライブラリ | 用途 |
| --- | --- |
| [Binary Search](binary_search.md) | 二分探索 |
| [Ternary Search](ternary_search.md) | 三分探索 |
| [Coordinate Compression](compress.md) | 座標圧縮 |
| [Mo](mo.md) | オフライン区間クエリ |
| [Random Test Generator](random_test_generator.md) | ランダムテスト生成 |

## ACL（同梱版）

[ACL 一覧](atcoder/index.md) / [DSU](atcoder/dsu.md) /
[Fenwick Tree](atcoder/fenwicktree.md) / [Segtree](atcoder/segtree.md) /
[Lazy Segtree](atcoder/lazysegtree.md) / [Max Flow](atcoder/maxflow.md) /
[Min Cost Flow](atcoder/mincostflow.md) / [SCC](atcoder/scc.md) /
[Two SAT](atcoder/twosat.md) / [Math](atcoder/math.md) /
[Modint](atcoder/modint.md) / [Convolution](atcoder/convolution.md) /
[String](atcoder/string.md) / [Appendix](atcoder/appendix.md)
