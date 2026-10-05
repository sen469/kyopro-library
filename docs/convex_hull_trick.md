# Convex Hull Trick

実装: [lib/kyopro/convex_hull_trick.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/convex_hull_trick.hpp)

直線 $y = ax + b$ の集合を管理し、指定した $x$ における最小値または最大値を求めます。
直線追加はならし $O(1)$、クエリは $O(\log N)$ です。

**制約**

- 最小値を求める場合、追加する傾き `a` は非増加順。同じ傾きも追加できます。
- 最大値を求める場合、追加する傾き `a` は非減少順。
- クエリの `x` の順序は任意です。追加とクエリを交互に実行できます。
- `T` は64ビット以下の符号付き整数型です。浮動小数点型・符号なし整数型には対応しません。
- `__int128` に対応した GCC / Clang を使用します。

傾きを任意の順序で追加できるデータ構造ではありません。
すべての直線を先に追加する場合は傾きでソートできますが、DP 中の追加順序を自由に
変えられない場合は、この単調性を満たすか確認してください。

```cpp
#include "kyopro/convex_hull_trick.hpp"
```

## コンストラクタ

```cpp
kyopro::convex_hull_trick<T = long long, bool is_min = true> cht;
```

直線を持たない CHT を作ります。`is_min = true` なら最小値、`false` なら最大値を求めます。

```cpp
kyopro::convex_hull_trick<> minimum;
kyopro::convex_hull_trick<long long, false> maximum;
```

**計算量**

- $O(1)$

## add_line

```cpp
void cht.add_line(T a, T b);
```

直線 $y = ax + b$ を追加します。同じ傾きの直線は切片が有利な方だけを保持します。
どの座標でも不要な直線は内部で取り除きます。

**制約**

- 最小値用では、直前に追加した傾きより大きな傾きを追加してはいけません。
- 最大値用では、直前に追加した傾きより小さな傾きを追加してはいけません。
- どちらも等しい傾きは許可します。`clear()` 後は任意の傾きから再開できます。

**計算量**

- ならし $O(1)$。1回の追加で複数の直線を取り除くことがあります。
- 全体の使用メモリは追加回数を $N$ として $O(N)$。

## query

```cpp
T cht.query(T x) const;
```

追加済みのすべての直線に対する $ax+b$ の最小値または最大値を返します。
クエリで内部の直線は削除しません。

**制約**

- `!cht.empty()`
- 答えが `T` の範囲に収まること。候補の直線すべての値が `T` に収まる必要はありません。

**計算量**

- $O(\log(H+1))$。$H$ は保持している直線数。

## query_wide

```cpp
__int128 cht.query_wide(T x) const;
```

`query` と同じ答えを `__int128` で返します。答えが `T` の範囲を超える場合に使用します。
係数と `x` が `T` の範囲内なら、内部の評価と交点比較も整数で正確に処理します。
`__int128` は `std::cout` に直接渡せないため、出力時は変換処理が必要です。

**制約**

- `!cht.empty()`

**計算量**

- $O(\log(H+1))$

## size / empty

```cpp
int cht.size() const;
bool cht.empty() const;
```

`size()` は内部に保持している直線数を返します。不要な直線を除去するため、
`add_line` を呼んだ回数とは異なります。`empty()` は直線がないかどうかを返します。

**計算量**

- $O(1)$

## clear

```cpp
void cht.clear();
```

すべての直線を削除し、傾きの追加順序もリセットします。
内部の配列の確保済み領域は保持します。

**計算量**

- $O(H)$ 以下。

## 使用例

```cpp
#include <cassert>
#include "kyopro/convex_hull_trick.hpp"

int main() {
    kyopro::convex_hull_trick<> cht;
    cht.add_line(3, 2);   // y = 3x + 2
    cht.add_line(1, 0);   // y = x
    cht.add_line(-2, 4);  // y = -2x + 4
    assert(cht.query(2) == 0);
    assert(cht.query(-1) == -1);

    kyopro::convex_hull_trick<long long, false> maximum;
    maximum.add_line(-2, 4);
    maximum.add_line(1, 0);
    maximum.add_line(3, 2);
    assert(maximum.query(2) == 8);
}
```

例えば $dp_i = \min_{j<i}(a_j x_i + dp_j)$ という DP では、
`query(x_i)` で $dp_i$ を求めた後、`add_line(a_i, dp_i)` を実行できます。
初期状態の直線を先に追加し、$a_j$ が非増加順であることを確認してください。
