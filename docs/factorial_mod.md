# Factorial Mod

実装: [lib/kyopro/factorial_mod.hpp](../lib/kyopro/factorial_mod.hpp)

素数 mod で `n! mod p` を求めます。
`n < p` のとき、`0` 側または `p - 1` 側の近い方から計算します。
`n >= p` の場合は `n!` が `p` で割り切れるため `0` を返します。

```cpp
#include "kyopro/factorial_mod.hpp"
```

## factorial_mod_prime

```cpp
int x = kyopro::factorial_mod_prime<998244353>(n);
```

`MOD` は素数である必要があります。
`n` は `0 <= n` です。

Wilson の定理 `(p - 1)! = -1 mod p` を使うため、`n` が `p - 1` に近い場合も高速です。

**計算量**

- `n < p`: $O(\min(n, p - 1 - n) + \log p)$
- `n >= p`: $O(1)$

## 実行時 mod

```cpp
int x = kyopro::factorial_mod_prime(n, mod);
```

`mod` が入力で与えられる場合に使います。
こちらも `mod` は素数である必要があります。

**計算量**

- `n < mod`: $O(\min(n, mod - 1 - n) + \log mod)$
- `n >= mod`: $O(1)$

## batch

```cpp
vector<int> ns1 = {10, 20, 30};
auto ans1 = kyopro::factorial_mod_prime_batch<MOD>(ns1);

vector<long long> ns2 = {10, MOD - 1, MOD};
auto ans2 = kyopro::factorial_mod_prime_batch<MOD>(ns2);
```

複数の `n! mod p` をまとめて求めます。
クエリをソートし、`0` 側と `p - 1` 側からの積を共有します。
マルチケースでは単発関数を繰り返すより高速です。

実行時 mod 版もあります。

```cpp
auto ans = kyopro::factorial_mod_prime_batch(ns, mod);
```

**計算量**

- `O(q log q + L + R + q log p)`
ここで `q` はクエリ数、`L` は `0` 側から計算するクエリの最大 `n`、`R` は `p - 1` 側から計算するクエリの最大 `p - 1 - n` です。

## 問題
- [Factorial](https://judge.yosupo.jp/problem/factorial)
- [Many Factorials](https://judge.yosupo.jp/problem/many_factorials)

## 使用例

```cpp
constexpr int MOD = 998244353;

cout << kyopro::factorial_mod_prime<MOD>(10) << '\n'; // 3628800
cout << kyopro::factorial_mod_prime<MOD>(MOD - 1) << '\n'; // 998244352
cout << kyopro::factorial_mod_prime<MOD>(MOD) << '\n'; // 0
```

`n` が `p / 2` 付近の場合は、どちら側から計算しても線形に近い時間がかかります。
多くの小さい `n` に対して繰り返し使う場合は `binomial` の `fact` の方が向いています。
