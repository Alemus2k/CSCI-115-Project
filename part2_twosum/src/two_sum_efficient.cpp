// Two-Sum efficient — single pass with a hash map, O(n) time, O(n) space.
// Reads n and target from stdin, then n integers.
// Prints the first pair of indices where arr[i] + arr[j] == target.

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// stores complement → index as we scan; returns on first hit
pair<int,int> twoSumEfficient(const vector<int>& arr, int target) {
    unordered_map<int,int> seen;  // value → index
    for (int i = 0; i < (int)arr.size(); i++) {
        int complement = target - arr[i];
        auto it = seen.find(complement);
        if (it != seen.end())
            return {it->second, i};
        seen[arr[i]] = i;
    }
    return {-1, -1};
}

int main() {
    int n, target;
    cin >> n >> target;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];

    auto [i, j] = twoSumEfficient(arr, target);
    if (i == -1)
        cout << "No solution" << endl;
    else
        cout << "Indices: " << i << " and " << j
             << "  (values: " << arr[i] << " + " << arr[j] << " = " << target << ")" << endl;

    return 0;
}
