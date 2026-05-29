# Angular Sort

点を偏角の昇順にソートします。
`atan2` は使わず、半平面と外積で比較します。

```cpp
#include "kyopro/angular_sort.hpp"
```

## angular_sort

```cpp
vector<pair<long long, long long>> p = {
    {0, 1}, {1, 0}, {-1, 0}, {0, -1},
};

kyopro::angular_sort(p);
```

正の x 軸方向から反時計回りに並びます。
同じ偏角の点は原点から近い順に並びます。
原点 `(0, 0)` は角度を持たないため、先頭に並びます。

```cpp
// p = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
```

**計算量**

- $O(n \log n)$

## angular_sorted_indices

```cpp
vector<pair<int, int>> p = {
    {0, -1}, {1, 0}, {-1, 0}, {0, 1},
};

auto order = kyopro::angular_sorted_indices(p);
```

元の配列を変更せず、偏角順の添字列を返します。

```cpp
// order = {1, 3, 2, 0}
```

**計算量**

- $O(n \log n)$

## comparator

```cpp
sort(p.begin(), p.end(), kyopro::polar_angle_less<long long>());
```

`polar_angle_less<T>` を直接 `sort` の比較関数として使えます。
