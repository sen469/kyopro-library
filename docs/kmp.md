# KMP

実装: [lib/kyopro/kmp.hpp](../lib/kyopro/kmp.hpp)

KMP（Knuth–Morris–Pratt）法により、文字列や配列からパターンを検索します。
要素を `operator==` で比較できれば、`string` 以外のコンテナにも使えます。

```cpp
#include "kyopro/kmp.hpp"
```

## prefix_function

```cpp
template <class Container>
vector<int> prefix_function(const Container& pattern);
```

`prefix[i]` が `pattern[0, i + 1)` の真の接頭辞と接尾辞に共通する最大の長さになる配列を返します。
真の接頭辞とは、列自身を除く接頭辞です。

**引数**

- `const Container& pattern`: prefix function を求める列

**戻り値**

- 長さ `pattern.size()` の prefix function

**制約**

- `pattern.size()` と `pattern[i]` が使える
- 要素同士を `operator==` で比較できる
- `pattern.size()` が `int` に収まる

**計算量**

- $O(m)$

## kmp_search

```cpp
template <class Text, class Pattern>
vector<int> kmp_search(const Text& text, const Pattern& pattern);
```

`text` に `pattern` が出現するすべての開始位置を昇順で返します。
重なっている出現も返します。

`pattern` が空の場合は、空列が一致するすべての境界 `0, 1, ..., text.size()` を返します。

**引数**

- `const Text& text`: 検索対象の列
- `const Pattern& pattern`: 検索するパターン

**戻り値**

- `pattern` と一致する部分列の開始位置を持つ `vector<int>`

**制約**

- `text.size()`, `pattern.size()`, `text[i]`, `pattern[i]` が使える
- `text` と `pattern` の要素を `operator==` で比較できる
- `text.size()` と `pattern.size()` が `int` に収まる

**計算量**

- $O(n + m)$

返り値の構築に使う領域を除く追加領域は $O(m)$ です。

## kmp_find

```cpp
template <class Text, class Pattern>
int kmp_find(const Text& text, const Pattern& pattern);
```

`text` に `pattern` が最初に出現する位置を返します。
存在しなければ `-1` を返します。
`pattern` が空の場合は `0` を返します。

全出現位置を保存しないため、最初の一致を見つけた時点で検索を終了します。

**引数**

- `const Text& text`: 検索対象の列
- `const Pattern& pattern`: 検索するパターン

**戻り値**

- 最初に一致する部分列の開始位置
- 一致する部分列が存在しない場合は `-1`

**制約**

- `text.size()`, `pattern.size()`, `text[i]`, `pattern[i]` が使える
- `text` と `pattern` の要素を `operator==` で比較できる
- `text.size()` と `pattern.size()` が `int` に収まる

**計算量**

- 最悪 $O(n + m)$

追加領域は $O(m)$ です。

## 使用例

```cpp
string text = "abracadabra";
string pattern = "abra";

auto positions = kyopro::kmp_search(text, pattern);
// positions == vector<int>({0, 7})

cout << kyopro::kmp_find(text, pattern) << '\n'; // 0
```

重なった一致も取得できます。

```cpp
auto positions = kyopro::kmp_search(string("aaaaa"), string("aaa"));
// positions == vector<int>({0, 1, 2})
```
