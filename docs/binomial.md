# Binomial

二項係数を計算するライブラリです。
素数 mod 用と合成数 mod 対応版があります。

```cpp
#include "kyopro/binomial.hpp"
```

## コンストラクタ

```cpp
kyopro::binomial<MOD> binom;
kyopro::binomial<MOD> binom(max_n);
kyopro::dynamic_binomial binom(mod);
kyopro::dynamic_binomial binom(mod, max_n);
kyopro::arbitrary_mod_binomial<MOD> binom;
kyopro::arbitrary_mod_binomial<MOD> binom(max_n);
kyopro::dynamic_arbitrary_mod_binomial binom(mod);
kyopro::dynamic_arbitrary_mod_binomial binom(mod, max_n);
```

- `MOD` は素数である必要があります。
- `MOD` がコンパイル時定数の場合は `binomial<MOD>` を使います。
- 問題から `mod` が入力で与えられる場合は `dynamic_binomial` を使います。
- `dynamic_binomial` の `mod` も素数である必要があります。
- 合成数 mod の場合は `arbitrary_mod_binomial<MOD>` または `dynamic_arbitrary_mod_binomial` を使います。
- `max_n` を指定すると `0` から `max_n` まで前計算します。
- `max_n` を指定しない場合は、必要になったところまで自動で前計算します。

**制約**

- `binomial<MOD>` と `dynamic_binomial`: `0 <= n < mod`
- `arbitrary_mod_binomial<MOD>` と `dynamic_arbitrary_mod_binomial`: `0 <= n`

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

- `binomial<MOD>` と `dynamic_binomial`: `0 <= n < mod`
- `arbitrary_mod_binomial<MOD>` と `dynamic_arbitrary_mod_binomial`: `0 <= n`

**計算量**

- 前計算済みなら `O(1)`

## perm

```cpp
int binom.perm(int n, int k);
```

`nPk mod MOD` を返します。
`k < 0` または `n < k` の場合は `0` を返します。

**制約**

- `binomial<MOD>` と `dynamic_binomial`: `0 <= n < mod`
- `arbitrary_mod_binomial<MOD>` と `dynamic_arbitrary_mod_binomial`: `0 <= n`

**計算量**

- 前計算済みなら `O(1)`

## fact

```cpp
int binom.fact(int n);
```

`n! mod MOD` を返します。

**制約**

- `0 <= n`

**計算量**

- 前計算済みなら `O(1)`

## inv_fact

```cpp
int binom.inv_fact(int n);
```

`(n!)^{-1} mod MOD` を返します。
素数 mod 用の `binomial<MOD>` と `dynamic_binomial` だけで使えます。

**制約**

- `0 <= n < mod`

**計算量**

- 前計算済みなら `O(1)`

## 入力で mod が与えられる場合

```cpp
int mod;
cin >> mod;

kyopro::dynamic_binomial binom(mod);
cout << binom.comb(n, k) << '\n';
```

テンプレート引数には実行時に入力された変数を渡せないため、`binomial<mod>` とは書けません。
法が問題から与えられる場合は `dynamic_binomial` を使ってください。

## 合成数 mod の場合

```cpp
kyopro::arbitrary_mod_binomial<12> binom;
cout << binom.comb(n, k) << '\n';
```

`MOD` がコンパイル時定数でない場合は `dynamic_arbitrary_mod_binomial` を使います。

```cpp
int mod;
cin >> mod;

kyopro::dynamic_arbitrary_mod_binomial binom(mod);
cout << binom.comb(n, k) << '\n';
```

合成数 mod では階乗の逆元が存在しないことがあるため、`arbitrary_mod_binomial` と `dynamic_arbitrary_mod_binomial` は `inv_fact` を提供しません。
内部では mod を素因数冪に分解し、各素因数冪で計算した値を CRT で復元します。

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
