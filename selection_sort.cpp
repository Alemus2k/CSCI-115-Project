// Sorts integers in range [0, n-1] using Selection Sort.
// Automatically tests multiple input sizes across three cases:
//   - Best case:    any order (Selection Sort is always O(n^2))
//   - Average case: randomly shuffled array
//   - Worst case:   any order (Selection Sort is always O(n^2))
//
// Outputs: input type, n, median time (ns), comparison count,
//          and first 10 elements of sorted result.
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Selection Sort
// Returns the number of comparisons performed.
long long selectionSort(vector<int>& arr) {
    long long comparisons = 0;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
    return comparisons;
}

// Input generators

// Best case: already sorted [0, 1, ..., n-1]
// (Selection Sort has no true best case — always O(n^2) comparisons)
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
// (Still O(n^2) — included to match experimental design of other sorts)
vector<int> worstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

// Run one experiment: returns {median_time_ns, comparisons}
pair<long long, long long> runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);
    long long lastComparisons = 0;

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        lastComparisons = selectionSort(arr);
        auto end = high_resolution_clock::now();
        times[r] = duration_cast<nanoseconds>(end - start).count();
    }

    sort(times.begin(), times.end());
    long long medianTime = times[reps / 2];
    return {medianTime, lastComparisons};
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
    cout << "  SELECTION SORT — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (sorted — note: same O(n^2) comparisons)",          bestCase},
        {"Average Case (random)",                                        averageCase},
        {"Worst Case (reverse sorted — note: same O(n^2) comparisons)", worstCase}
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
            auto [medTime, comps] = runExperiment(c.generator, n, reps);
            cout << setw(10) << n
                 << setw(20) << medTime
                 << setw(20) << comps
                 << endl;

            if (n == sizes.front()) {
                vector<int> sample = c.generator(n);
                selectionSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
