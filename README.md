# CSCI 115 — Sorting Algorithms & Two-Sum Project

## Team

**Team Name:** Team 8
**Members:** Alessandro Heaney, Adrian Hilario, Matthew Kramer, Jose Martinez

---

## Language & Environment

- **Language:** C++17
- **Compiler:** g++ (GCC 7+) or clang++ (Clang 5+)
- **OS tested:** macOS 15.7.4 (Apple M2), Linux

---

## How to Run Part 1 (Sorting Benchmarks)

Each of the 8 sorting algorithms is a standalone C++ program. It generates its own inputs, runs benchmarks, and prints timing tables.

**Compile and run one algorithm:**
```bash
g++ -O2 -std=c++17 part1_sorting/src/insertion_sort.cpp -o insertion_sort
./insertion_sort
```

**Compile and run all 8 at once:**
```bash
cd part1_sorting/tests
bash run_all.sh
```

Expected output: three tables (best / average / worst case) per algorithm, showing median time in nanoseconds over 10 runs across n = 1000, 5000, 10000, 25000, 50000, plus the first 10 sorted elements for n = 1000.

---

## How to Run Part 2 (Two-Sum)

Two implementations are provided: brute-force O(n²) and hash-map O(n).

**Compile:**
```bash
g++ -O2 -std=c++17 part2_twosum/src/two_sum_brute.cpp     -o two_sum_brute
g++ -O2 -std=c++17 part2_twosum/src/two_sum_efficient.cpp -o two_sum_efficient
```

**Input format:** first line is `n target`, second line is `n` space-separated integers.

**Example:**
```bash
echo "5 9
2 7 11 15 4" | ./two_sum_brute
# Output: Indices: 0 and 1  (values: 2 + 7 = 9)

echo "5 9
2 7 11 15 4" | ./two_sum_efficient
# Output: Indices: 0 and 1  (values: 2 + 7 = 9)
```

**Run all sample test cases:**
```bash
cd part2_twosum/tests
bash run_tests.sh
```

---

## How to Reproduce Graphs

1. Compile and run each sorting program (see Part 1 instructions above) to generate the timing output.
2. Copy the median time and comparison count values from the printed tables into a spreadsheet.
3. Graphs were produced in Microsoft Excel from that data — one chart per algorithm for Sections 4 and 5, and combined charts for Section 3.

---

## Notes

- **Timing:** `std::chrono::high_resolution_clock` wraps the sort call only — array generation is not timed.
- **Repetitions:** 10 runs per (algorithm, case, size) combination; the median is reported.
- **Reproducibility:** random shuffles use `srand(42)` (fixed seed) so array contents are identical across machines.
- **Comparisons:** Counting Sort and Radix Sort are not comparison-based; their comparison count is reported as N/A.
- **Quicksort worst case:** all-equal input (`[1,1,...,1]`), which makes every partition maximally unbalanced. All other algorithms use sorted or reverse-sorted inputs.
- **Input range:** all arrays contain integers in [0, n-1].

---

## Repository Structure

```
README.md
part1_sorting/
    src/                    ← 8 sorting algorithm .cpp files
    tests/
        run_all.sh          ← compile + run all benchmarks
part2_twosum/
    src/
        two_sum_brute.cpp       ← O(n²) brute force
        two_sum_efficient.cpp   ← O(n) hash-map solution
    tests/
        test_cases.txt          ← sample inputs
        run_tests.sh            ← run both programs against test cases
theoretical_vs_experimental_data.docx   ← full written report
```
