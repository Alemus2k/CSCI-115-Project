// Two-Sum brute force — checks all pairs, O(n²) time, O(1) space.
// Reads n and target from stdin, then n integers.
// Prints the first pair of indices where arr[i] + arr[j] == target.

#include <iostream>
#include <vector>

using namespace std;

// scans all (i, j) pairs; returns the first match or {-1,-1}
pair<int,int> twoSumBrute(const vector<int>& arr, int target) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] + arr[j] == target)
                return {i, j};
    return {-1, -1};
}

int main() {
    int n, target;
    cin >> n >> target;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];

    auto [i, j] = twoSumBrute(arr, target);
    if (i == -1)
        cout << "No solution" << endl;
    else
        cout << "Indices: " << i << " and " << j
             << "  (values: " << arr[i] << " + " << arr[j] << " = " << target << ")" << endl;

    return 0;
}
