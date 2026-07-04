# Compress

実装: [lib/kyopro/compress.hpp](../lib/kyopro/compress.hpp)

座標圧縮を行います。
値を昇順に並べて重複を除き、各値を `0` 以上の添字に変換します。

```cpp
#include "kyopro/compress.hpp"
```

## compress_result

```cpp
template <class T>
struct compress_result {
    vector<int> compressed;
    vector<T> values;
};
```

**メンバ変数**

- `vector<int> compressed`: 元配列の各要素を圧縮した添字列
- `vector<T> values`: 圧縮に使った重複なし昇順の値列

## compress_result::size

```cpp
int res.size() const;
```

圧縮後の座標数を返します。

**戻り値**

- `values.size()`

**計算量**

- $O(1)$

## compress

```cpp
template <class T>
compress_result<T> compress(const vector<T>& a);
```

配列 `a` を座標圧縮します。

**引数**

- `const vector<T>& a`: 圧縮する配列

**戻り値**

- `compressed` と `values` を持つ `compress_result<T>`
- `compressed[i]` は `a[i]` の圧縮後の値
- `values[j]` は圧縮後の値 `j` に対応する元の値

**制約**

- `T` は `operator<` が使える
- `a.size()` が `int` に収まる

**計算量**

- $O(n \log n)$

## compressor

```cpp
template <class T>
class compressor;
```

座標集合を保持する型です。
後から値の位置を調べたり、元の値へ復元したりできます。

## コンストラクタ

```cpp
compressor();
```

空の座標集合を作ります。
この状態ではまだ `build()` されていません。

**計算量**

- $O(1)$

```cpp
explicit compressor(const vector<T>& values);
```

`values` から座標集合を作り、重複なし昇順に整列します。

**引数**

- `const vector<T>& values`: 座標集合の候補

**制約**

- `T` は `operator<` が使える

**計算量**

- $O(n \log n)$

```cpp
template <class Iterator>
compressor(Iterator first, Iterator last);
```

半開区間 `[first, last)` から座標集合を作り、重複なし昇順に整列します。

**引数**

- `Iterator first`: 先頭 iterator
- `Iterator last`: 末尾 iterator

**制約**

- iterator の値型は `T`
- `T` は `operator<` が使える

**計算量**

- $O(n \log n)$

## add

```cpp
void comp.add(const T& x);
```

座標集合の候補に値 `x` を追加します。
追加後に検索系の操作を使う場合は `build()` してください。

**引数**

- `const T& x`: 追加する値

**計算量**

- 償却 $O(1)$

## build

```cpp
void comp.build();
```

追加済みの値を重複なし昇順に整列し、検索できる状態にします。

**制約**

- `T` は `operator<` が使える

**計算量**

- $O(n \log n)$

## size

```cpp
int comp.size() const;
```

座標数を返します。

**戻り値**

- 重複なしの座標数

**計算量**

- $O(1)$

## empty

```cpp
bool comp.empty() const;
```

座標集合が空か判定します。

**戻り値**

- 空なら `true`
- そうでなければ `false`

**計算量**

- $O(1)$

## get / lower_bound

```cpp
int comp.get(const T& x) const;
int comp.lower_bound(const T& x) const;
```

`x` 以上の最初の位置を返します。
`lower_bound(x)` は `get(x)` と同じです。

**引数**

- `const T& x`: 調べる値

**戻り値**

- `values()[i] >= x` となる最小の `i`
- 存在しない場合は `size()`

**制約**

- `build()` 済み
- `T` は `operator<` が使える

**計算量**

- $O(\log n)$

## upper_bound

```cpp
int comp.upper_bound(const T& x) const;
```

`x` より大きい最初の位置を返します。

**引数**

- `const T& x`: 調べる値

**戻り値**

- `values()[i] > x` となる最小の `i`
- 存在しない場合は `size()`

**制約**

- `build()` 済み
- `T` は `operator<` が使える

**計算量**

- $O(\log n)$

## contains

```cpp
bool comp.contains(const T& x) const;
```

`x` が座標集合に含まれるか判定します。

**引数**

- `const T& x`: 調べる値

**戻り値**

- 含まれるなら `true`
- そうでなければ `false`

**制約**

- `build()` 済み
- `T` は `operator<`, `operator==` が使える

**計算量**

- $O(\log n)$

## operator[]

```cpp
const T& comp.operator[](int i) const;
```

圧縮後の値 `i` に対応する元の値を返します。

**引数**

- `int i`: 圧縮後の値

**戻り値**

- `values()[i]` への const 参照

**制約**

- `build()` 済み
- `0 <= i < size()`

**計算量**

- $O(1)$

## values

```cpp
const vector<T>& comp.values() const;
```

圧縮に使う重複なし昇順の値列を返します。

**戻り値**

- 座標集合への const 参照

**制約**

- `build()` 済み

**計算量**

- $O(1)$

## 使用例

```cpp
vector<long long> x = {10000000000LL, -3, 4, 4};
auto res = kyopro::compress(x);

for (int id : res.compressed) {
    cout << id << ' ';
}
cout << '\n'; // 2 0 1 1
```
