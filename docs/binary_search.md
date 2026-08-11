# Binary Search

実装: [lib/kyopro/binary_search.hpp](../lib/kyopro/binary_search.hpp)

整数の半開区間 `[first, last)` 上で、単調な判定を二分探索します。
判定が `false` から `true` に切り替わる最初の整数を返します。

```cpp
#include "kyopro/binary_search.hpp"
```

## binary_search

```cpp
template <class Int, class Predicate>
Int kyopro::binary_search(Int first, Int last, Predicate pred);
```

`pred(x)` が真になる最初の `x` を返します。
区間内に該当する値がなければ `last` を返します。
空区間では `pred` を呼ばずに `last` を返します。

```cpp
long long x = kyopro::binary_search(0LL, 1000000001LL,
                                    [](long long x) {
    return x * x >= 1000000000LL;
});
// x == 31623
```

**引数**

- `Int first`: 探索区間の左端（含む）
- `Int last`: 探索区間の右端（含まない）
- `Predicate pred`: 整数を受け取り `bool` に変換できる値を返す関数

**戻り値**

- `pred(x)` が真になる最小の `x`
- 存在しない場合は `last`

**制約**

- `Int` は `bool` 以外の整数型
- `first <= last`
- `[first, last)` で `pred(x)` は `false`、`true` の順に単調

**計算量**

- `pred` の呼び出しを $O(\log(last - first))$ 回

端点の番兵を用意するめぐる式と同様に、答えが存在しない場合も右端
`last` を番兵として扱えます。`last` 自体に対して `pred` は呼ばれません。
