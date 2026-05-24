# Compress

座標圧縮を行います。
値を昇順に並べて重複を除き、各値を `0` 以上の添字に変換します。

```cpp
#include "kyopro/compress.hpp"
```

## compress

```cpp
vector<int> a = {10, -5, 10, 7};
auto res = kyopro::compress(a);
```

`res.compressed` に圧縮後の配列が入ります。
`res.values[i]` で圧縮後の値 `i` から元の値へ戻せます。

```cpp
// res.compressed = {2, 0, 2, 1}
// res.values = {-5, 7, 10}
```

**計算量**

- $O(n \log n)$

## compressor

```cpp
kyopro::compressor<long long> comp(xs);

int id = comp.get(x);
long long original = comp[id];
```

`compressor<T>` は座標集合を保持する型です。
後から値の位置を調べたり、元の値へ復元したりできます。

- `get(x)`: `x` 以上の最初の位置を返します。
- `lower_bound(x)`: `get(x)` と同じです。
- `upper_bound(x)`: `x` より大きい最初の位置を返します。
- `contains(x)`: `x` が座標集合に含まれるなら `true` を返します。
- `values()`: 圧縮に使った重複なし昇順の値列を返します。

**計算量**

- 構築: $O(n \log n)$
- クエリ: $O(\log n)$

## 使用例

```cpp
vector<long long> x = {10000000000LL, -3, 4, 4};
auto res = kyopro::compress(x);

for (int id : res.compressed) {
    cout << id << ' ';
}
cout << '\n'; // 2 0 1 1
```
