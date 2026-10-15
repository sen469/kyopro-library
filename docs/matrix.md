# Matrix

[実装の解説](explanations/matrix.md)

実装: [lib/kyopro/matrix.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/matrix.hpp)

行列の加算、減算、乗算、累乗、行列式、逆行列、ランク、一次連立方程式の解を求めます。
`long long` や `atcoder::modint` など、`+`, `-`, `*`, `+=`, `-=`, `T()`, `T(1)` が使える型で利用できます。
行列式・逆行列・ランク・連立方程式には、非零要素で正確に除算できる体上の型が必要です。
素数を法とする `atcoder::modint` を利用できます。これら4関数では整数型・浮動小数点型を利用できません。

```cpp
#include "kyopro/matrix.hpp"
```

## matrix

```cpp
template <class T>
class matrix;
```

`matrix<T>` は内部で `vector<vector<T>>` を保持します。

## コンストラクタ

```cpp
matrix();
```

空の行列を作ります。

**計算量**

- $O(1)$

```cpp
matrix(int h, int w, T value = T());
```

高さ `h`、幅 `w` の行列を作り、すべての要素を `value` で初期化します。

**引数**

- `int h`: 行数
- `int w`: 列数
- `T value`: 初期値

**制約**

- `0 <= h`
- `0 <= w`

**計算量**

- $O(hw)$

```cpp
explicit matrix(const vector<vector<T>>& data);
```

二次元配列 `data` から行列を作ります。

**引数**

- `const vector<vector<T>>& data`: 初期値

**制約**

- `data` の各行の長さがすべて等しい

**計算量**

- $O(hw)$

## height

```cpp
int a.height() const;
```

行列の高さを返します。

**戻り値**

- 行数

**計算量**

- $O(1)$

## width

```cpp
int a.width() const;
```

行列の幅を返します。

**戻り値**

- 列数。`matrix(h, w)` で作った場合は、0行でも `w` を返します。
- デフォルト構築・空の二次元配列から構築した場合は `0`

**計算量**

- $O(1)$

## empty

```cpp
bool a.empty() const;
```

行数が 0 か判定します。

**戻り値**

- `height() == 0` なら `true`
- そうでなければ `false`

**計算量**

- $O(1)$

## operator[]

```cpp
vector<T>& a.operator[](int i);
const vector<T>& a.operator[](int i) const;
```

`i` 行目を返します。
`a[i][j]` で要素にアクセスできます。

**引数**

- `int i`: 行番号

**戻り値**

- `i` 行目への参照

**制約**

- `0 <= i < height()`

**計算量**

- $O(1)$

## data

```cpp
const vector<vector<T>>& a.data() const;
```

内部の二次元配列を返します。

**戻り値**

- 内部データへの const 参照

**計算量**

- $O(1)$

## identity

```cpp
static matrix<T> matrix<T>::identity(int n);
```

`n * n` の単位行列を返します。

**引数**

- `int n`: 行列サイズ

**戻り値**

- `n * n` の単位行列

**制約**

- `0 <= n`
- `T(1)` が使える

**計算量**

- $O(n^2)$

## operator+= / operator-=

```cpp
matrix<T>& a.operator+=(const matrix<T>& rhs);
matrix<T>& a.operator-=(const matrix<T>& rhs);
```

行列の加算・減算を行います。

**引数**

- `const matrix<T>& rhs`: 右辺の行列

**戻り値**

- 更新後の `*this`

**制約**

- `a.height() == rhs.height()`
- `a.width() == rhs.width()`
- `T` は `operator+=` または `operator-=` が使える

**計算量**

- $O(hw)$

## operator*=

```cpp
matrix<T>& a.operator*=(const matrix<T>& rhs);
```

行列の乗算を行い、結果を `a` に代入します。

**引数**

- `const matrix<T>& rhs`: 右辺の行列

**戻り値**

- 更新後の `*this`

**制約**

- `a.width() == rhs.height()`
- `T` は `operator+`, `operator*`, `operator+=` が使える

**計算量**

- $O(hwk)$

`a` が `h * w`、`rhs` が `w * k` の場合です。

## operator+ / operator- / operator*

```cpp
template <class T>
matrix<T> operator+(matrix<T> lhs, const matrix<T>& rhs);

template <class T>
matrix<T> operator-(matrix<T> lhs, const matrix<T>& rhs);

template <class T>
matrix<T> operator*(const matrix<T>& lhs, const matrix<T>& rhs);
```

行列の加算、減算、乗算の結果を返します。

**引数**

- `matrix<T> lhs`: 左辺の行列
- `const matrix<T>& rhs`: 右辺の行列

**戻り値**

- 演算結果の行列

**制約**

- 加算・減算では `lhs.height() == rhs.height()`
- 加算・減算では `lhs.width() == rhs.width()`
- 乗算では `lhs.width() == rhs.height()`
- `T` は必要な算術演算が使える

**計算量**

- 加算・減算: $O(hw)$
- 乗算: $O(hwk)$

## pow

```cpp
matrix<T> a.pow(long long e) const;
```

正方行列 `a` の `e` 乗を二分累乗で求めます。
`e = 0` の場合は単位行列を返します。

**引数**

- `long long e`: 指数

**戻り値**

- `a^e`

**制約**

- `0 <= e`
- `a.height() == a.width()`
- `T` は `operator+`, `operator*`, `operator+=`, `T(1)` が使える

**計算量**

- $O(n^3 \log e)$

## matrix_power

```cpp
template <class T>
matrix<T> matrix_power(const matrix<T>& a, long long e);
```

`a.pow(e)` と同じです。

**引数**

- `const matrix<T>& a`: 底の行列
- `long long e`: 指数

