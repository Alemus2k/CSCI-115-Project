"""
generate_graphs.py — reproduces all performance graphs from the CSCI 115 report.

Steps:
  1. Compiles each sorting benchmark with g++ -O2 -std=c++17
  2. Runs each benchmark and parses the timing tables
  3. Saves all figures to graphs/

Output files:
  graphs/fig1_insertion_time_fn.png   ... fig8_radix_time_fn.png
  graphs/fig9_insertion_time_comps.png ... fig14_heapsort_time_comps.png
  graphs/fig15_all_algorithms_best.png
  graphs/fig16_all_algorithms_avg.png
  graphs/fig17_all_algorithms_worst.png

Requirements: Python 3.8+, matplotlib, numpy
  pip install matplotlib numpy
"""

import subprocess
import re
import math
import os
import sys

import matplotlib.pyplot as plt
import numpy as np

# ─── config ──────────────────────────────────────────────────────────────────

SRC_DIR    = "part1_sorting/src"
BUILD_DIR  = "build"
GRAPHS_DIR = "graphs"

SIZES = [1000, 5000, 10000, 25000, 50000]

ALGORITHMS = [
    "insertion_sort",
    "selection_sort",
    "bubble_sort",
    "merge_sort",
    "quicksort",
    "heapsort",
    "counting_sort",
    "radix_sort",
]

# display names for axis labels / titles
DISPLAY = {
    "insertion_sort": "Insertion Sort",
    "selection_sort": "Selection Sort",
    "bubble_sort":    "Bubble Sort",
    "merge_sort":     "Merge Sort",
    "quicksort":      "Quicksort",
    "heapsort":       "Heapsort",
    "counting_sort":  "Counting Sort",
    "radix_sort":     "Radix Sort",
}

# f(n) divisor for the Time/f(n) graph (index 0=best, 1=avg, 2=worst)
FN = {
    "insertion_sort": [lambda n: n,         lambda n: n**2,     lambda n: n**2    ],
    "selection_sort": [lambda n: n**2,       lambda n: n**2,     lambda n: n**2   ],
    "bubble_sort":    [lambda n: n,         lambda n: n**2,     lambda n: n**2    ],
    "merge_sort":     [lambda n: n*math.log2(n), lambda n: n*math.log2(n), lambda n: n*math.log2(n)],
    "quicksort":      [lambda n: n*math.log2(n), lambda n: n*math.log2(n), lambda n: n**2           ],
    "heapsort":       [lambda n: n*math.log2(n), lambda n: n*math.log2(n), lambda n: n*math.log2(n)],
    "counting_sort":  [lambda n: n,         lambda n: n,         lambda n: n       ],
    "radix_sort":     [lambda n: n,         lambda n: n,         lambda n: n       ],
}

FN_LABEL = {
    "insertion_sort": ["n",         "n²",       "n²"          ],
    "selection_sort": ["n²",        "n²",       "n²"          ],
    "bubble_sort":    ["n",         "n²",       "n²"          ],
    "merge_sort":     ["n log n",   "n log n",  "n log n"     ],
    "quicksort":      ["n log n",   "n log n",  "n²"          ],
    "heapsort":       ["n log n",   "n log n",  "n log n"     ],
    "counting_sort":  ["n",         "n",        "n"           ],
    "radix_sort":     ["n",         "n",        "n"           ],
}

CASE_LABELS = ["Best Case", "Average Case", "Worst Case"]
COLORS      = ["steelblue", "darkorange", "crimson"]

# ─── compile ─────────────────────────────────────────────────────────────────

