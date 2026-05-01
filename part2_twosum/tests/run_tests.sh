#!/bin/bash
# Compiles both Two-Sum programs and runs them against the sample test cases.

set -e
SRC="../src"
BIN="./bin"
mkdir -p "$BIN"

echo "=== Compiling Two-Sum programs ==="
g++ -O2 -std=c++17 "$SRC/two_sum_brute.cpp"     -o "$BIN/two_sum_brute"
g++ -O2 -std=c++17 "$SRC/two_sum_efficient.cpp" -o "$BIN/two_sum_efficient"
echo "OK"

run_test() {
    local label="$1"
    local n="$2"
    local target="$3"
    local input="$4"

    echo ""
    echo "--- $label ---"
    echo "Input: n=$n, target=$target, array=[$input]"
    echo "  Brute:     $(echo -e "$n $target\n$input" | "$BIN/two_sum_brute")"
    echo "  Efficient: $(echo -e "$n $target\n$input" | "$BIN/two_sum_efficient")"
}

run_test "Basic pair"        5  9   "2 7 11 15 4"
run_test "Pair at end"       6  13  "1 3 5 7 9 4"
run_test "No solution"       4  100 "1 2 3 4"
run_test "Negative numbers"  5  0   "-3 7 -4 4 3"
run_test "Larger array"      10 20  "15 2 11 7 6 3 9 12 14 8"

echo ""
echo "=== All tests done ==="
