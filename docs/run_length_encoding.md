# Run Length Encoding

実装: [lib/kyopro/run_length_encoding.hpp](../lib/kyopro/run_length_encoding.hpp)

連続する同じ値を `(値, 個数)` にまとめるランレングス圧縮です。
文字列、`vector`、iterator 範囲に対して使えます。

```cpp
#include "kyopro/run_length_encoding.hpp"
```

## run_length_encoding

```cpp
template <class InputIt>
vector<pair<typename iterator_traits<InputIt>::value_type, int>>
run_length_encoding(InputIt first, InputIt last);
```

半開区間 `[first, last)` の隣り合う等しい要素をまとめます。

**引数**

- `InputIt first`: 先頭 iterator
- `InputIt last`: 末尾 iterator

**戻り値**

- `vector<pair<T, int>>`
- 各要素は `{値, 連続個数}`
- `first == last` の場合は空の `vector`

**制約**

- `InputIt` は `iterator_traits<InputIt>::value_type` を持つ
- 要素同士を `operator==` で比較できる
- 連続個数が `int` に収まる

**計算量**

- $O(n)$

```cpp
template <class Container>
auto run_length_encoding(const Container& c);
```

コンテナ全体に対してランレングス圧縮します。

**引数**

- `const Container& c`: 圧縮するコンテナ

**戻り値**

- `run_length_encoding(begin(c), end(c))` と同じ

**制約**

- `std::begin(c)`, `std::end(c)` が使える
- 要素同士を `operator==` で比較できる
- 連続個数が `int` に収まる

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
