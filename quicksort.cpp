// Quicksort benchmark using Median-of-Three pivot (Lomuto partition).
// Best/average case is O(n log n) on random input; worst case is O(n²) on all-equal input.
// Prints median time (ns), comparison count, and first 10 sorted elements per case.

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// puts the median of arr[left], arr[mid], arr[right] at arr[right] to use as pivot
void medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid])   swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right])  swap(arr[mid], arr[right]);
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

// random input — median-of-three pivots well here, giving O(n log n)
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

// same as best case — both demonstrate O(n log n) average behavior
vector<int> averageCase(int n) {
    return bestCase(n);
}

// all equal elements — every partition is maximally unbalanced, giving O(n²)
vector<int> worstCase(int n) {
    return vector<int>(n, 1);
}

// runs the sort reps times and returns {median time (ns), comparison count}
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

// quick sanity check — prints the first 10 elements
void printSample(const vector<int>& arr) {
    int limit = min((int)arr.size(), 10);
    cout << "  First " << limit << " elements: [";
    for (int i = 0; i < limit; i++) {
        cout << arr[i];
        if (i < limit - 1) cout << ", ";
    }
    cout << "]" << endl;
}

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
        {"Best Case (random — good pivot distribution)",  bestCase},
        {"Average Case (random)",                         averageCase},
        {"Worst Case (all equal — O(n²) partitioning)",  worstCase}
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
