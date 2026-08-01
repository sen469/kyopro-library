# DSU

実装: [lib/kyopro/dsu.hpp](../lib/kyopro/dsu.hpp)

無向グラフに対して、

- 辺の追加
- 2 頂点が連結かの判定
- 連結成分ごとの辺数の取得

をならし $O(\alpha(n))$ 時間で処理できます。

ACL の `dsu` に近い API です。
`merge(a, b)` は辺 `(a, b)` を 1 本追加する操作として扱われ、各連結成分の辺数を管理します。

```cpp
#include "kyopro/dsu.hpp"
```

## コンストラクタ

```cpp
dsu d();
```

頂点数 0 の DSU を作ります。

**計算量**

- $O(1)$

```cpp
explicit dsu(int n);
```

`n` 頂点 0 辺の無向グラフを作ります。

**引数**

- `int n`: 頂点数

**制約**

- `0 <= n`

**計算量**

- $O(n)$

## merge

```cpp
int d.merge(int a, int b);
```

辺 `(a, b)` を追加します。
`a`, `b` が連結だった場合はその代表元、非連結だった場合は新たな代表元を返します。

同じ辺を複数回追加した場合、`edge_count` ではすべて数え、`unique_edge_count` では初回だけ数えます。
自己ループ `(a, a)` も辺として数えます。

**引数**

- `int a`: 辺の片端
- `int b`: 辺のもう片端

**制約**

- `0 <= a < n`
- `0 <= b < n`

**計算量**

- ならし $O(\alpha(n))$
- 重複なし辺の記録に使うハッシュテーブルの期待 $O(1)$

## same

```cpp
bool d.same(int a, int b);
```

頂点 `a`, `b` が連結かどうかを返します。

**引数**

- `int a`: 頂点
- `int b`: 頂点

**制約**

- `0 <= a < n`
- `0 <= b < n`

**計算量**

- ならし $O(\alpha(n))$

## leader

```cpp
int d.leader(int a);
```

頂点 `a` の属する連結成分の代表元を返します。

**引数**

- `int a`: 頂点

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## size

```cpp
int d.size(int a);
```

頂点 `a` の属する連結成分の頂点数を返します。

**引数**

- `int a`: 頂点

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## edge_count

```cpp
long long d.edge_count(int a);
```

頂点 `a` の属する連結成分に追加された辺数を返します。
同じ無向辺を複数回追加した場合も、その回数だけ数えます。

**引数**

- `int a`: 頂点

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## unique_edge_count

```cpp
long long d.unique_edge_count(int a);
```

頂点 `a` の属する連結成分に追加された重複なしの辺数を返します。
無向辺 `(u, v)` と `(v, u)` は同じ辺として扱います。
自己ループ `(u, u)` も 1 種類の辺として扱います。

**引数**

- `int a`: 頂点

**制約**

- `0 <= a < n`

**計算量**

- ならし $O(\alpha(n))$

## groups

```cpp
vector<vector<int>> d.groups();
```

グラフを連結成分に分け、その情報を返します。
返り値は「1 つの連結成分の頂点番号のリスト」のリストです。
`vector` 内でどの順番で頂点が格納されているかは未定義です。

**計算量**

- $O(n)$

## 使用例

頂点 `r` を含む連結成分が、重複辺を含めても木かどうかは `edge_count(r) == size(r) - 1` で判定できます。
重複を無視した単純グラフとして判定したい場合は `unique_edge_count(r) == size(r) - 1` を使います。

```cpp
kyopro::dsu d(4);
d.merge(0, 1);
d.merge(1, 2);

int r = 0;
cout << d.size(r) << '\n';              // 3
cout << d.edge_count(r) << '\n';        // 2
cout << d.unique_edge_count(r) << '\n'; // 2

bool is_tree = d.edge_count(r) == d.size(r) - 1;
cout << is_tree << '\n'; // 1

d.merge(0, 1);
cout << d.edge_count(r) << '\n';        // 3
cout << d.unique_edge_count(r) << '\n'; // 2
```
