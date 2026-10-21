# Aho–Corasick

[実装の解説](explanations/aho_corasick.md)

実装: [lib/kyopro/aho_corasick.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/aho_corasick.hpp)

複数のパターンを登録し、テキスト内の出現回数・一致位置を求めます。
重なった一致、同じパターンの複数登録、空文字列に対応します。

```cpp
#include "kyopro/aho_corasick.hpp"
```

以下では文字種数を $\sigma$、登録数を $P$、根を含むノード数を $V$、テキスト長を $N$ とします。
パターン・テキストの長さ、$P$、$V$ は `int` に収まることが必要です。

## テンプレート引数

```cpp
template <int CHAR_SIZE = 26, char BASE = 'a'>
class aho_corasick;
```

`BASE` から連続する `CHAR_SIZE` 種類の文字を扱います。文字は `unsigned char` に変換して解釈します。
登録パターン・テキスト・`next` に渡す文字はすべてこの範囲内にしてください。

**制約**

- `1 <= CHAR_SIZE <= 256`
- `(unsigned char)BASE + CHAR_SIZE <= 256`

```cpp
kyopro::aho_corasick<> lower;             // a-z
kyopro::aho_corasick<10, '0'> digits;     // 0-9
kyopro::aho_corasick<256, '\0'> bytes;   // 全256バイト値
```

ノードごとに文字種数分の配列を持ち、全体の空間は $O(V\sigma+P)$ です。

## コンストラクタ

```cpp
aho_corasick();
explicit aho_corasick(const vector<string>& patterns);
```

空のオートマトン、または `patterns` を順に登録したオートマトンを作ります。
コンストラクタでは `build()` を呼びません。根のノード番号は0です。

**計算量**

- 空の場合: $O(\sigma)$
- 配列を渡す場合: $O(V\sigma+P+L)$（$L$ はパターンの長さの総和）

## insert

```cpp
int ac.insert(const string& pattern);
```

パターンを1個登録し、0から始まる登録順のIDを返します。
重複する文字列にも別のIDを割り当てます。
既存のID・ノード番号は変わりません。

空文字列はテキストの各境界に一致し、長さ $N$ のテキストには $N+1$ 回出現します。

**計算量**

- ならし $O(1+|pattern|\sigma)$

## build

```cpp
void ac.build();
```

現在のパターンからfailure linkと遷移表を構築します。何度でも呼べます。
`next`・`failure`・`match_count`・`match_ids`・テキスト検索の前に必要です。
`insert()` の後は、重複パターン・空文字列の追加であっても再び `build()` を呼んでください。

**計算量**

- $O(V\sigma)$

## size / node_count

```cpp
int ac.size() const;
int ac.node_count() const;
```

`size()` は重複を含む登録パターン数、`node_count()` は根を含むノード数を返します。
空のオートマトンは登録数0・ノード数1です。`build()` 前も利用できます。

**計算量**

- $O(1)$

## terminal

```cpp
int ac.terminal(int id) const;
```

登録ID `id` のパターンに対応する終端ノードを返します。
同じ文字列のIDは同じノードを指し、空文字列の終端は0です。`build()` 前も利用できます。

**制約**

- `0 <= id < size()`

**計算量**

- $O(1)$

## next

```cpp
int ac.next(int state, char c) const;
```

状態 `state` から文字 `c` を1個読んだ後の状態を返します。
状態は「読み込んだ文字列の接尾辞のうち、登録パターンの接頭辞でもある最長のもの」を表します。
先頭から検索するときの初期状態は0です。区切ってテキストを読む場合は、前回の状態を引き継げます。

**制約**

- 最後の `insert()` 後に `build()` 済み
- `0 <= state < node_count()`
- `c` は指定した文字種の範囲内

**計算量**

- $O(1)$

## failure

```cpp
int ac.failure(int state) const;
```

`state` が表す文字列の真の接尾辞のうち、登録パターンの接頭辞でもある最長のもののノードを返します。
根の場合は0を返します。

**制約**

- 最後の `insert()` 後に `build()` 済み
- `0 <= state < node_count()`

**計算量**

- $O(1)$

## match_count / match_ids

```cpp
long long ac.match_count(int state) const;
vector<int> ac.match_ids(int state) const;
```

現在の位置で終わる一致の個数・パターンIDを返します。
状態そのものだけでなくfailure link上の終端を含み、重複ID・空文字列も数えます。
`match_ids()` はパターンの長さの降順、同じ文字列では登録IDの昇順に返します。

**制約**

- 最後の `insert()` 後に `build()` 済み
- `0 <= state < node_count()`

**計算量**

- `match_count`: $O(1)$
- `match_ids`: $O(1+k)$（$k$ は返すID数）

## count

```cpp
vector<long long> ac.count(const string& text) const;
```

各登録パターンの出現回数を、登録ID順の長さ $P$ の配列で返します。
重なった一致も数えます。同じ文字列を登録したIDにはそれぞれ同じ回数を返します。
一致の全列挙を行わないため、出現回数が多い場合にも利用できます。

**制約**

- 最後の `insert()` 後に `build()` 済み
- `text` の各文字は指定した文字種の範囲内

**計算量**

- 時間 $O(N+V+P)$、追加空間 $O(V+P)$

## count_matches

```cpp
long long ac.count_matches(const string& text) const;
```

すべての登録IDに対する出現回数の合計を返します。`count(text)` の要素の総和と等しい値です。

**制約**

- `count()` と同じ

**計算量**

- 時間 $O(N+1)$、追加空間 $O(1)$

## find_all

```cpp
vector<pair<int, int>> ac.find_all(const string& text) const;
```

一致ごとに `{開始位置, パターンID}` を返します。位置は0-indexedです。
そのパターンの長さを $l$ とすると、一致範囲は `[開始位置, 開始位置 + l)` です。

一致の終端位置の昇順、同じ終端位置ではパターンの長さの降順、同じ文字列では登録IDの昇順に並びます。
開始位置順ではありません。空文字列は開始位置0から $N$ までの全境界に一致します。

**制約**

- `count()` と同じ

**計算量**

- 時間 $O(N+K+1)$、返り値の空間 $O(K)$（$K$ は重複を含む全一致数）

## 使用例

```cpp
kyopro::aho_corasick<> ac;
int he = ac.insert("he");
int she = ac.insert("she");
int hers = ac.insert("hers");
ac.build();

auto counts = ac.count("ushers"); // {1, 1, 1}
cout << ac.count_matches("ushers") << '\n'; // 3
for (auto [pos, id] : ac.find_all("ushers")) {
    cout << pos << ' ' << id << '\n'; // {1, she}, {2, he}, {2, hers}
}

int state = 0;
long long total = ac.match_count(state); // 空文字列の先頭境界での一致
for (char c : string("ushers")) {
    state = ac.next(state, c);
    total += ac.match_count(state);
}
cout << total << '\n'; // 3
```
