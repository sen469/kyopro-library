# Trie

実装: [lib/kyopro/trie.hpp](../lib/kyopro/trie.hpp)

文字列集合を Trie 木で管理します。
同じ文字列を複数回挿入できます。

```cpp
#include "kyopro/trie.hpp"
```

## テンプレート引数

```cpp
template <int CHAR_SIZE = 26, char BASE = 'a'>
class trie;
```

- `CHAR_SIZE`: 扱う文字種の数
- `BASE`: 最小の文字

デフォルトでは `'a'` から `'z'` までの 26 文字を扱います。
数字文字列を扱う場合は次のようにします。

```cpp
kyopro::trie<10, '0'> trie;
```

## node

```cpp
struct node {
    array<int, CHAR_SIZE> next;
    int pass_count;
    int end_count;
};
```

**メンバ変数**

- `next[i]`: 文字 `BASE + i` で遷移した先のノード番号。存在しない場合は `-1`
- `pass_count`: このノードを通る文字列の個数。重複も数える
- `end_count`: このノードで終わる文字列の個数。重複も数える

## コンストラクタ

```cpp
trie();
```

空の Trie 木を作ります。
根ノードだけを持ちます。

**計算量**

- $O(CHAR\_SIZE)$

## insert

```cpp
void trie.insert(const string& s);
```

文字列 `s` を 1 個追加します。

**引数**

- `const string& s`: 追加する文字列

**制約**

- `s` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|s| \cdot CHAR\_SIZE)$

新しいノードを作るときに `array` を初期化するため、厳密には上の計算量です。
`CHAR_SIZE` を定数とみなすと $O(|s|)$ です。

## erase

```cpp
bool trie.erase(const string& s);
```

文字列 `s` を 1 個削除します。
`s` が存在しない場合は何も変更しません。

**引数**

- `const string& s`: 削除する文字列

**戻り値**

- 削除できた場合は `true`
- `s` が存在しない場合は `false`

**制約**

- `s` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|s|)$

## count

```cpp
int trie.count(const string& s) const;
```

文字列 `s` が何個挿入されているかを返します。

**引数**

- `const string& s`: 調べる文字列

**戻り値**

- `s` の個数

**制約**

- `s` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|s|)$

## contains

```cpp
bool trie.contains(const string& s) const;
```

文字列 `s` が 1 個以上挿入されているか判定します。

**引数**

- `const string& s`: 調べる文字列

**戻り値**

- `count(s) > 0` なら `true`
- そうでなければ `false`

**制約**

- `s` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|s|)$

## starts_with

```cpp
bool trie.starts_with(const string& prefix) const;
```

`prefix` に対応するノードが存在するか判定します。
`prefix` から始まる文字列が削除済みで 0 個でも、ノードが残っていれば `true` になります。
現在挿入されている文字列数で判定したい場合は `prefix_count(prefix) > 0` を使います。

**引数**

- `const string& prefix`: 調べる接頭辞

**戻り値**

- `prefix` に対応するノードが存在するなら `true`
- 存在しないなら `false`

**制約**

- `prefix` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|prefix|)$

## prefix_count

```cpp
int trie.prefix_count(const string& prefix) const;
```

`prefix` から始まる文字列が何個挿入されているかを返します。
重複も数えます。

**引数**

- `const string& prefix`: 調べる接頭辞

**戻り値**

- `prefix` から始まる文字列の個数

**制約**

- `prefix` の各文字 `c` について `BASE <= c < BASE + CHAR_SIZE`

**計算量**

- $O(|prefix|)$

## size

```cpp
int trie.size() const;
```

挿入されている文字列数を返します。
重複も数えます。

**戻り値**

- Trie 木に入っている文字列数

**計算量**

- $O(1)$

## node_count

```cpp
int trie.node_count() const;
```

Trie 木が持つノード数を返します。
削除してもノードは消さないため、減ることはありません。

**戻り値**

- Trie 木のノード数

**計算量**

- $O(1)$

## operator[]

```cpp
const trie<CHAR_SIZE, BASE>::node& trie.operator[](int i) const;
```

ノード番号 `i` のノードを返します。

**引数**

- `int i`: ノード番号

**戻り値**

- ノード `i` への const 参照

**制約**

- `0 <= i < node_count()`

**計算量**

- $O(1)$

## 使用例

```cpp
kyopro::trie<> trie;
trie.insert("app");
trie.insert("apple");
trie.insert("apply");

cout << trie.contains("app") << '\n';     // 1
cout << trie.count("app") << '\n';        // 1
cout << trie.prefix_count("app") << '\n'; // 3

trie.erase("app");
cout << trie.contains("app") << '\n';     // 0
cout << trie.starts_with("app") << '\n';  // 1
```
