# 配置・ランテス・提出用展開

## 必要な環境

- Bash と `diff` が使える環境（Linux・macOS・WSL など）
- `g++` と Python 3.9 以降
- ドキュメントもビルドする場合は Python 3.12 以降と MkDocs

以下ではライブラリを `~/kyopro-library` に置き、解答を作成する作業フォルダを
`~/competitive-programming` とします。コマンド中のパスは実際の配置に合わせてください。
ツールの実行自体にインターネット接続は不要です。

## 配置とセットアップ

リポジトリをダウンロード済みなら、そのディレクトリから実行します。

```sh
cd ~/kyopro-library
bash setup.sh ~/competitive-programming
cd ~/competitive-programming
```

作業フォルダには次のファイルが配置されます。

```text
~/kyopro-library/
  lib/                       # ライブラリ本体
    kyopro/                  # 自作ライブラリ
    atcoder/                 # 同梱 ACL
    all                      # 一括 include
  tool/                      # ツールの原本
  setup.sh

~/competitive-programming/
  lib -> ~/kyopro-library/lib
  expander.py                # setup.sh がコピー
  rantes.sh                  # setup.sh がコピー
  generate.py                # 存在しない場合だけ雛形を作成
  ans.cpp                    # 存在しない場合だけ雛形を作成
  main.cpp                   # 自分で作成する解答
```

`lib` はコピーではなく、ライブラリ本体へのシンボリックリンクです。
元のリポジトリを移動・削除するとリンクが使えなくなるため、固定の場所に置きます。
別 PC に持ち込む場合は、リポジトリもコピーして、その PC でセットアップし直してください。

セットアップを再実行すると `expander.py` と `rantes.sh` は上書きされますが、
既存の `generate.py` と `ans.cpp` は維持されます。ライブラリ本体の更新はリンク経由で反映され、
ツールの更新には再セットアップが必要です。

既存の `lib` が別のリンク先を指している場合に限り、次で付け替えられます。

```sh
cd ~/kyopro-library
bash setup.sh --force ~/competitive-programming
```

`lib` が実ディレクトリの場合は `--force` でも置換しません。
既存の内容を確認し、別の作業フォルダを指定してください。

## include とコンパイル

必要なヘッダを `main.cpp` から読み込みます。

```cpp
#include <bits/stdc++.h>
#include "kyopro/union_find.hpp"
#include <atcoder/segtree>
```

作業フォルダで `-I./lib` を指定してコンパイルします。

```sh
cd ~/competitive-programming
g++ -std=c++17 -O2 -Wall -Wextra -I. -I./lib main.cpp -o main
./main < input.txt
```

すべてのライブラリをまとめて読み込む場合は `#include "lib/all"` も使えます。
展開後のコード量を抑えたい場合は、必要なヘッダだけを読み込んでください。

## ランテスの準備

ランテスではランダムに生成した同じ入力を自分の解答と正解用の愚直解に渡し、出力を比較します。
次の3ファイルを作業フォルダに用意します。

| ファイル | 役割 |
| --- | --- |
| `main.cpp` | 検証したい解答 |
| `ans.cpp` | 小さい入力なら確実に正解する愚直解 |
| `generate.py` | 問題の入力形式に従って1ケースを標準出力に出すプログラム |

`setup.sh` が作る `generate.py` と `ans.cpp` は空の雛形です。
そのまま実行せず、問題に合わせて内容を実装してください。

例えば「長さ `n` の整数列の総和」を検証する場合、`generate.py` は次のように書けます。

```python
import random

n = random.randint(1, 10)
a = [random.randint(-10, 10) for _ in range(n)]
print(n)
print(*a)
```

この例の `ans.cpp` は次のようになります。`main.cpp` 側も同じ入力を読み、総和だけを出力します。

```cpp
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        long long x;
        std::cin >> x;
        sum += x;
    }
    std::cout << sum << '\n';
}
```

最初は愚直解でもすぐ終わる小さい制約を使い、境界値・重複・負の値などは問題の制約内で生成します。
デバッグ出力は標準出力に混ぜず、`std::cerr` などを使ってください。

## ランテスの実行と失敗ケース

```sh
cd ~/competitive-programming
bash rantes.sh main.cpp ans.cpp
```