def compile_all():
    os.makedirs(BUILD_DIR, exist_ok=True)
    for alg in ALGORITHMS:
        src = os.path.join(SRC_DIR, f"{alg}.cpp")
        out = os.path.join(BUILD_DIR, alg)
        print(f"  compiling {alg}...", end="", flush=True)
        result = subprocess.run(
            ["g++", "-O2", "-std=c++17", src, "-o", out],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            print(f" FAILED\n{result.stderr}")
            sys.exit(1)
        print(" ok")

# ─── run & parse ─────────────────────────────────────────────────────────────

def run_and_parse(alg):
    """Returns { 'best': [(n, time, comps), ...], 'avg': [...], 'worst': [...] }"""
    binary = os.path.join(BUILD_DIR, alg)
    result = subprocess.run([binary], capture_output=True, text=True, timeout=120)
    if result.returncode != 0:
        print(f"Error running {alg}: {result.stderr}")
        sys.exit(1)

    data = {"best": [], "avg": [], "worst": []}
    current = None

    for line in result.stdout.splitlines():
        line = line.strip()
        if "Best Case" in line:
            current = "best"
        elif "Average Case" in line:
            current = "avg"
        elif "Worst Case" in line:
            current = "worst"
        elif current:
            # data rows look like:  "1000      234500   999"  or  "1000  234500  N/A"
            m = re.match(r"^(\d+)\s+(\d+)\s+(\d+|N/A)", line)
            if m:
                n     = int(m.group(1))
                t     = int(m.group(2))
                comps = None if m.group(3) == "N/A" else int(m.group(3))
                data[current].append((n, t, comps))

    return data

# ─── figure helpers ───────────────────────────────────────────────────────────

def save(fig, name):
    path = os.path.join(GRAPHS_DIR, name)
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  saved {path}")

# ─── Fig 1–8: Time / f(n) vs n ───────────────────────────────────────────────

def plot_time_fn(alg, data, fig_num):
    fig, ax = plt.subplots(figsize=(7, 4.5))
    cases = [("best", 0), ("avg", 1), ("worst", 2)]

    for key, idx in cases:
        rows = data[key]
        if not rows:
            continue
        ns = [r[0] for r in rows]
        fn_vals = [FN[alg][idx](n) for n in ns]
        ratios  = [t / f for (_, t, _), f in zip(rows, fn_vals)]
        label   = f"{CASE_LABELS[idx]}  [÷ {FN_LABEL[alg][idx]}]"
        ax.plot(ns, ratios, marker="o", color=COLORS[idx], label=label)

    ax.set_title(f"{DISPLAY[alg]} — Time / f(n) vs n  (Figure {fig_num})")
    ax.set_xlabel("n")
    ax.set_ylabel("Time / f(n)  (ns per operation)")
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    save(fig, f"fig{fig_num}_{alg}_time_fn.png")

# ─── Fig 9–14: Time / #comparisons vs n ─────────────────────────────────────

COMP_ALGORITHMS = [
    "insertion_sort", "selection_sort", "bubble_sort",
    "merge_sort", "quicksort", "heapsort",
]

def plot_time_comps(alg, data, fig_num):
    fig, ax = plt.subplots(figsize=(7, 4.5))
    cases = [("best", 0), ("avg", 1), ("worst", 2)]

    for key, idx in cases:
        rows = [(n, t, c) for n, t, c in data[key] if c is not None and c > 0]
        if not rows:
            continue
        ns    = [r[0] for r in rows]
        ratio = [r[1] / r[2] for r in rows]
        ax.plot(ns, ratio, marker="o", color=COLORS[idx], label=CASE_LABELS[idx])

    ax.set_title(f"{DISPLAY[alg]} — Time / #Comparisons vs n  (Figure {fig_num})")
    ax.set_xlabel("n")
    ax.set_ylabel("ns per comparison")
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    save(fig, f"fig{fig_num}_{alg}_time_comps.png")

# ─── Fig 15–17: cross-algorithm comparison ───────────────────────────────────

def plot_comparison(all_data, case_key, case_label, fig_num):
    fig, (ax_all, ax_fast) = plt.subplots(1, 2, figsize=(13, 5))

    fast_algs = ["merge_sort", "quicksort", "heapsort", "counting_sort", "radix_sort"]

    for alg in ALGORITHMS:
        rows = all_data[alg][case_key]
        if not rows:
            continue
        ns = [r[0] for r in rows]
        ts = [r[1] / 1e6 for r in rows]  # ns → ms
        ax_all.plot(ns, ts, marker="o", label=DISPLAY[alg])
        if alg in fast_algs:
            ax_fast.plot(ns, ts, marker="o", label=DISPLAY[alg])

    for ax, title in [
        (ax_all,  f"All Algorithms — {case_label}"),
        (ax_fast, f"O(n log n) & Linear — {case_label}"),
    ]:
        ax.set_xlabel("n")
        ax.set_ylabel("Median Time (ms)")
        ax.set_title(title)
        ax.legend(fontsize=7)
        ax.grid(True, alpha=0.3)

    fig.suptitle(f"Figure {fig_num}: {case_label} Performance Comparison", fontsize=11)
    fig.tight_layout()
    save(fig, f"fig{fig_num}_comparison_{case_key}.png")

# ─── main ─────────────────────────────────────────────────────────────────────

def main():
    os.makedirs(GRAPHS_DIR, exist_ok=True)

    print("=== Compiling sorting benchmarks ===")
    compile_all()

    print("\n=== Running benchmarks (this takes ~1 minute) ===")
    all_data = {}
    for alg in ALGORITHMS:
        print(f"  {alg}...", end="", flush=True)
        all_data[alg] = run_and_parse(alg)
        print(" done")

    print("\n=== Generating graphs ===")

    # Fig 1–8: Time / f(n)
    for i, alg in enumerate(ALGORITHMS, start=1):
        plot_time_fn(alg, all_data[alg], i)

    # Fig 9–14: Time / #comparisons
    for i, alg in enumerate(COMP_ALGORITHMS, start=9):
        plot_time_comps(alg, all_data[alg], i)

    # Fig 15–17: cross-algorithm comparison
    for fig_num, (case_key, case_label) in enumerate(
        [("best", "Best Case"), ("avg", "Average Case"), ("worst", "Worst Case")],
        start=15
    ):
        plot_comparison(all_data, case_key, case_label, fig_num)

    print(f"\nAll graphs saved to {GRAPHS_DIR}/")

if __name__ == "__main__":
    main()
