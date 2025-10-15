// You are climbing a staircase. It takes n steps to reach the top. 
// Each time you can climb 1 or 2 steps. In how many distinct ways can you climb to the top?

#include <iostream>
#include <vector>
using namespace std;

// Function to calculate the number of ways to climb n stairs
int climbStairs(int n) {
    if (n <= 2) return n;

    vector<int> dp(n + 1);
    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

int main() {
    int n;
    cout << "Enter the number of stairs: ";  cin >> n;

    if (n <= 0) {
        cout << "Number of stairs must be a positive integer." << endl;
        return 1;
    }
    cout << "Number of distinct ways to climb to the top: " << climbStairs(n) << endl;
    return 0;
}
