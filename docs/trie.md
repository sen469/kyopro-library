# Trie

文字列集合を Trie 木で管理します。
同じ文字列を複数回挿入できます。

```cpp
#include "kyopro/trie.hpp"
```

## trie

```cpp
kyopro::trie<> trie;
trie.insert("apple");
trie.insert("app");
```

デフォルトでは `'a'` から `'z'` までの 26 文字を扱います。
文字種を変える場合は `kyopro::trie<文字数, 先頭文字>` のように指定します。

```cpp
kyopro::trie<10, '0'> trie;
trie.insert("0123");
```

## 操作

```cpp
trie.insert(s);
bool ok = trie.erase(s);
int c = trie.count(s);
bool exists = trie.contains(s);
bool has_prefix = trie.starts_with(prefix);
int pc = trie.prefix_count(prefix);
```

- `insert(s)`: 文字列 `s` を 1 個追加します。
- `erase(s)`: 文字列 `s` を 1 個削除します。存在しない場合は `false` を返します。
- `count(s)`: 文字列 `s` の個数を返します。
- `contains(s)`: 文字列 `s` が 1 個以上あるか判定します。
- `starts_with(prefix)`: `prefix` から始まるノードが存在するか判定します。
- `prefix_count(prefix)`: `prefix` から始まる文字列の個数を返します。
- `size()`: 挿入されている文字列数を返します。重複も数えます。
- `node_count()`: Trie 木のノード数を返します。

**計算量**

- 各操作: $O(|s|)$

## 使用例

```cpp
kyopro::trie<> trie;
trie.insert("app");
trie.insert("apple");
trie.insert("apply");

cout << trie.contains("app") << '\n';    // 1
cout << trie.count("app") << '\n';       // 1
cout << trie.prefix_count("app") << '\n'; // 3

trie.erase("app");
cout << trie.contains("app") << '\n';    // 0
cout << trie.starts_with("app") << '\n'; // 1
```
