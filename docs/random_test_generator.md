# 競プロ用ランダムテスト生成ガイド (Python)

このドキュメントでは、競プロのランダムテストで使用する `generate.py` の書き方と、Python の `random` モジュールを用いた便利なテクニックをまとめています。

## 1. 基本テンプレート (`generate.py`)

以下のコードをコピーして `generate.py` として保存し、`generate()` 関数の中身を問題に合わせて書き換えてください。

```python
import random
import string
import sys

# --- ユーティリティ関数 ---

def gen_int(l, r):
    """[l, r] の範囲の整数を生成"""
    return random.randint(l, r)

def gen_array(n, l, r):
    """長さ n, 要素 [l, r] の配列を生成"""
    return [random.randint(l, r) for _ in range(n)]

def gen_unique_array(n, l, r):
    """長さ n, 要素 [l, r] で重複のない配列を生成"""
    return random.sample(range(l, r + 1), n)

def gen_string(length, alphabet=string.ascii_lowercase):
    """指定された文字セットからなる長さ length の文字列を生成"""
    return "".join(random.choices(alphabet, k=length))

def gen_grid(h, w, chars=".#", weights=None):
    """H x W のグリッドを生成 (weightsで出現確率を調整可能)"""
    if weights is None: weights = [1] * len(chars)
    return ["".join(random.choices(chars, weights=weights, k=w)) for _ in range(h)]

def gen_tree(n, start=1):
    """N頂点の木 (辺のリスト) を生成"""
    edges = []
    nodes = list(range(start, start + n))
    random.shuffle(nodes)
    for i in range(1, n):
        u = nodes[i]
        v = nodes[random.randint(0, i - 1)]
        if random.getrandbits(1): u, v = v, u
        edges.append((u, v))
    return edges

def is_prime(n):
    """素数判定 (ミラー-ラビン法)"""
    if n < 2: return False
    if n == 2 or n == 3: return True
    if n % 2 == 0: return False
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    # 判定に使う底 (32bit/64bit整数なら以下で十分)
    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23]:
        if n <= a: break
        x = pow(a, d, n)
        if x == 1 or x == n - 1: continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1: break
        else: return False
    return True

def gen_prime(l, r):
    """[l, r] の範囲の素数をランダムに1つ生成"""
    while True:
        p = random.randint(l, r)
        if is_prime(p): return p

def gen_composite(l, r):
    """[l, r] の範囲の合成数をランダムに1つ生成"""
    while True:
        n = random.randint(l, r)
        if n > 1 and not is_prime(n): return n

# --- 生成ロジック ---

def generate():
    # 1. パラメータの決定
    N = gen_int(1, 10)
    A = gen_array(N, 1, 100)
    
    # 2. 標準出力への出力
    print(N)
    print(*A)

if __name__ == "__main__":
    generate()
```

## 2. Python `random` モジュール解説

### 数値の生成
- `random.randint(a, b)`: $a \le x \le b$ となる整数 $x$ を生成（**両端を含む**）。
- `random.random()`: $0.0 \le x < 1.0$ の浮動小数点数を生成。
- `random.uniform(a, b)`: $a \le x \le b$ の浮動小数点数を生成。

### 配列・リストの操作
- `random.choice(list)`: リストから要素を1つランダムに選ぶ。
- `random.choices(list, k=n)`: リストから**重複を許して** $n$ 個選ぶ（文字列や配列生成に便利）。
  - `weights=[w1, w2, ...]` を指定すると出現確率を調整できる。
- `random.sample(list, k=n)`: リストから**重複なしで** $n$ 個選ぶ（組み合わせや集合に便利）。
- `random.shuffle(list)`: リストをインプレースで（中身を直接）シャッフルする。

## 3. 逆引きテクニック

### 文字列の生成
`string` モジュールと組み合わせたり、直接文字セットを指定します。
```python
import string
# 英小文字
S = "".join(random.choices(string.ascii_lowercase, k=N))
# 特定の文字 (A, B, C)
S = "".join(random.choices("ABC", k=N))
# 括弧を含む文字列
S = "".join(random.choices("()", k=N))
# 英数字と記号
S = "".join(random.choices(string.ascii_letters + string.digits + "()[]{}", k=N))
```

### グリッドの生成
壁（`#`）の密度を調整するのがコツです。
```python
# 道(.)が80%, 壁(#)が20%の 5x5 グリッド
grid = ["".join(random.choices(".#", weights=[8, 2], k=5)) for _ in range(5)]
```

### 出力形式のテクニック
- **スペース区切り**: `print(*A)`
- **改行区切り**: `print(*A, sep="\n")`
- **高速出力**: 大量のデータを出力する場合は `sys.stdout.write` を使うか、大きな文字列にまとめてから一度に `print` します。

## 4. テストのコツ

- **コーナーケースを狙う**: `random.choice([1, 2, N, 10**18])` のように、境界値を意図的に混ぜるとバグが見つかりやすくなります。
- **シード値の固定**: バグが再現したときは、冒頭で `random.seed(42)` のように固定すると、常に同じ入力が生成されるようになります。
- **最初は小さく**: $N=10$ 程度の小さなケースで数千回回すほうが、目視で原因を特定しやすいため効率的です。
