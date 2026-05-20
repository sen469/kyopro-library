# Run Length Encoding

連続する同じ値を `(値, 個数)` にまとめるランレングス圧縮です。
文字列、`vector`、iterator 範囲に対して使えます。

```cpp
#include "kyopro/run_length_encoding.hpp"
```

## run_length_encoding

```cpp
auto rle = kyopro::run_length_encoding(s);
auto rle = kyopro::run_length_encoding(first, last);
```

隣り合う等しい要素をまとめた `vector<pair<T, int>>` を返します。
空の場合は空の `vector` を返します。

**計算量**

- $O(n)$

## 使用例

```cpp
string s = "aaabbcaa";
auto rle = kyopro::run_length_encoding(s);

// rle = {{'a', 3}, {'b', 2}, {'c', 1}, {'a', 2}}
for (auto [c, len] : rle) {
    cout << c << ' ' << len << '\n';
}
```
