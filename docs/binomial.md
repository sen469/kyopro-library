# Binomial

実装: [lib/kyopro/binomial.hpp](../lib/kyopro/binomial.hpp)

二項係数を計算するライブラリです。
素数 mod 用と合成数 mod 対応版があります。

```cpp
#include "kyopro/binomial.hpp"
```

## コンストラクタ

```cpp
kyopro::binomial<MOD> binom;
kyopro::binomial<MOD> binom(max_n);
kyopro::binomial<MOD, T> binom;
kyopro::binomial<MOD, T> binom(max_n);
kyopro::dynamic_binomial binom(mod);
kyopro::dynamic_binomial binom(mod, max_n);
kyopro::dynamic_binomial<T> binom(mod);
kyopro::dynamic_binomial<T> binom(mod, max_n);
kyopro::arbitrary_mod_binomial<MOD> binom;
kyopro::arbitrary_mod_binomial<MOD> binom(max_n);
kyopro::arbitrary_mod_binomial<MOD, T> binom;
kyopro::arbitrary_mod_binomial<MOD, T> binom(max_n);
kyopro::dynamic_arbitrary_mod_binomial binom(mod);
kyopro::dynamic_arbitrary_mod_binomial binom(mod, max_n);
kyopro::dynamic_arbitrary_mod_binomial<T> binom(mod);
kyopro::dynamic_arbitrary_mod_binomial<T> binom(mod, max_n);
```

- 素数 mod で `MOD` がコンパイル時定数の場合は `binomial<MOD>` を使います。
- 問題から `mod` が入力で与えられる場合は `dynamic_binomial` を使います。
- `binomial<MOD>` の `MOD` と `dynamic_binomial` の `mod` は素数である必要があります。
- 合成数 mod の場合は `arbitrary_mod_binomial<MOD>` または `dynamic_arbitrary_mod_binomial` を使います。
- `T` は `fact`, `inv_fact`, `perm`, `comb`, `operator()` の戻り値型です。省略した場合は `int` です。
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
T binom.comb(int n, int k);
T binom(int n, int k);
T binom.comb(long long n, long long k); // arbitrary_mod_binomial 系
T binom(long long n, long long k);      // arbitrary_mod_binomial 系
```

`nCk mod MOD` を返します。
`k < 0` または `n < k` の場合は `0` を返します。
戻り値型 `T` を省略した場合は `int` を返します。

**制約**

- `binomial<MOD>` と `dynamic_binomial`: `0 <= n < mod`
- `arbitrary_mod_binomial<MOD>` と `dynamic_arbitrary_mod_binomial`: `0 <= n`
- `T` は `T(int)` で剰余値から構築できる

**計算量**

- 前計算済みなら `O(1)`

## perm

```cpp
T binom.perm(int n, int k);
T binom.perm(long long n, long long k); // arbitrary_mod_binomial 系
```

`nPk mod MOD` を返します。
`k < 0` または `n < k` の場合は `0` を返します。
戻り値型 `T` を省略した場合は `int` を返します。

**制約**

- `binomial<MOD>` と `dynamic_binomial`: `0 <= n < mod`
- `arbitrary_mod_binomial<MOD>` と `dynamic_arbitrary_mod_binomial`: `0 <= n`
- `T` は `T(int)` で剰余値から構築できる

**計算量**

- 前計算済みなら `O(1)`

## fact

```cpp
T binom.fact(int n);
```

`n! mod MOD` を返します。
戻り値型 `T` を省略した場合は `int` を返します。

**制約**

- `0 <= n`
- `T` は `T(int)` で剰余値から構築できる

**計算量**

- 前計算済みなら `O(1)`

## inv_fact

```cpp
T binom.inv_fact(int n);
```

`(n!)^{-1} mod MOD` を返します。
素数 mod 用の `binomial<MOD>` と `dynamic_binomial` だけで使えます。
戻り値型 `T` を省略した場合は `int` を返します。

**制約**

- `0 <= n < mod`
- `T` は `T(int)` で剰余値から構築できる

**計算量**

- 前計算済みなら `O(1)`

## 戻り値型を指定する場合

```cpp
using mint = atcoder::modint998244353;

kyopro::binomial<998244353, mint> binom_mint;
mint x = binom_mint.comb(n, k);

kyopro::binomial<998244353, long long> binom_ll;
long long y = binom_ll.comb(n, k);
```

第 2 テンプレート引数に戻り値型を指定できます。
`fact`, `inv_fact`, `perm`, `comb`, `operator()` がその型を返します。

実行時 mod の `atcoder::modint` を使う場合は、先に `set_mod` してください。

```cpp
int mod;
cin >> mod;

atcoder::modint::set_mod(mod);
kyopro::dynamic_binomial<atcoder::modint> binom(mod);
atcoder::modint x = binom.comb(n, k);
```

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

[問題例(ABC021 D-多重ループ)](https://atcoder.jp/contests/abc021/submissions/75963254)

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
