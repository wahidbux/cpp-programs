/*
 * =====================================================================================
 *
 * Filename:  binary_search.cpp
 * 
 * Author:    MNV Rohith
 *
 * Description:
 *  Interactive implementation of Binary Search Algorithm using both
 *  iterative and recursive methods.
 *
 * =====================================================================================
 *
 * Algorithm:  Binary Search
 * Language:   C++
 * Category:   Divide & Conquer
 *
 * Description:
 *  Binary Search efficiently locates a target element within a sorted array
 *  by repeatedly dividing the search interval in half.Binary Search only works
 *  correctly on sorted arrays, and by default, it assumes the array is sorted 
 *  in increasing (ascending) order.If the array were sorted in decreasing order (e.g., 38 23 16 12 8 5 2),
 *  then the comparison logic (arr[mid] < target) would need to be reversed.
 *
 * Note:
 *  The input array must be sorted in ascending order for the algorithm
 *  to work correctly.
 *
 * Time Complexity:  O(log n)
 * Space Complexity: O(1) for iterative, O(log n) for recursive
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
using namespace std;

// Iterative Binary Search
int binarySearchIterative(const vector<int>& arr, int target) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target){
            return mid;  // Found
        }else if (arr[mid] < target){
            low = mid + 1;
         } else{
            high = mid - 1;}
    }
    return -1;  // Not found
}

// Recursive Binary Search
int binarySearchRecursive(const vector<int>& arr, int low, int high, int target) {
    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;

    if (arr[mid] == target){
        return mid;
    }else if (arr[mid] < target){
        return binarySearchRecursive(arr, mid + 1, high, target);
    }else{
        return binarySearchRecursive(arr, low, mid - 1, target);}
}

int main() {
    cout << "NOTE: Please enter a sorted array in ascending order.\n\n";

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " sorted elements:\n";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    int target;
    cout << "Enter target element to search: ";
    cin >> target;

    cout << "\n--- Binary Search Results ---\n";

    // Iterative version
    int resultIter = binarySearchIterative(arr, target);
    if (resultIter != -1)
        cout << "Iterative Binary Search: Element found at index " << resultIter << "\n";
    else
        cout << "Iterative Binary Search: Element not found\n";

    // Recursive version
    int resultRec = binarySearchRecursive(arr, 0, n - 1, target);
    if (resultRec != -1)
        cout << "Recursive Binary Search: Element found at index " << resultRec << "\n";
    else
        cout << "Recursive Binary Search: Element not found\n";

    return 0;
}
