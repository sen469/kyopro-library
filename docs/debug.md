# デバッグ出力

```cpp
#include "debug/all"
```

`dg(...)` は標準エラーへ行番号・引数名・型名・値を色付きで出力します。
標準コンテナ、`pair`・`tuple`、`__int128`、`interval_heap` などを表示できます。

## 構造体の登録

構造体に `DG_FIELDS(...)` を追加すると、指定したメンバを名前付きで表示できます。

```cpp
#include "debug/struct.hpp" // debug/all からも使えます

struct S {
    long long sum;
    int length;
    DG_FIELDS(sum, length)
};

S x{10, 3};
dg(x); // 値の表示: {sum: 10, length: 3}

std::vector<S> a = {{10, 3}, {2, 1}};
dg(a); // {{sum: 10, length: 3}, {sum: 2, length: 1}}
```

登録した順に出力し、省略したメンバは表示しません。`DG_FIELDS()` なら `{}` です。
構造体の入れ子やテンプレート型にも対応します。
`cout << x` など、`operator<<` でも同じ表示を利用できます。

```cpp
struct TreeNode {
    S data;
    std::vector<S> children;
    DG_FIELDS(data, children)
};
```

`class` では `DG_FIELDS` を `public:` の位置に置いてください。
そこから private メンバを登録することもできます。

**条件**

- C++17 以降を使用する
- 登録する各メンバがデバッグ出力に対応している
- 登録するのは参照を取得できるメンバ。ビットフィールドや一時値を返す式は指定できない

`DG_FIELDS` は `debug_fields()` と `debug_field_names()` を生成します。
これらの名前を持つメンバは別途定義しないでください。
`val()` も持つ型では、登録したメンバの表示を優先します。
なお、`dg()` 自体は従来どおり引数をコピーするため、コピーできないオブジェクトは
直接 `operator<<` で表示してください。

## 必要な出力だけを include する

| ヘッダ | 内容 |
| --- | --- |
| `debug/debug.hpp` | 基本型・標準コンテナ・`__int128` など |
| `debug/struct.hpp` | 登録した構造体。基本のヘッダも含む |
| `debug/interval_heap.hpp` | `interval_heap`。基本のヘッダも含む |
| `debug/all` | 上記すべて |
