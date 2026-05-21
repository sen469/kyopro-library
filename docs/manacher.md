# Manacher

文字列や `vector` の各中心について、最長回文の半径を $O(n)$ で求めます。

```cpp
#include "kyopro/manacher.hpp"
```

## manacher

```cpp
auto odd = kyopro::manacher(s);
```

奇数長回文の半径を返します。
`odd[i] = k` のとき、`s[i - k + 1] ... s[i + k - 1]` が中心 `i` の最長奇数長回文です。
回文長は `2 * odd[i] - 1` です。

**計算量**

- $O(n)$

## manacher_even

```cpp
auto even = kyopro::manacher_even(s);
```

偶数長回文の半径を返します。
`even[i] = k` のとき、`s[i - k] ... s[i + k - 1]` が `i - 1` と `i` の間を中心とする最長偶数長回文です。
回文長は `2 * even[i]` です。

**計算量**

- $O(n)$

## palindrome_radii

```cpp
auto radii = kyopro::palindrome_radii(s);
bool ok = radii.is_palindrome(l, r);
```

`odd` と `even` をまとめて計算します。
`is_palindrome(l, r)` は半開区間 `[l, r)` が回文なら `true` を返します。

**計算量**

- 構築: $O(n)$
- `is_palindrome`: $O(1)$

## 使用例

```cpp
string s = "abacaba";
auto radii = kyopro::palindrome_radii(s);

cout << radii.odd[3] << '\n'; // 4
cout << radii.is_palindrome(0, 7) << '\n'; // 1
```
