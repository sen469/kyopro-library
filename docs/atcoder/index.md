# AC(AtCoder) Library Document (同梱版)

このドキュメントは本リポジトリに同梱した ACL の説明です。AtCoder 上の版とは異なる場合があります。[AtCoder の公式ドキュメント](https://atcoder.github.io/ac-library/production/document_ja/)も参照してください。

## インストール方法

- zipファイルを解凍すると、`ac-library`フォルダ, そしてその中に`atcoder`フォルダが入っているはずです。
- g++を使っている場合, `atcoder`フォルダを`main.cpp`と同じ場所に置いて、`g++ main.cpp -std=c++14 -I .`でコンパイルできます。  
  - `-std=c++14`か`-std=c++17`をつけてコンパイルする必要があります。
- 詳しくは [Appendix](appendix.md) を参照してください。

## お約束

- 制約外の入力を入れたときの挙動はすべて未定義です。
- このドキュメントでは長い型を便宜上短く書きます
  - `unsigned int` → `uint`
  - `long long` → `ll`
  - `unsigned long long` → `ull`
- $0^0$ は $1$ です
- 明記されていない場合、多重辺や自己ループも入力可能です。

## リスト

`#include <atcoder/all>` : 一括include

### データ構造

- [`#include <atcoder/fenwicktree>`](fenwicktree.md)
- [`#include <atcoder/segtree>`](segtree.md)
- [`#include <atcoder/lazysegtree>`](lazysegtree.md)
- [`#include <atcoder/string>`](string.md)

### 数学

- [`#include <atcoder/math>`](math.md)
- [`#include <atcoder/convolution>`](convolution.md)
- 💻[`#include <atcoder/modint>`](modint.md)

### グラフ

- [`#include <atcoder/dsu>`](dsu.md)
- [`#include <atcoder/maxflow>`](maxflow.md)
- [`#include <atcoder/mincostflow>`](mincostflow.md)
- [`#include <atcoder/scc>`](scc.md)
- [`#include <atcoder/twosat>`](twosat.md)

## 付録

- [Appendix / FAQ](appendix.md)

## テスト

- [こちら](https://atcoder.jp/contests/practice2) で実際にこの Library を使う問題を解いてみることができます。

## ライセンス

`/document_en/lib`, `/document_ja/lib` 以下で再配布しているライブラリを除きCC0ライセンスで公開しています。

