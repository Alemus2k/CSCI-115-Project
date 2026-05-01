// Merge Sort benchmark — tests n = 1000 to 50000 across sorted, random, and reverse input.
// Always O(n log n) regardless of input order; different inputs are tested for consistency.
// Prints median time (ns), comparison count, and first 10 sorted elements per case.

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

long long mergeHelper(vector<int>& arr, int left, int right) {
    if (left >= right) return 0;

    long long comparisons = 0;
    int mid = left + (right - left) / 2;
    comparisons += mergeHelper(arr, left, mid);
    comparisons += mergeHelper(arr, mid + 1, right);

    // merge the two sorted halves into a temp buffer, then copy back
    vector<int> temp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        comparisons++;
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }
    while (i <= mid)   temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = left; k <= right; k++)
        arr[k] = temp[k - left];

    return comparisons;
}

long long mergeSort(vector<int>& arr) {
    return mergeHelper(arr, 0, (int)arr.size() - 1);
}

// Input generators

// sorted input — no true best case, same O(n log n) either way
vector<int> bestCase(int n) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 0);
    return arr;
}

// random shuffle, fixed seed for reproducibility across algorithms
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

// reverse sorted — no true worst case, same O(n log n) either way
vector<int> worstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

// runs the sort reps times and returns {median time (ns), comparison count}
pair<long long, long long> runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);
    long long lastComparisons = 0;

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        lastComparisons = mergeSort(arr);
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
    cout << "  MERGE SORT — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (sorted — same O(n log n) comparisons)",          bestCase},
        {"Average Case (random)",                                      averageCase},
        {"Worst Case (reverse sorted — same O(n log n) comparisons)", worstCase}
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
                mergeSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
