# Implicit Treap

長さが変化する列を管理します。
列の任意位置への挿入、削除、区間反転、区間作用、区間積の取得をならし $O(\log n)$ で行えます。

```cpp
#include "kyopro/implicit_treap.hpp"
```

## コンストラクタ

```cpp
kyopro::implicit_treap<S, op, e, F, mapping, composition, id> a;
kyopro::implicit_treap<S, op, e, F, mapping, composition, id> a(vector<S> v);
```

- 型 `S`
- 写像の型 `F`
- 二項演算 `S op(S a, S b)`
- 単位元 `S e()`
- 写像 `S mapping(F f, S x)`
- 写像の合成 `F composition(F f, F g)`
- 恒等写像 `F id()`

以下の条件を満たす必要があります。

- `(S, op, e)` はモノイドである
- `mapping(id(), x) == x`
- `mapping(f, op(x, y)) == op(mapping(f, x), mapping(f, y))`
- `mapping(composition(f, g), x) == mapping(f, mapping(g, x))`

`v` が与えられた場合、`a[i] = v[i]` として初期化します。

**制約**

- `op`, `e`, `mapping`, `composition`, `id` は関数である

**計算量**

- `implicit_treap()`: $O(1)$
- `implicit_treap(vector<S> v)`: ならし $O(n \log n)$

## size

```cpp
int a.size();
```

現在の列の長さを返します。

**計算量**

- $O(1)$

## empty

```cpp
bool a.empty();
```

列が空なら `true`、そうでなければ `false` を返します。

**計算量**

- $O(1)$

## get

```cpp
S a.get(int p);
S a[p];
```

`a[p]` を返します。

**制約**

- `0 <= p < a.size()`

**計算量**

- ならし $O(\log n)$

## set

```cpp
void a.set(int p, S x);
```

`a[p] = x` とします。

**制約**

- `0 <= p < a.size()`

**計算量**

- ならし $O(\log n)$

## prod

```cpp
S a.prod(int l, int r);
```

`op(a[l], ..., a[r - 1])` を、モノイドの型 `S` として返します。
`l == r` のときは `e()` を返します。

**制約**

- `0 <= l <= r <= a.size()`

**計算量**

- ならし $O(\log n)$

## all_prod

```cpp
S a.all_prod();
```

全要素の積 `op(a[0], ..., a[n - 1])` を返します。
`n == 0` のときは `e()` を返します。

**計算量**

- $O(1)$

## apply

```cpp
void a.apply(int l, int r, F f);
```

`i = l, ..., r - 1` について `a[i] = mapping(f, a[i])` とします。

**制約**

- `0 <= l <= r <= a.size()`

**計算量**

- ならし $O(\log n)$

## reverse

```cpp
void a.reverse(int l, int r);
```

区間 `[l, r)` の要素の順番を反転します。

**制約**

- `0 <= l <= r <= a.size()`

**計算量**

- ならし $O(\log n)$

## insert

```cpp
void a.insert(int p, S x);
void a.push_front(S x);
void a.push_back(S x);
```

`a.insert(p, x)` は `x` を `a[p]` の直前に挿入します。
`p == a.size()` のときは末尾に挿入します。

`a.push_front(x)` は `a.insert(0, x)` と同じです。
`a.push_back(x)` は `a.insert(a.size(), x)` と同じです。

**制約**

- `0 <= p <= a.size()`

**計算量**

- ならし $O(\log n)$

## erase

```cpp
S a.erase(int p);
void a.erase(int l, int r);
```

`a.erase(p)` は `a[p]` を削除し、削除した値を返します。
`a.erase(l, r)` は区間 `[l, r)` を削除します。

**制約**

- `0 <= p < a.size()`
- `0 <= l <= r <= a.size()`

**計算量**

- `erase(p)`: ならし $O(\log n)$
- `erase(l, r)`: ならし $O(\log n + (r - l))$

## to_vector

```cpp
vector<S> a.to_vector();
```

現在の列を `vector<S>` として返します。

**計算量**

- $O(n)$

## clear

```cpp
void a.clear();
```

列を空にします。

**計算量**

- $O(n)$

## 使用例

区間加算、区間和、挿入、削除、反転を行う例です。

```cpp
#include <bits/stdc++.h>
#include "kyopro/implicit_treap.hpp"

using namespace std;

struct S {
    long long sum;
    int size;
};

S op(S l, S r) {
    return {l.sum + r.sum, l.size + r.size};
}

S e() {
    return {0, 0};
}

struct F {
    long long add;
};

S mapping(F f, S x) {
    return {x.sum + f.add * x.size, x.size};
}

F composition(F f, F g) {
    return {f.add + g.add};
}

F id() {
    return {0};
}

int main() {
    using treap =
        kyopro::implicit_treap<S, op, e, F, mapping, composition, id>;

    vector<S> v = {{1, 1}, {2, 1}, {3, 1}, {4, 1}};
    treap a(v);

    a.apply(1, 3, {10});       // [1, 12, 13, 4]
    cout << a.prod(0, 4).sum << '\n';  // 30

    a.reverse(1, 4);           // [1, 4, 13, 12]
    a.insert(2, {100, 1});     // [1, 4, 100, 13, 12]
    cout << a.erase(3).sum << '\n';    // 13

    for (S x : a.to_vector()) {
        cout << x.sum << ' ';
    }
    cout << '\n';              // 1 4 100 12
}
```
