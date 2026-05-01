// Sorts integers in range [0, n-1] using Radix Sort (LSD, base 10).
// Automatically tests multiple input sizes across three cases:
//   - Best case:    already sorted array*
//   - Average case: randomly shuffled array
//   - Worst case:   reverse sorted array*
//
// * Radix Sort has no true best/worst case distinction — it always
//   performs O(d * (n + b)) operations regardless of input order,
//   where d = number of digits and b = base (10).
//   For values in [0, n-1], d = floor(log10(n-1)) + 1.
//   Sorted/reverse-sorted inputs are used for consistency with other algorithms.
//
// Radix Sort is not comparison-based; comparison count is reported as N/A.
//
// Outputs: input type, n, median time (ns), comparison count (N/A),
//          and first 10 elements of sorted result.

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// One pass of counting sort on the digit selected by exp (1, 10, 100, ...)
void countingSortByDigit(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);

    for (int x : arr) count[(x / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    // Traverse right-to-left for stable sort
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[--count[digit]] = arr[i];
    }

    arr = output;
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());

    // Process each digit from least significant to most significant
    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countingSortByDigit(arr, exp);
}

// Input generators

// Best case: already sorted [0, 1, ..., n-1]
// (No true best case for Radix Sort — always O(d * (n + b)))
vector<int> bestCase(int n) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 0);
    return arr;
}

// Average case: randomly shuffled [0, 1, ..., n-1]
vector<int> averageCase(int n) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 0);
    srand(42);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
    return arr;
}

// Worst case: reverse sorted [n-1, n-2, ..., 0]
// (No true worst case for Radix Sort — always O(d * (n + b)))
vector<int> worstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

// Run one experiment: returns median time in nanoseconds
// Repeats REPS times and takes the median time.
long long runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        radixSort(arr);
        auto end = high_resolution_clock::now();
        times[r] = duration_cast<nanoseconds>(end - start).count();
    }

    sort(times.begin(), times.end());
    return times[reps / 2];
}

// Print first 10 elements of a sorted array for verification
void printSample(const vector<int>& arr) {
    int limit = min((int)arr.size(), 10);
    cout << "  First " << limit << " elements: [";
    for (int i = 0; i < limit; i++) {
        cout << arr[i];
        if (i < limit - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Main
int main() {
    vector<int> sizes = {1000, 5000, 10000, 25000, 50000};
    int reps = 10;

    cout << "======================================================" << endl;
    cout << "  RADIX SORT (LSD, base 10) — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (sorted — note: same O(d*(n+b)) operations)",          bestCase},
        {"Average Case (random)",                                            averageCase},
        {"Worst Case (reverse sorted — note: same O(d*(n+b)) operations)", worstCase}
    };

    for (auto& c : cases) {
        cout << "\n--- " << c.label << " ---" << endl;
        cout << left
             << setw(10) << "n"
             << setw(20) << "Median Time (ns)"
             << setw(20) << "Comparisons"
             << endl;
        cout << string(50, '-') << endl;

        for (int n : sizes) {
            long long medTime = runExperiment(c.generator, n, reps);
            cout << setw(10) << n
                 << setw(20) << medTime
                 << setw(20) << "N/A"
                 << endl;

            if (n == sizes.front()) {
                vector<int> sample = c.generator(n);
                radixSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
