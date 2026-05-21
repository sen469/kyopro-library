# Sieve

エラトステネスの篩です。
素数判定、素数列挙、最小素因数、素因数分解、約数列挙ができます。

```cpp
#include "kyopro/sieve.hpp"
```

## コンストラクタ

```cpp
kyopro::sieve<> sv(n);
kyopro::sieve<long long> sv_ll(n);
```

`0` 以上 `n` 以下の整数について前計算します。
テンプレート引数 `T` は `factorize` や `divisors` の値の型です。省略した場合は `int` になります。

**計算量**

- $O(n)$

## is_prime

```cpp
bool sv.is_prime(T x);
```

`x` が素数なら `true` を返します。
`x <= sv.max_n()` の場合は篩の結果を使い、それより大きい場合は Miller-Rabin で判定します。

**制約**

- `0 <= x`

**計算量**

- `x <= sv.max_n()` の場合: $O(1)$
- それ以外の場合: $O(\log x)$ 程度

## primes

```cpp
const vector<int>& ps = sv.primes();
```

`sv.max_n()` 以下の素数を昇順で返します。

**計算量**

- $O(1)$

## spf

```cpp
int p = sv.spf(T x);
```

`x` の最小素因数を `int` で返します。

**制約**

- `2 <= x <= sv.max_n()`

**計算量**

- $O(1)$

## factorize

```cpp
auto f = sv.factorize(x);
```

`x` を素因数分解し、`vector<pair<T, int>>` で `(素因数, 指数)` を昇順に返します。
`x == 1` の場合は空の `vector` を返します。
`x <= sv.max_n()` の場合は最小素因数テーブルを使い、それより大きい場合は Miller-Rabin と Pollard Rho で分解します。

**制約**

- `1 <= x`
- `x` は `uint64_t` に収まる正の整数

**計算量**

- `x <= sv.max_n()` の場合: $O(\log x)$
- それ以外の場合: 確率的に高速

## divisors

```cpp
auto ds = sv.divisors(x);
```

`x` の正の約数を `vector<T>` で昇順に返します。
内部で `factorize(x)` を使います。

**制約**

- `1 <= x`
- `x` は `uint64_t` に収まる正の整数

**計算量**

- 約数の個数を `d` として、`factorize(x)` の計算量 + $O(d \log d)$

## 使用例

```cpp
kyopro::sieve<long long> sv(1000000);

if (sv.is_prime(n)) {
    cout << "prime" << '\n';
}

for (auto [p, e] : sv.factorize(n)) {
    cout << p << ' ' << e << '\n';
}
```
