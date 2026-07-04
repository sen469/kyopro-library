# Doubling

実装: [lib/kyopro/doubling.hpp](../lib/kyopro/doubling.hpp)

関数グラフ上で、同じ遷移を何回も適用した結果をダブリングで求めるライブラリです。
`f^k(v)` の計算や、遷移しながら辺の値を畳み込む処理に使えます。

```cpp
#include "kyopro/doubling.hpp"
```

## doubling

```cpp
class kyopro::doubling;
```

各頂点から次に移動する頂点を 1 つだけ持つグラフで、`k` 回遷移した後の頂点を求めます。
遷移先がない場合は `-1` を使います。

## コンストラクタ

```cpp
doubling();
```

頂点数 0 の空の構造体を作ります。

**計算量**

- $O(1)$

```cpp
explicit doubling(int n, long long max_step = (1LL << 60));
```

頂点数 `n` の構造体を作ります。
この時点では全頂点の遷移先は `-1` です。
`set_next` で遷移を設定してから `build` してください。

**引数**

- `int n`: 頂点数
- `long long max_step`: クエリで指定する最大遷移回数

**制約**

- `0 <= n`
- `0 <= max_step`

**計算量**

- $O(n \log max\_step)$

```cpp
explicit doubling(const vector<int>& next, long long max_step = (1LL << 60));
```

遷移先配列 `next` から構造体を作り、前計算まで行います。
`next[v]` は頂点 `v` から 1 回遷移した先です。
遷移先がない場合は `-1` にします。

**引数**

- `const vector<int>& next`: 遷移先配列
- `long long max_step`: クエリで指定する最大遷移回数

**制約**

- `next[v] == -1` または `0 <= next[v] < next.size()`
- `0 <= max_step`

**計算量**

- $O(n \log max\_step)$

## set_next

```cpp
void db.set_next(int v, int to);
```

頂点 `v` から 1 回遷移した先を `to` に設定します。
設定後にクエリを使う場合は `build` してください。

**引数**

- `int v`: 遷移元の頂点
- `int to`: 遷移先の頂点。遷移先がない場合は `-1`

**制約**

- `0 <= v < n`
- `to == -1` または `0 <= to < n`

**計算量**

- $O(1)$

## build

```cpp
void db.build();
```

設定済みの遷移からダブリングテーブルを前計算します。

**計算量**

- $O(n \log max\_step)$

## jump

```cpp
int db.jump(int v, long long step) const;
int db(int v, long long step) const;
```

頂点 `v` から `step` 回遷移した後の頂点を返します。
途中で遷移先がなくなった場合は `-1` を返します。

**引数**

- `int v`: 開始頂点
- `long long step`: 遷移回数

**戻り値**

- `step` 回遷移した後の頂点
- 途中で遷移先がなくなった場合は `-1`

**制約**

- `0 <= v < n`
- `0 <= step <= max_step`
- `build` 済み

**計算量**

- $O(\log max\_step)$

## next

```cpp
int db.next(int v, int power) const;
```

頂点 `v` から $2^{power}$ 回遷移した後の頂点を返します。

**引数**

- `int v`: 開始頂点
- `int power`: ダブリングテーブルの段数

**制約**

- `0 <= v < n`
- `0 <= power < db.log()`
- `build` 済み

**計算量**

- $O(1)$

## size

```cpp
int db.size() const;
```

頂点数を返します。

**計算量**

- $O(1)$

## log

```cpp
int db.log() const;
```

ダブリングテーブルの段数を返します。

**計算量**

- $O(1)$

## doubling_monoid

```cpp
template <class S, S (*op)(S, S), S (*e)()>
class kyopro::doubling_monoid;
```

遷移先に加えて、各遷移の値を管理します。
`step` 回遷移した後の頂点と、通った遷移の値を順に `op` で畳み込んだ値を返します。
`op` は結合律を満たし、`e()` は単位元を返す必要があります。

## doubling_monoid のコンストラクタ

```cpp
doubling_monoid();
explicit doubling_monoid(int n, long long max_step = (1LL << 60));
doubling_monoid(const vector<int>& next, const vector<S>& value, long long max_step = (1LL << 60));
```

`next[v]` は頂点 `v` から 1 回遷移した先、`value[v]` はその遷移の値です。
`next[v] == -1` の場合、`value[v]` は使われず `e()` として扱います。

**制約**

- `0 <= n`
- `next.size() == value.size()`
- `next[v] == -1` または `0 <= next[v] < next.size()`
- `0 <= max_step`

**計算量**

- 空の構造体: $O(1)$
- それ以外: $O(n \log max\_step)$

## doubling_monoid::set_next

```cpp
void db.set_next(int v, int to, const S& value);
```

頂点 `v` から 1 回遷移した先と、その遷移の値を設定します。
設定後にクエリを使う場合は `build` してください。

**制約**

- `0 <= v < n`
- `to == -1` または `0 <= to < n`

**計算量**

- $O(1)$

## doubling_monoid::build

```cpp
void db.build();
```

設定済みの遷移と値からダブリングテーブルを前計算します。

**計算量**

- $O(n \log max\_step)$

## doubling_monoid::jump

```cpp
pair<int, S> db.jump(int v, long long step) const;
pair<int, S> db(int v, long long step) const;
```

頂点 `v` から `step` 回遷移した後の頂点と、通った遷移の値の畳み込みを返します。
途中で遷移先がなくなった場合、頂点は `-1` になり、値は実際に通れた遷移だけを畳み込みます。

**戻り値**

- `first`: `step` 回遷移した後の頂点。途中で遷移先がなくなった場合は `-1`
- `second`: 通った遷移の値を順に `op` で畳み込んだ値

**制約**

- `0 <= v < n`
- `0 <= step <= max_step`
- `build` 済み

**計算量**

- $O(\log max\_step)$

## doubling_monoid::next

```cpp
int db.next(int v, int power) const;
```

頂点 `v` から $2^{power}$ 回遷移した後の頂点を返します。

**計算量**

- $O(1)$

## doubling_monoid::value

```cpp
S db.value(int v, int power) const;
```

頂点 `v` から最大 $2^{power}$ 回遷移するときに通る遷移の値を畳み込んだ値を返します。
途中で遷移先がなくなる場合は、実際に通れる遷移だけを畳み込みます。

**計算量**

- $O(1)$

## 使用例

```cpp
vector<int> next = {1, 2, 0};
kyopro::doubling db(next);

cout << db.jump(0, 5) << '\n'; // 2
```

重み付き遷移で、通った辺の重みの総和も求める例です。

```cpp
long long op(long long a, long long b) {
    return a + b;
}

long long e() {
    return 0;
}

vector<int> next = {1, 2, -1};
vector<long long> weight = {10, 20, 0};

kyopro::doubling_monoid<long long, op, e> db(next, weight);
auto [to, sum] = db.jump(0, 3);

cout << to << ' ' << sum << '\n'; // -1 30
```
