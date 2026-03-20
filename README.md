# CSCI 115 — Sorting Algorithms Project
## README

---

## Overview

This project implements and benchmarks 8 sorting algorithms in C++
as part of a study comparing theoretical complexity against
experimental performance. Each algorithm is in its own `.cpp` file
and runs automatically across multiple input sizes and input types
(best case, average case, worst case).

---

## Files Included

| File                  | Algorithm                          |
|-----------------------|------------------------------------|
| `insertion_sort.cpp`  | Insertion Sort                     |
| `selection_sort.cpp`  | Selection Sort                     |
| `bubble_sort.cpp`     | Bubble Sort (with early-exit flag) |
| `merge_sort.cpp`      | Merge Sort                         |
| `quicksort.cpp`       | Quicksort (Median-of-Three pivot)  |
| `heapsort.cpp`        | Heapsort                           |
| `counting_sort.cpp`   | Counting Sort                      |
| `radix_sort.cpp`      | Radix Sort (LSD, base 10)          |

---

## Dependencies

- A C++ compiler supporting **C++17** or later
  - `g++` (GCC 7+) — recommended
  - `clang++` (Clang 5+) — also works
  - MSVC 2017+ on Windows
- No external libraries required — uses only the C++ Standard Library

---

## Input Design

All arrays contain integers in the range **[0, n-1]**

### Input sizes tested:
`n = 1000, 5000, 10000, 25000, 50000`

### Input types per algorithm:

| Algorithm      | Best Case Input         | Average Case Input   | Worst Case Input         |
|----------------|-------------------------|----------------------|--------------------------|
| Insertion Sort | Already sorted          | Randomly shuffled    | Reverse sorted           |
| Selection Sort | Already sorted*         | Randomly shuffled    | Reverse sorted*          |
| Bubble Sort    | Already sorted          | Randomly shuffled    | Reverse sorted           |
| Merge Sort     | Already sorted*         | Randomly shuffled    | Reverse sorted*          |
| Quicksort      | Randomly shuffled       | Randomly shuffled    | All equal elements       |
| Heapsort       | Already sorted*         | Randomly shuffled    | Reverse sorted*          |
| Counting Sort  | Already sorted*         | Randomly shuffled    | Reverse sorted*          |
| Radix Sort     | Already sorted*         | Randomly shuffled    | Reverse sorted*          |

\* These algorithms have no true best/worst case distinction — they
  always perform the same number of operations regardless of input
  order. Sorted/reverse-sorted inputs are used for consistency.

---

## Output Format

Each program prints a table for each case (best, average, worst)
showing:

- `n` — input size
- `Median Time (ns)` — median execution time over 10 runs in nanoseconds
- `Comparisons` — number of element-to-element comparisons
  (reported as N/A for Counting Sort and Radix Sort, which are not
  comparison-based)
- First 10 elements of the sorted output (printed for n=1000 only,
  for quick correctness verification)

### Sample output (Insertion Sort):
```
======================================================
  INSERTION SORT — Performance Study
======================================================

--- Best Case (sorted) ---
n         Median Time (ns)    Comparisons
--------------------------------------------------
1000      2341                999
  First 10 elements: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
5000      11203               4999
10000     22917               9999
...
```

---

## Timing Mechanism

- Uses `std::chrono::high_resolution_clock`
- Timer wraps only the sort call — array generation is excluded
- **10 repetitions** per (algorithm, case, size) combination
- **Median** of 10 runs is reported (robust against OS scheduling noise)

---

## Notes on Comparison Counting

- For comparison-based sorts (Insertion, Selection, Bubble, Merge,
  Quicksort, Heapsort), every `if (arr[i] > arr[j])` style check
  increments a counter.
- Counting Sort and Radix Sort do not compare elements to each other,
  so their comparison count is reported as N/A.
- These comparison counts are used in Part 1 Section 5 of the report
  to analyze whether #comparisons predicts execution time.

---

## Reproducibility

- Random shuffles use `srand(42)` (fixed seed) for reproducibility.
- Running the same program twice on the same machine will produce
  the same array contents, but timing may vary slightly due to OS
  scheduling, CPU caching, and other hardware effects.

---

## Part 2 — Two-Sum Problem

The Two-Sum implementations (brute force and efficient) are described
in the written report with pseudocode and complexity analysis.
No separate `.cpp` file is required per the project deliverables spec.

---

## Authors

- Alessandro Heaney , Name 2, Name 3, Name 4

