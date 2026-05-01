// Counting Sort benchmark — values in [0, n-1] so k = n, making runtime always O(n).
// Not comparison-based; comparison count is reported as N/A.
// Tests n = 1000 to 50000 across sorted, random, and reverse input for consistency.

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());

    vector<int> count(maxVal + 1, 0);
    for (int x : arr) count[x]++;

    // write values back in sorted order
    int idx = 0;
    for (int v = 0; v <= maxVal; v++)
        while (count[v]-- > 0)
            arr[idx++] = v;
}

// Input generators

// sorted input — no true best case, same O(n) either way
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

// reverse sorted — no true worst case, same O(n) either way
vector<int> worstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

// runs the sort reps times and returns the median time (ns)
long long runExperiment(vector<int> (*generator)(int), int n, int reps = 10) {
    vector<long long> times(reps);

    for (int r = 0; r < reps; r++) {
        vector<int> arr = generator(n);
        auto start = high_resolution_clock::now();
        countingSort(arr);
        auto end = high_resolution_clock::now();
        times[r] = duration_cast<nanoseconds>(end - start).count();
    }

    sort(times.begin(), times.end());
    return times[reps / 2];
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
    cout << "  COUNTING SORT — Performance Study" << endl;
    cout << "======================================================" << endl;
    cout << fixed << setprecision(2);

    struct Case {
        string label;
        vector<int> (*generator)(int);
    };

    vector<Case> cases = {
        {"Best Case (sorted — same O(n) operations)",          bestCase},
        {"Average Case (random)",                              averageCase},
        {"Worst Case (reverse sorted — same O(n) operations)", worstCase}
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
                countingSort(sample);
                printSample(sample);
            }
        }
    }

    cout << "\n======================================================" << endl;
    cout << "  Done. All runs used " << reps << " repetitions (median reported)." << endl;
    cout << "======================================================" << endl;

    return 0;
}
