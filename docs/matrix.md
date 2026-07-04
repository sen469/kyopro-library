# Matrix

実装: [lib/kyopro/matrix.hpp](../lib/kyopro/matrix.hpp)

行列の加算、減算、乗算、累乗を行います。
`long long` や `atcoder::modint` など、`+`, `-`, `*`, `+=`, `-=`, `T()`, `T(1)` が使える型で利用できます。

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

- 列数。空行列なら `0`

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
