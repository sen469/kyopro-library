# Mo's Algorithm

[実装の解説](explanations/mo.md)

実装: [lib/kyopro/mo.hpp](https://github.com/sen469/kyopro-library/blob/main/lib/kyopro/mo.hpp)

静的配列に対するオフライン区間クエリを、Mo のアルゴリズムで並べ替えて処理します。
現在の区間に要素を 1 個追加・削除する処理を用意することで、各クエリの答えを求められます。

```cpp
#include "kyopro/mo.hpp"
```

すべての区間は半開区間 `[left, right)` です。

## コンストラクタ

```cpp
mo();
```

要素数 0 の Mo 構造体を作ります。

**計算量**

- $O(1)$

```cpp
explicit mo(int n);
```

要素数 `n` の Mo 構造体を作ります。

**引数**

- `int n`: 対象となる配列の要素数

**制約**

- `0 <= n`

**計算量**

- $O(1)$

## add_query

```cpp
int mo.add_query(int left, int right);
```

半開区間 `[left, right)` に対するクエリを追加し、そのクエリ ID を返します。
クエリ ID は 0 から追加順に割り当てられます。
空区間も追加できます。

**引数**

- `int left`: 区間の左端
- `int right`: 区間の右端

**制約**

- `0 <= left <= right <= mo.element_count()`

**計算量**

- 償却 $O(1)$

## element_count

```cpp
int mo.element_count() const;
```

対象となる配列の要素数を返します。

**計算量**

- $O(1)$

## query_count

```cpp
int mo.query_count() const;
```

追加されたクエリ数を返します。

**計算量**

- $O(1)$

## run

```cpp
template <class Add, class Erase, class Answer>
void mo.run(Add&& add, Erase&& erase, Answer&& answer) const;
```

追加されたクエリを Mo の順序で処理します。
現在の区間は最初に空区間 `[0, 0)` です。

各コールバックは次の形式で呼び出されます。

```cpp
void add(int index);
void erase(int index);
void answer(int query_id);
```

- `add(index)`: 現在の区間に添字 `index` の要素を追加する
- `erase(index)`: 現在の区間から添字 `index` の要素を削除する
- `answer(query_id)`: 現在の状態からクエリ `query_id` の答えを保存する

`answer` はクエリ ID 順ではなく、内部で並べ替えた順に呼ばれます。
答えを元の順序で得るには、`answer[query_id]` のようにクエリ ID を添字として保存してください。

このオーバーロードは、区間の左側と右側で追加・削除処理が同じ場合に使います。

**制約**

- `add`, `erase`, `answer` を上記の形式で呼び出せる
- 各コールバックを呼ぶたびに、対応する要素の追加・削除・回答が正しく行われる

**計算量**

`n = mo.element_count()`、`q = mo.query_count()` とします。

- クエリの整列: $O(q \log q)$
- `add` と `erase` の呼び出し回数: 合計 $O(n\sqrt{q} + q)$
- `answer` の呼び出し回数: $q$ 回
- 追加領域: $O(q)$

各コールバックが $O(1)$ なら、全体で $O(q \log q + n\sqrt{q} + q)$ です。

## 左右で処理を分ける run

```cpp
template <
    class AddLeft,
    class AddRight,
    class EraseLeft,
    class EraseRight,
    class Answer
>
void mo.run(
    AddLeft&& add_left,
    AddRight&& add_right,
    EraseLeft&& erase_left,
    EraseRight&& erase_right,
    Answer&& answer
) const;
```

区間の左側と右側で異なる追加・削除処理を指定します。
要素の順序が重要な状態を管理するときに使います。

各コールバックは次の形式で呼び出されます。

```cpp
void add_left(int index);
void add_right(int index);
void erase_left(int index);
void erase_right(int index);
void answer(int query_id);
```

呼び出し時点での区間を `[left, right)` とすると、各操作の意味は次のとおりです。

- `add_left(left - 1)`: 左側へ要素を追加し、区間を `[left - 1, right)` にする
- `add_right(right)`: 右側へ要素を追加し、区間を `[left, right + 1)` にする
- `erase_left(left)`: 左端の要素を削除し、区間を `[left + 1, right)` にする
- `erase_right(right - 1)`: 右端の要素を削除し、区間を `[left, right - 1)` にする
- `answer(query_id)`: 現在の状態からクエリ `query_id` の答えを保存する

**制約**

- 各コールバックを上記の形式で呼び出せる
- 各コールバックを呼ぶたびに、対応する要素の追加・削除・回答が正しく行われる

**計算量**

左右共通の `run` と同じです。

## 使用例
- 転倒数クエリ[https://judge.yosupo.jp/submission/406461]

各区間に含まれる異なる値の個数を求めます。

```cpp
vector<int> a = {1, 2, 1, 3, 2};
vector<pair<int, int>> ranges = {
    {0, 3},
    {1, 5},
};

kyopro::mo mo(a.size());
for (auto [left, right] : ranges) {
    mo.add_query(left, right);
}

vector<int> frequency(4);
vector<int> answers(ranges.size());
int distinct = 0;

auto add = [&](int index) {
    if (frequency[a[index]]++ == 0) distinct++;
};

auto erase = [&](int index) {
    if (--frequency[a[index]] == 0) distinct--;
};

mo.run(add, erase, [&](int query_id) {
    answers[query_id] = distinct;
});

// answers == vector<int>({2, 3})
```
