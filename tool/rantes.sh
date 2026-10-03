#!/bin/bash
set -euo pipefail

CXX=${CXX:-g++}
COMPILE_FLAGS=(-std=c++20 -O2 -Wall -Wextra)
# includeの探索先。複数ある場合は1行ずつ追加する。空配列でも使用可能。
LIBRARY_PATHS=(
    # "/path/to/another-library"
)
PYTHON=${PYTHON:-python3}
GENERATOR=generate.py
INPUT=input.txt
OUTPUT=out1.txt
ANSWER=out2.txt

# 引数がないときは不要なファイルを削除する
if [ "$#" -eq 0 ]; then
    binaries=(main ans dbg a.out)
    for source in ./*.cpp; do
        [ -f "$source" ] || continue
        base=${source%.cpp}
        binaries+=("$base" "${base}.debug" "./_${base#./}" "./_${base#./}.debug")
    done
    for file in "${binaries[@]}"; do
        if [ -f "$file" ] && [ -x "$file" ] && [ ! -L "$file" ]; then
            rm -f -- "$file"
        fi
    done
    for file in "$OUTPUT" "$ANSWER" output.txt out.txt; do
        [ "$file" = "$INPUT" ] || rm -f -- "$file"
    done
    echo "Cleaned executables and outputs."
    exit 0
fi

if [ "$#" -lt 2 ]; then
    echo "Usage: bash rantes.sh j.cpp ans.cpp [cases (0: unlimited)] [-- compiler flags...]" >&2
    exit 2
fi
cases=${3:-0}
[[ "$cases" =~ ^(0|[1-9][0-9]*)$ ]] || { echo 'cases must be a nonnegative integer' >&2; exit 2; }
trap 'echo "Interrupted. Input and outputs kept."; exit 130' INT

source=$1
reference=$2
shift 2
if [ "$#" -gt 0 ]; then shift; fi
if [ "$#" -gt 0 ]; then
    [ "$1" = -- ] || { echo 'Use -- before compiler flags' >&2; exit 2; }
    shift
fi
for file in "$source" "$reference"; do
    if [[ "$file" != *.cpp || ! -f "$file" ]]; then
        echo "Specify an existing .cpp file: $file" >&2
        exit 2
    fi
done
mode=${MODE:-release}
case "$mode" in
    release) suffix= ;;
    debug) suffix=.debug; COMPILE_FLAGS+=(-O0 -g -D_GLIBCXX_DEBUG= -fsanitize=address,undefined -fno-omit-frame-pointer) ;;
    *) echo 'MODE must be release or debug' >&2; exit 2 ;;
esac
program=_$(basename -- "${source%.cpp}")$suffix
answer=_$(basename -- "${reference%.cpp}")$suffix
if [[ "$program" == "$answer" && "$source" != "$reference" ]]; then
    echo 'Use different source filenames to avoid overwriting an executable.' >&2
    exit 2
fi

# 追加のincludeパスやコンパイル設定は -- 以降で受け取る。
INCLUDE_FLAGS=()
for path in "${LIBRARY_PATHS[@]}"; do
    INCLUDE_FLAGS+=(-I "$path")
done
"$CXX" "${COMPILE_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$source" "$@" -o "$program"
"$CXX" "${COMPILE_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$reference" "$@" -o "$answer"

for ((i=1; cases==0 || i<=cases; i++)); do
    : > "$OUTPUT"
    : > "$ANSWER"
    "$PYTHON" "$GENERATOR" > "$INPUT"
    printf 'Case %d\n' "$i"
    "./$program" < "$INPUT" > "$OUTPUT"
    "./$answer" < "$INPUT" > "$ANSWER"
    if ! diff -u "$OUTPUT" "$ANSWER"; then
        echo "WA found!"
        echo '--- input ---'
        cat "$INPUT"
        exit 1
    fi
done
echo "Passed $cases cases."
