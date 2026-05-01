// Sorts integers in range [0, n-1] using Quicksort (Median-of-Three pivot).
// Automatically tests multiple input sizes across three cases:
//   - Best case:    randomly shuffled array (good pivot distribution)
//   - Average case: randomly shuffled array
//   - Worst case:   all equal elements (pivot always equals every element — O(n^2))
//
// Pivot selection: median of arr[left], arr[mid], arr[right], placed at arr[right].
// Partitioning: Lomuto scheme.
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

// Rearranges arr[left], arr[mid], arr[right] so their median ends up at arr[right]
void medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid])   swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right])  swap(arr[mid], arr[right]);
    // Median is now at arr[mid]; move it to arr[right] to use as pivot
    swap(arr[mid], arr[right]);
}

void quicksortHelper(vector<int>& arr, int left, int right, long long& comparisons) {
    if (left >= right) return;

    medianOfThree(arr, left, right);
    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        comparisons++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    int pi = i + 1;

    quicksortHelper(arr, left, pi - 1, comparisons);
    quicksortHelper(arr, pi + 1, right, comparisons);
}

long long quickSort(vector<int>& arr) {
    long long comparisons = 0;
    if ((int)arr.size() > 1)
        quicksortHelper(arr, 0, (int)arr.size() - 1, comparisons);
    return comparisons;
}

// Input generators

// Best case: randomly shuffled [0, 1, ..., n-1]
// (Median-of-three pivot performs well on random input; no distinct best case)
vector<int> bestCase(int n) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 0);
    srand(42);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
    return arr;
}

// Average case: randomly shuffled [0, 1, ..., n-1]
// (Same as best case — both demonstrate O(n log n) average behavior)
vector<int> averageCase(int n) {
    return bestCase(n);
}

// Worst case: all equal elements
// (Any pivot selected by median-of-three equals every element — O(n^2) partitioning)
vector<int> worstCase(int n) {
    return vector<int>(n, 1);
}

// Run one experiment: returns {median_time_ns, comparisons}
// Repeats REPS times and takes the median time.
pair<long long, long long> runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);
    long long lastComparisons = 0;

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        lastComparisons = quickSort(arr);
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
    cout << "  QUICKSORT (Median-of-Three) — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (random — good pivot distribution)",    bestCase},
        {"Average Case (random)",                           averageCase},
        {"Worst Case (all equal — O(n^2) partitioning)",   worstCase}
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
                quickSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
