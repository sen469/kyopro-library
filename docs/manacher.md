# Manacher

実装: [lib/kyopro/manacher.hpp](../lib/kyopro/manacher.hpp)

文字列や `vector` の各中心について、最長回文の半径を $O(n)$ で求めます。

```cpp
#include "kyopro/manacher.hpp"
```

## manacher

```cpp
template <class Container>
vector<int> manacher(const Container& s);
```

奇数長回文の半径を返します。
`res[i] = k` のとき、`s[i - k + 1] ... s[i + k - 1]` が中心 `i` の最長奇数長回文です。
回文長は `2 * res[i] - 1` です。

**引数**

- `const Container& s`: 調べる列

**戻り値**

- 各中心の奇数長回文半径を持つ `vector<int>`

**制約**

- `s.size()` が使える
- `s[i]` が使える
- 要素同士を `operator==` で比較できる
- `s.size()` が `int` に収まる

**計算量**

- $O(n)$

## manacher_even

```cpp
template <class Container>
vector<int> manacher_even(const Container& s);
```

偶数長回文の半径を返します。
`res[i] = k` のとき、`s[i - k] ... s[i + k - 1]` が `i - 1` と `i` の間を中心とする最長偶数長回文です。
回文長は `2 * res[i]` です。

**引数**

- `const Container& s`: 調べる列

**戻り値**

- 各中心の偶数長回文半径を持つ `vector<int>`

**制約**

- `s.size()` が使える
- `s[i]` が使える
- 要素同士を `operator==` で比較できる
- `s.size()` が `int` に収まる

**計算量**

- $O(n)$

## palindrome_radii_result

```cpp
struct palindrome_radii_result {
    vector<int> odd;
    vector<int> even;
};
```

**メンバ変数**

- `vector<int> odd`: 奇数長回文半径。`manacher(s)` の結果
- `vector<int> even`: 偶数長回文半径。`manacher_even(s)` の結果

## is_palindrome

```cpp
bool radii.is_palindrome(int l, int r) const;
```

半開区間 `[l, r)` が回文か判定します。

**引数**

- `int l`: 区間の左端
- `int r`: 区間の右端

**戻り値**

- `[l, r)` が回文なら `true`
- そうでなければ `false`

**制約**

- `0 <= l <= r <= odd.size()`
- `odd` と `even` は同じ長さ

**計算量**

- $O(1)$

## palindrome_radii

```cpp
template <class Container>
palindrome_radii_result palindrome_radii(const Container& s);
```

奇数長・偶数長の回文半径をまとめて計算します。

**引数**

- `const Container& s`: 調べる列

**戻り値**

- `odd` と `even` を持つ `palindrome_radii_result`

**制約**

- `s.size()` が使える
- `s[i]` が使える
- 要素同士を `operator==` で比較できる
- `s.size()` が `int` に収まる

**計算量**

- $O(n)$

## 使用例

```cpp
string s = "abacaba";
auto radii = kyopro::palindrome_radii(s);

cout << radii.odd[3] << '\n'; // 4
cout << radii.is_palindrome(0, 7) << '\n'; // 1
```
