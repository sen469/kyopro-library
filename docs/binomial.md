# Binomial

素数 mod で二項係数を計算するライブラリです。
階乗と逆階乗を前計算し、`nCk mod MOD` を高速に返します。

```cpp
#include "kyopro/binomial.hpp"
```

## コンストラクタ

```cpp
kyopro::binomial<MOD> binom;
kyopro::binomial<MOD> binom(max_n);
```

- `MOD` は素数である必要があります。
- `max_n` を指定すると `0` から `max_n` まで前計算します。
- `max_n` を指定しない場合は、必要になったところまで自動で前計算します。

**制約**

- `0 <= n < MOD`

**計算量**

- 構築: `O(max_n)`
- 自動拡張: 追加分を `m` として `O(m + log MOD)`

## comb

```cpp
int binom.comb(int n, int k);
int binom(int n, int k);
```

`nCk mod MOD` を返します。
`k < 0` または `n < k` の場合は `0` を返します。

**制約**

- `0 <= n < MOD`

**計算量**

- 前計算済みなら `O(1)`

## perm

```cpp
int binom.perm(int n, int k);
```

`nPk mod MOD` を返します。
`k < 0` または `n < k` の場合は `0` を返します。

**制約**

- `0 <= n < MOD`

**計算量**

- 前計算済みなら `O(1)`

## fact

```cpp
int binom.fact(int n);
```

`n! mod MOD` を返します。

**制約**

- `0 <= n < MOD`

**計算量**

- 前計算済みなら `O(1)`

## inv_fact

```cpp
int binom.inv_fact(int n);
```

`(n!)^{-1} mod MOD` を返します。

**制約**

- `0 <= n < MOD`

**計算量**

- 前計算済みなら `O(1)`

[問題例(ABC021 D-多重ループ)] (https://atcoder.jp/contests/abc021/submissions/75963254)

```cpp
#include <bits/stdc++.h>
using namespace std;:
#include "kyopro/binomial.hpp"
using namespace kyopro;

int main()
{
    int n, k;
    cin >> n >> k;
    binomial<1000000007> binom(n);
    cout << binom.comb(n + k - 1, k) << endl;
    return (0);
}
```
