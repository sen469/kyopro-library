#!/bin/bash

# 削除対象のファイル定義
TARGET_FILES=("main" "ans" "out1.txt" "out2.txt" "input.txt" "dbg")

# 終了処理用の関数
cleanup()
{
    echo -e "\nInterrupted. Cleaning up..."
    rm -f "${TARGET_FILES[@]}"
    exit 0
}

# SIGINT (Ctrl+C) を受け取ったら cleanup 関数を実行する
trap cleanup SIGINT

# 引数の数が 0 個の場合：削除処理
if [ $# -eq 0 ]; then
    echo "Cleaning up files..."
    rm -f "${TARGET_FILES[@]}"
    echo "Done."
    exit 0
fi

# 引数が 2 個以外の場合は使い方を表示
if [ $# -ne 2 ]; then
    echo "Usage: $0 [your_code.cpp] [ans_code.cpp]"
    echo "Or:    $0 (to clean up files)"
    exit 1
fi

# コンパイル実行
g++ -I./ -O3 "$1" -o main && g++ -I./ -O3 "$2" -o ans

# 直前のコマンドが失敗した場合は終了
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running tests... Press Ctrl+C to stop."

# テストループ
while true; do
    python3 generate.py > input.txt

    # 実行が失敗した場合もループを抜ける
    ./main < input.txt > out1.txt || break
    ./ans < input.txt > out2.txt || break

    # 差分比較
    if ! diff out1.txt out2.txt; then
        echo "WA found!"
        echo "--- input ---"
        cat input.txt
        break
    fi
done