**戻り値**

- `a^e`

**制約**

- `0 <= e`
- `a.height() == a.width()`

**計算量**

- $O(n^3 \log e)$

## determinant

```cpp
T a.determinant() const;
```

正方行列 `a` の行列式を返します。`a` は変更しません。
特異行列では `T(0)`、0行0列の行列では `T(1)` を返します。

**制約**

- `a.height() == a.width()`
- `T` は厳密な体上の型であること
- `T(0)`, `T(1)`, `==`, `-`, `*`, `/`, `*=`, `-=` が使えること

**計算量**

- 時間 $O(n^3)$、追加空間 $O(n^2)$（$n$ は行数）

## inverse

```cpp
std::optional<matrix<T>> a.inverse() const;
```

正方行列 `a` の逆行列を返します。`a` は変更しません。
逆行列が存在しない場合は `std::nullopt` を返します。
0行0列の行列では、0行0列の行列を含む `optional` を返します。

**制約**

- `a.height() == a.width()`
- `T` の制約は `determinant()` と同じ

**計算量**

- 時間 $O(n^3)$、追加空間 $O(n^2)$（$n$ は行数）

## rank

```cpp
int a.rank() const;
```

行列 `a` のランクを返します。長方形の行列も扱えます。`a` は変更しません。
0行・0列の行列や零行列のランクは0です。

**制約**

- `T` の制約は `determinant()` と同じ

**計算量**

- 時間 $O(hw\min(h,w))$、追加空間 $O(hw)$
- $h$ は行数、$w$ は列数

上記の計算量は要素型の演算を $O(1)$ としたものです。
各関数はピボットごとに1回、合計で高々 $\min(h,w)$ 回の除算を行います。

## solve

```cpp
template <class T>
struct linear_equation_result {
    bool exists;
    vector<T> solution;
    vector<vector<T>> basis;
};

linear_equation_result<T> a.solve(const vector<T>& b) const;
```

一次連立方程式 $Ax=b$ を解きます。長方形の行列も扱えます。
`a` と `b` は変更しません。

**戻り値**

- `exists`: 解が存在するなら `true`、存在しないなら `false`
- `solution`: 自由変数をすべて0にした特殊解。長さは列数 `w`
- `basis`: 同次方程式 $Ax=0$ の解空間の基底。各ベクトルの長さは `w`

解が存在しない場合、`solution` と `basis` は空配列です。
解が存在する場合、`basis.size()` は $w-\operatorname{rank}(A)$ です。
`basis` が空なら解は一意です。

一般解は、任意の $c_i\in T$ を使って次のように表せます。

$$
x=\text{solution}+\sum_{i=0}^{d-1}c_i\,\text{basis}[i]
$$

$d$ は基底の本数です。基底は自由変数の列番号の昇順に並び、対応する自由変数を1、
それ以外の自由変数を0にしたベクトルを返します。

0行 `w` 列なら、特殊解は零ベクトル、基底は `w` 本の標準基底です。
0列なら、`b` がすべて0の場合に限って解が存在し、特殊解・基底はともに空配列です。

**制約**

- `b.size() == a.height()`
- `T` の制約は `determinant()` と同じ

**計算量**

- 時間 $O(hw\min(h,w)+h+w+wd)$
- 返り値を含む追加空間 $O(hw+h+w+wd)$
- $h$ は行数、$w$ は列数、$d=w-\operatorname{rank}(A)$
- 要素型の演算を $O(1)$ とした計算量。除算はピボットごとに1回

基底を明示的に返すため、零行列などでは出力だけで $O(w^2)$ の時間と空間が必要です。

## modintでの除算

ピボットの逆元は `T(1) / pivot` で求めます。
`atcoder::modint` の `/` は内部で `inv()` を呼ぶため、逆元を手動で渡す必要はありません。

- `modint998244353`・`modint1000000007` など、素数を法とする型を利用できます。
- `dynamic_modint` は素数の法を設定し、行列作成後から計算終了まで法を変更しないでください。
- 合成数を法とする型は対象外です。非零でも逆元を持たない要素があり、行列全体が可逆でも途中のピボットで除算できない場合があります。
- ランク・行列式・可逆性は、その法の下での値です。有理数上のランクなどとは異なる場合があります。

整数型は除算で切り捨てが起こり、浮動小数点型は零判定に誤差処理が必要になるため、これら4関数ではコンパイル時に拒否します。
加減乗算・累乗では引き続き使用できます。

## 問題

- [Pow of Matrix](https://judge.yosupo.jp/submission/374671)

## 使用例

```cpp
kyopro::matrix<long long> a({
    {1, 1},
    {1, 0},
});

auto b = a.pow(10);
cout << b[0][1] << '\n'; // 55
```

```cpp
#include <atcoder/modint>
#include "kyopro/matrix.hpp"

using mint = atcoder::modint998244353;
kyopro::matrix<mint> a({{1, 2}, {3, 4}});

cout << a.determinant().val() << '\n'; // 998244351 (= -2)
cout << a.rank() << '\n'; // 2

auto inv = a.inverse();
if (inv) {
    auto identity = a * *inv;
    cout << identity[0][0].val() << '\n'; // 1
}
```

```cpp
using mint = atcoder::modint998244353;
kyopro::matrix<mint> a({{1, 2, 3}, {0, 1, 1}});
vector<mint> b = {4, 1};

auto result = a.solve(b);
if (!result.exists) {
    cout << "No solution\n";
} else {
    // solution = {2, 1, 0}
    // basis = {{-1, -1, 1}}（mod 998244353）
    for (auto x : result.solution) cout << x.val() << ' ';
    cout << '\n';
}
```
