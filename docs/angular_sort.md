# Angular Sort

[実装の解説](explanations/angular_sort.md)

実装: [lib/kyopro/angular_sort.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/angular_sort.hpp)

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

第 2 引数に非零の方向ベクトル `start` を渡すと、その方向を開始位置にできます。
`start` はソートの中心を移動する座標ではなく、原点から見た開始方向です。
ソート対象に同じ方向の点がなくても指定できます。

```cpp
kyopro::angular_sort(p, {0, 1});
// p = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}}

kyopro::angular_sort(p, {-1, -1}); // 負の対角方向から反時計回り
```

既定では開始方向と同じ偏角の点を最初にし、そこから反時計回りに一周します。
原点は既定では開始方向によらず先頭で、同じ偏角の点は原点から近い順です。
`start` の長さは順序に影響しません。

```cpp
template <class T>
void kyopro::angular_sort(vector<pair<T, T>>& points,
                         pair<T, T> start = {T(1), T(0)});
```

**制約**

- `start != (0, 0)`
- 整数型では、点どうし・`start` と点の外積、および点の平方ノルムが符号付き 128 ビットに収まる
- 浮動小数点型では座標・開始方向が有限で、計算途中にオーバーフローしない

**計算量**

- $O(n \log n)$

## angular_sort_options

開始方向の端点の扱い、回転方向、原点の位置は設定オブジェクトで指定できます。
既存の `angular_sort(p)` と `angular_sort(p, {sx, sy})` は従来どおり使えます。

```cpp
kyopro::angular_sort_options<long long> options;
options.start = {-1, 0};
options.include_start = false;
options.origin = kyopro::angular_origin::direction;
kyopro::angular_sort(p, options);
```

この設定は偏角を `(-pi, pi]`、原点の偏角を `0` としてソートする問題に対応します。
負の x 軸方向の点は最後、原点は正の x 軸方向の点と同じ偏角として扱います。

| 設定 | 既定値 | 意味 |
| --- | --- | --- |
| `start` | `{1, 0}` | 原点から見た開始方向。非零ベクトル |
| `include_start` | `true` | `true` は開始方向の点を先頭側、`false` は末尾側に置く |
| `clockwise` | `false` | `false` は反時計回り、`true` は時計回り |
| `origin` | `angular_origin::first` | 原点の扱い。下記の3通り |
| `origin_direction` | `{1, 0}` | `origin == direction` の場合に原点へ割り当てる方向 |

`include_start = false` でも点は削除されません。指定方向からの回転角を
`[0, 2pi)` とするか `(0, 2pi]` とするかの違いです。
開始方向に点が存在しない場合、この設定による差はありません。

原点の扱いは次から選びます。

- `angular_origin::first`: 常に先頭
- `angular_origin::last`: 常に末尾
- `angular_origin::direction`: `origin_direction` と同じ偏角として扱う

`origin_direction` は開始方向からの相対角ではなく、実際の平面上の方向です。
既定値 `{1, 0}` は、開始方向によらず「原点の偏角を 0」とする定義に対応します。
任意の非零方向も指定できます。同じ偏角の中では実際の距離で比較するため、
原点はその方向の非零の点より先になります。

### よく使う設定

```cpp
kyopro::angular_sort_options<long long> options;
options.origin = kyopro::angular_origin::direction; // 原点の偏角は 0

// [0, 2pi)
kyopro::angular_sort(p, options);

// (-pi, pi]
options.start = {-1, 0};
options.include_start = false;
kyopro::angular_sort(p, options);

// [-pi, pi)
options.include_start = true;
kyopro::angular_sort(p, options);

// 正の y 軸から時計回り、開始方向の点は末尾、原点は常に末尾
options.start = {0, 1};
options.clockwise = true;
options.include_start = false;
options.origin = kyopro::angular_origin::last;
kyopro::angular_sort(p, options);
```

**制約**

- `start != (0, 0)`
- `origin == angular_origin::direction` のとき `origin_direction != (0, 0)`
- 計算に使う外積（`start`・`origin_direction` も含む）と点の平方ノルムが計算型に収まる
- `T` は点の座標と同じ型

整数では符号付き 128 ビット、浮動小数点では `long double` で計算します。
計算量は設定によらず $O(n\log n)$ です。

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

`angular_sort` と同じように、第 2 引数で開始方向を指定できます。

```cpp
auto order_from_up = kyopro::angular_sorted_indices(p, {0, 1});
// order_from_up = {3, 2, 0, 1}
```

開始方向の既定値・制約・原点の扱いは `angular_sort` と同じです。
同じ座標の点どうしの添字順は保証しません。

設定オブジェクトも同じように渡せます。

```cpp
auto order = kyopro::angular_sorted_indices(p, options);
```

**計算量**

- $O(n \log n)$

## comparator

```cpp
sort(p.begin(), p.end(), kyopro::polar_angle_less<long long>());
sort(p.begin(), p.end(), kyopro::polar_angle_less<long long>({1, 1}));
sort(p.begin(), p.end(), kyopro::polar_angle_less<long long>(options));
```

`polar_angle_less<T>` を直接 `sort` の比較関数として使えます。
コンストラクタに開始方向を渡せます。省略した場合は `(1, 0)` です。
`angular_sort_options<T>` を渡すと、端点・回転方向・原点の扱いも設定できます。