スクリプトは `g++ -I. -I./lib -O3` で両方をコンパイルし、以下を繰り返します。

1. `python3 generate.py` の出力を `input.txt` に保存。
2. `main` の出力を `out1.txt`、`ans` の出力を `out2.txt` に保存。
3. `diff` で出力を比較。不一致なら `WA found!` と入力を表示して停止。

実行場所は、必ず `generate.py` と `lib` がある作業フォルダにしてください。
`main`・`ans`・`input.txt`・`out1.txt`・`out2.txt` は実行時に上書きされます。

不一致または解答プログラムの異常終了で停止した場合、入出力ファイルは残ります。
再実行前に失敗ケースを別名で保存すると、修正後にも再現できます。

```sh
cp input.txt failed-input.txt
cp out1.txt failed-main.txt
cp out2.txt failed-ans.txt
./main < failed-input.txt
./ans < failed-input.txt
```

手動停止は `Ctrl+C` です。**手動停止時は `main`・`ans`・`out1.txt`・`out2.txt`・
`input.txt`・`dbg` が削除されます。** 同じ名前で保存したいファイルを置かないでください。
引数なしの `bash rantes.sh` も、これらを削除する後片付けコマンドです。

現在のスクリプトは試行回数の上限・タイムアウト・浮動小数点の誤差判定を持ちません。
空白や改行も含めた `diff` 比較なので、複数の正解出力がある問題にもそのままでは使えません。
また、乱数生成器の終了コードは検査しないため、事前に `python3 generate.py` で入力を確認してください。
コンパイル時の `-std` 指定はないため、必要な言語規格が既定値と異なる場合は、
作業フォルダの `rantes.sh` の両方のコンパイルコマンドに `-std=c++17` などを追加します。

## 提出用にライブラリを展開

検証が終わったら、作業フォルダで解答とライブラリを1ファイルにまとめます。

```sh
cd ~/competitive-programming
python3 expander.py main.cpp -o submit.cpp
g++ -std=c++17 -O2 -Wall -Wextra submit.cpp -o submit
./submit < input.txt
```

`input.txt` は確認用の入力を用意してください。ランテスを `Ctrl+C` で終了した場合は削除されています。
展開後は `-I./lib` なしでコンパイルし、提出先へは生成された `submit.cpp` を提出します。
`-o` を省略すると、実行したディレクトリの `combined.cpp` に出力します。
出力先は上書きされるため、元の `main.cpp` を指定しないでください。

同梱 ACL とローカルヘッダの `#include` を再帰的に展開し、同じファイルの重複展開は省きます。
探索先で見つからないヘッダは `#include` のまま残るため、通常の標準ヘッダもそのままです。
条件付きコンパイルの条件を評価する C++ プリプロセッサではないので、展開後のコンパイルも確認します。

### 展開オプション

| オプション | 用途 |
| --- | --- |
| `-o submit.cpp` | 出力ファイルを指定 |
| `--console` または `-c` | ファイルを作らず標準出力へ出す |
| `--lib /path/to/includes` | ヘッダの探索先を追加。複数回指定可能 |
| `--ignore /path/to/header.hpp` | 指定ファイル・ディレクトリを展開せず include を残す |
| `--origname main.cpp` | 元ファイルの行番号を追える `#line` を追加 |
| `--github-base ""` | 展開開始コメントへの GitHub URL 付与を無効化 |

`lib/debug/` は既定で展開対象外です。デバッグ用 include は削除されず残るため、
提出時に有効にならない `#ifdef LOCAL` などで囲むか、提出用コードから取り除いてください。
コメントの GitHub URL は参照情報であり、展開中にネットアクセスすることはありません。

## よくある問題

| 症状 | 確認すること |
| --- | --- |
| `kyopro/...` や `atcoder/...` が見つからない | `lib` のリンク先が存在するか、コンパイル時に `-I./lib` があるか |
| `generate.py` が見つからない | ランテスを作業フォルダから実行しているか |
| ランテスで入力が空になる | 雛形を編集したか、生成器にエラーがないか |
| 展開後もローカル include が残る | ヘッダの場所、`--lib`、`--ignore`、`lib/debug/` を確認 |
| コピーしたツールが古い | リポジトリ更新後に `setup.sh` を再実行したか |
