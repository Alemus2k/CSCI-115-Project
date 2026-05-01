// Sorts integers in range [0, n-1] using Heapsort.
// Automatically tests multiple input sizes across three cases:
//   - Best case:    already sorted array*
//   - Average case: randomly shuffled array
//   - Worst case:   reverse sorted array*
//
// * Heapsort has no true best/worst case distinction — it always
//   performs O(n log n) operations regardless of input order.
//   Sorted/reverse-sorted inputs are used for consistency with other algorithms.
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

// Sift down arr[i] within a max-heap of size heapSize
void heapify(vector<int>& arr, int heapSize, int i, long long& comparisons) {
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize) {
        comparisons++;
        if (arr[left] > arr[largest]) largest = left;
    }
    if (right < heapSize) {
        comparisons++;
        if (arr[right] > arr[largest]) largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, heapSize, largest, comparisons);
    }
}

long long heapSort(vector<int>& arr) {
    long long comparisons = 0;
    int n = arr.size();

    // Build max heap (bottom-up)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, comparisons);

    // Extract elements one by one: swap root with last, shrink heap, heapify
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, comparisons);
    }

    return comparisons;
}

// Input generators

// Best case: already sorted [0, 1, ..., n-1]
// (No true best case for Heapsort — always O(n log n))
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
// (No true worst case for Heapsort — always O(n log n))
vector<int> worstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

// Run one experiment: returns {median_time_ns, comparisons}
// Repeats REPS times and takes the median time.
pair<long long, long long> runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);
    long long lastComparisons = 0;

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        lastComparisons = heapSort(arr);
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
    cout << "  HEAPSORT — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (sorted — note: same O(n log n) comparisons)",          bestCase},
        {"Average Case (random)",                                            averageCase},
        {"Worst Case (reverse sorted — note: same O(n log n) comparisons)", worstCase}
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
                heapSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
