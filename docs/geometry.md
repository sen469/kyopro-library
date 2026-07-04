# Geometry

実装: [lib/kyopro/geometry.hpp](../lib/kyopro/geometry.hpp)

2次元幾何で使う点・直線・円と、公差 `eps` 付きの判定関数集です。

```cpp
#include "kyopro/geometry.hpp"
```

すべて `kyopro::geometry` 名前空間にあります。
座標型は `long double` です。

## EPS 比較

```cpp
constexpr long double default_eps = 1e-10L;

int sign(long double x, long double eps = default_eps);
bool equal(long double a, long double b, long double eps = default_eps);
bool lt(long double a, long double b, long double eps = default_eps);
bool le(long double a, long double b, long double eps = default_eps);
bool gt(long double a, long double b, long double eps = default_eps);
bool ge(long double a, long double b, long double eps = default_eps);
```

- `sign(x)` は `x > eps` なら `1`、`x < -eps` なら `-1`、それ以外なら `0` を返します。
- ほかの比較関数も `eps` を考慮します。

## point

```cpp
struct point {
    long double x, y;
};
```

`+`, `-`, `*`, `/` が使えます。

```cpp
long double dot(point a, point b);
long double cross(point a, point b);
long double norm2(point p);
long double norm(point p);
long double distance(point a, point b);
bool same_point(point a, point b, long double eps = default_eps);
point rotate90(point p);
point rotate(point p, long double rad);
```

## line

```cpp
struct line {
    point a, b;
};
```

2点 `a`, `b` を通る直線です。

```cpp
point direction(line l);
bool parallel(line l1, line l2, long double eps = default_eps);
bool orthogonal(line l1, line l2, long double eps = default_eps);
bool on_line(line l, point p, long double eps = default_eps);
bool same_line(line l1, line l2, long double eps = default_eps);
int line_line_relation(line l1, line l2, long double eps = default_eps);
bool on_segment(point a, point b, point p, long double eps = default_eps);
long double distance_line_point(line l, point p);
long double distance_segment_point(point a, point b, point p);
bool intersect_segments(point a, point b, point c, point d, long double eps = default_eps);
```

- `on_line` は点が直線上にあるかを判定します。
- `same_line` は2直線が同じ直線かを判定します。
- `line_line_relation` は2直線の位置関係を返します。
- `on_segment` は点が線分上にあるかを判定します。
- `intersect_segments` は2線分が交差するかを判定します。端点接触も交差に含みます。

`line_line_relation` は次を返します。

- `0`: 平行で交点なし
- `1`: 同じ直線
- `2`: 1点で交わる

## circle

```cpp
struct circle {
    point center;
    long double r;
};
```

```cpp
bool same_circle(circle c1, circle c2, long double eps = default_eps);
bool on_circle(circle c, point p, long double eps = default_eps);
int circle_position(circle c, point p, long double eps = default_eps);
bool in_circle(circle c, point p, bool strict = false, long double eps = default_eps);
int line_circle_relation(line l, circle c, long double eps = default_eps);
int segment_circle_relation(point a, point b, circle c, long double eps = default_eps);
int circle_circle_relation(circle c1, circle c2, long double eps = default_eps);
```

`circle_position(c, p)` は次を返します。

- `-1`: 円の内部
- `0`: 円周上
- `1`: 円の外部

`line_circle_relation` と `segment_circle_relation` は次を返します。

- `0`: 交点なし
- `1`: 接する
- `2`: 2点で交わる

`circle_circle_relation` は次を返します。

- `0`: 離れている
- `1`: 外接
- `2`: 2点で交わる
- `3`: 内接
- `4`: 一方が他方の内部にあり、接しない
- `5`: 同じ円

## 使用例

```cpp
using namespace kyopro::geometry;

line l(point(0, 0), point(2, 2));
cout << on_line(l, point(1, 1)) << '\n'; // 1

circle c(point(0, 0), 5);
cout << on_circle(c, point(3, 4)) << '\n'; // 1
cout << circle_position(c, point(1, 1)) << '\n'; // -1
```
