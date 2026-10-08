# 実装の解説

コンテスト中に実装を読み直したり、問題に合わせて変更したりするための解説です。
各ページでは、基本原理、内部データ、処理の流れと不変条件、計算量の根拠、改造時の注意点、改造後の確認例を説明します。
引数・戻り値・制約などの API 仕様は、対応する使用法ページを参照してください。

対象は `lib/kyopro/` の全45ライブラリと、同梱 ACL の公開12ライブラリです。
解説はこのリポジトリの実装に対応しており、同名の一般的なアルゴリズムと実装方法が異なる場合もあります。
ローカル版にも解説と実装コードが含まれるため、オフラインで参照できます。

改造するときは、まず制約と不変条件を確認し、変更後に各ページの確認例を使って愚直解との比較を行ってください。
記載した確認例は検証の観点であり、すべてが既存の自動テストに含まれるわけではありません。

[ライブラリ一覧に戻る](../index.md) / [配置・ランテス・提出用展開](../usage.md)

## データ構造

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| Convex Hull Trick | [使用法](../convex_hull_trick.md) | [解説](convex_hull_trick.md) |
| Union Find | [使用法](../union_find.md) | [解説](union_find.md) |
| Dynamic Union Find | [使用法](../dynamic_union_find.md) | [解説](dynamic_union_find.md) |
| Weighted Union Find | [使用法](../weighted_union_find.md) | [解説](weighted_union_find.md) |
| Persistent Union Find | [使用法](../persistent_union_find.md) | [解説](persistent_union_find.md) |
| Dynamic Segtree | [使用法](../dynamic_segtree.md) | [解説](dynamic_segtree.md) |
| Persistent Segtree | [使用法](../persistent_segtree.md) | [解説](persistent_segtree.md) |
| Segtree ND | [使用法](../segtree_nd.md) | [解説](segtree_nd.md) |
| Implicit Treap | [使用法](../implicit_treap.md) | [解説](implicit_treap.md) |
| Balanced Binary Search Tree | [使用法](../balanced_binary_search_tree.md) | [解説](balanced_binary_search_tree.md) |
| Sparse Table | [使用法](../sparse_table.md) | [解説](sparse_table.md) |
| Wavelet Matrix | [使用法](../wavelet_matrix.md) | [解説](wavelet_matrix.md) |
| Dynamic Wavelet Matrix | [使用法](../dynamic_wavelet_matrix.md) | [解説](dynamic_wavelet_matrix.md) |
| Dynamic Range Kth | [使用法](../dynamic_range_kth.md) | [解説](dynamic_range_kth.md) |
| Cumulative Sum | [使用法](../cumulative_sum.md) | [解説](cumulative_sum.md) |
| Interval Set | [使用法](../interval_set.md) | [解説](interval_set.md) |
| Interval Heap | [使用法](../interval_heap.md) | [解説](interval_heap.md) |
| Binary Trie | [使用法](../binary_trie.md) | [解説](binary_trie.md) |

## グラフ・木

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| Dijkstra | [使用法](../dijkstra.md) | [解説](dijkstra.md) |
| Topological Sort | [使用法](../topological_sort.md) | [解説](topological_sort.md) |
| Cycle Detection | [使用法](../cycle_detection.md) | [解説](cycle_detection.md) |
| Bipartite Graph | [使用法](../bipartite_graph.md) | [解説](bipartite_graph.md) |
| Euler Tour | [使用法](../euler_tour.md) | [解説](euler_tour.md) |
| Lowest Common Ancestor | [使用法](../lowest_common_ancestor.md) | [解説](lowest_common_ancestor.md) |
| Tree Diameter | [使用法](../tree_diameter.md) | [解説](tree_diameter.md) |
| Tree Centroid | [使用法](../tree_centroid.md) | [解説](tree_centroid.md) |
| Rerooting DP | [使用法](../rerooting_dp.md) | [解説](rerooting_dp.md) |
| Doubling | [使用法](../doubling.md) | [解説](doubling.md) |

## 文字列

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| Rolling Hash | [使用法](../rolling_hash.md) | [解説](rolling_hash.md) |
| KMP | [使用法](../kmp.md) | [解説](kmp.md) |
| Manacher | [使用法](../manacher.md) | [解説](manacher.md) |
| Trie | [使用法](../trie.md) | [解説](trie.md) |
| Run Length Encoding | [使用法](../run_length_encoding.md) | [解説](run_length_encoding.md) |

## 数学

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| Sieve | [使用法](../sieve.md) | [解説](sieve.md) |
| Binomial | [使用法](../binomial.md) | [解説](binomial.md) |
| Factorial Mod | [使用法](../factorial_mod.md) | [解説](factorial_mod.md) |
| Arbitrary Mod Convolution | [使用法](../arbitrary_mod_convolution.md) | [解説](arbitrary_mod_convolution.md) |
| Matrix | [使用法](../matrix.md) | [解説](matrix.md) |
| Bigint | [使用法](../bigint.md) | [解説](bigint.md) |
| Geometry | [使用法](../geometry.md) | [解説](geometry.md) |
| Angular Sort | [使用法](../angular_sort.md) | [解説](angular_sort.md) |

## その他

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| Binary Search | [使用法](../binary_search.md) | [解説](binary_search.md) |
| Ternary Search | [使用法](../ternary_search.md) | [解説](ternary_search.md) |
| Coordinate Compression | [使用法](../compress.md) | [解説](compress.md) |
| Mo | [使用法](../mo.md) | [解説](mo.md) |

## ACL（同梱版）

| ライブラリ | 使用法 | 実装の解説 |
| --- | --- | --- |
| DSU | [使用法](../atcoder/dsu.md) | [解説](atcoder/dsu.md) |
| Fenwick Tree | [使用法](../atcoder/fenwicktree.md) | [解説](atcoder/fenwicktree.md) |
| Segtree | [使用法](../atcoder/segtree.md) | [解説](atcoder/segtree.md) |
| Lazy Segtree | [使用法](../atcoder/lazysegtree.md) | [解説](atcoder/lazysegtree.md) |
| Max Flow | [使用法](../atcoder/maxflow.md) | [解説](atcoder/maxflow.md) |
| Min Cost Flow | [使用法](../atcoder/mincostflow.md) | [解説](atcoder/mincostflow.md) |
| SCC | [使用法](../atcoder/scc.md) | [解説](atcoder/scc.md) |
| Two SAT | [使用法](../atcoder/twosat.md) | [解説](atcoder/twosat.md) |
| Math | [使用法](../atcoder/math.md) | [解説](atcoder/math.md) |
| Modint | [使用法](../atcoder/modint.md) | [解説](atcoder/modint.md) |
| Convolution | [使用法](../atcoder/convolution.md) | [解説](atcoder/convolution.md) |
| String | [使用法](../atcoder/string.md) | [解説](atcoder/string.md) |
