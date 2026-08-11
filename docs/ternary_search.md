# Ternary Search

実装: [lib/kyopro/ternary_search.hpp](../lib/kyopro/ternary_search.hpp)

整数の半開区間 `[first, last)` 上で単峰な関数の最適値を取る位置を三分探索します。
既定では最小値を探索し、同じ最適値を取る整数が複数あれば最も左を返します。

```cpp
#include "kyopro/ternary_search.hpp"
```

## ternary_search

```cpp
template <class Int, class Function, class Compare = std::less<>>
Int kyopro::ternary_search(Int first, Int last, Function function,
                           Compare compare = Compare());
```

`compare(function(x), function(y))` が真なら、`x` の方が `y` より良いものとして
最適な整数を返します。既定の `std::less<>` では最小値を探索します。

```cpp
int x = kyopro::ternary_search(-100, 101, [](int x) {
    return (x - 7) * (x - 7);
});
// x == 7

int y = kyopro::ternary_search(0, 100, [](int x) {
    return -(x - 40) * (x - 40);
}, std::greater<>());
// y == 40
```

**引数**

- `Int first`: 探索区間の左端（含む）
- `Int last`: 探索区間の右端（含まない）
- `Function function`: 整数を受け取り比較可能な評価値を返す関数
- `Compare compare`: 第 1 引数の方が第 2 引数より良いとき真を返す比較関数

**戻り値**

- 最適値を取る最も左の整数

**制約**

- `Int` は `bool` 以外の整数型
- `first < last`
- `function` は `compare` の意味で単峰

**計算量**

- `function` の呼び出しを $O(\log(last - first))$ 回
