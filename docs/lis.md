# LIS

[実装の解説](explanations/lis.md)

実装: [lib/kyopro/lis.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/lis.hpp)

最長増加部分列（Longest Increasing Subsequence）の長さと、その部分列の一例を求めます。
部分列は連続している必要はありませんが、元の配列の添字順を保ちます。
狭義増加・広義増加の切り替えと、比較関数の指定に対応します。

```cpp
#include "kyopro/lis.hpp"

std::vector<int> a = {3, 1, 2, 2, 4};
int length = kyopro::lis_length(a);        // 3
auto result = kyopro::lis(a);
// result.length() == 3
// result.values == {1, 2, 4}
for (int i : result.indices) cout << i << ' ';

auto nonstrict = kyopro::lis(a, false);    // 広義増加
// nonstrict.values == {1, 2, 2, 4}
```

## lis_length

```cpp
template <class T, class Compare = std::less<T>>
int kyopro::lis_length(const vector<T>& a, bool strict = true,
                       Compare compare = Compare());
```

最長増加部分列の長さを返します。入力配列は変更しません。
空配列では `0` を返します。

| 引数 | 意味 |
| --- | --- |
| `a` | 対象の配列 |
| `strict` | `true` なら狭義増加（`<`）、`false` なら広義増加（`<=`） |
| `compare` | 値の順序を表す厳密弱順序。既定は `std::less<T>` |

比較関数を指定した場合、狭義増加では隣接要素 `x, y` が `compare(x, y)`、
広義増加では `!compare(y, x)` を満たす部分列を求めます。

```cpp
// 最長狭義減少部分列
int decreasing = kyopro::lis_length(a, true, std::greater<int>());

// 最長広義減少（非増加）部分列
int nonincreasing = kyopro::lis_length(a, false, std::greater<int>());
```

**制約**

- `a.size()` が `int` に収まる
- `compare` は `T` に対する厳密弱順序

**計算量**

- 時間: $O(n\log(n+1))$
- 追加空間: $O(L)$（`L` は答えの長さ）

## lis / lis_result

```cpp
template <class T>
struct lis_result {
    vector<int> indices;
    vector<T> values;
    int length() const;
};

template <class T, class Compare = std::less<T>>
lis_result<T> kyopro::lis(const vector<T>& a, bool strict = true,
                          Compare compare = Compare());
```

最長増加部分列の一例を復元します。

- `indices`: 復元した要素の元配列での添字。0-indexed の昇順
- `values`: `a[indices[i]]` を順に並べた部分列
- `length()`: 部分列の長さ。$O(1)$

引数の意味と制約は `lis_length` と同じです。加えて `T` のコピーが必要です。
空配列では `indices`・`values` はともに空になります。
解が複数ある場合、どの解を返すかは保証しません。
辞書順最小や、添字列の辞書順最小を保証する関数ではありません。

**計算量**

- 時間: $O(n\log(n+1))$
- 追加空間: $O(n)$

以上の計算量は比較と値のコピーを $O(1)$ としたものです。
数値の番兵は使用しないため、整数型の最小値・最大値や文字列も扱えます。
