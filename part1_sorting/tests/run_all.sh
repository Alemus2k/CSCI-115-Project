#!/bin/bash
# Compiles and runs all 8 sorting benchmarks.
# Each program is self-contained — it generates its own inputs and prints timing tables.

set -e
SRC="../src"
BIN="./bin"
mkdir -p "$BIN"

ALGORITHMS=(
    bubble_sort
    counting_sort
    heapsort
    insertion_sort
    merge_sort
    quicksort
    radix_sort
    selection_sort
)

echo "=== Compiling ==="
for alg in "${ALGORITHMS[@]}"; do
    g++ -O2 -std=c++17 "$SRC/$alg.cpp" -o "$BIN/$alg"
    echo "  $alg — OK"
done

echo ""
echo "=== Running benchmarks ==="
for alg in "${ALGORITHMS[@]}"; do
    echo ""
    echo "=============================="
    echo "  $alg"
    echo "=============================="
    "$BIN/$alg"
done
