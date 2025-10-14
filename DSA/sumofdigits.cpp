#include <bits/stdc++.h>
using namespace std;

/*
 * sum_of_digits.cpp
 *
 * A program to find the sum of digits of a number.
 * Example:
 *   Input: 1234
 *   Output: 10
 */

int main() {
    long long n;
    cin >> n;

    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }

    cout << sum;
    return 0;
}
