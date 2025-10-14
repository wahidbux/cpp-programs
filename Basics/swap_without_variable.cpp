/*
Program to swap two numbers without using a temporary variable.
*/

#include <iostream>
using namespace std;

int main() 
{
    int a, b;

    // Input two numbers
    cout << "Enter first number (a): ";
    cin >> a;
    cout << "Enter second number (b): ";
    cin >> b;

    // Display before swap
    cout << "\nBefore swapping:\n";
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    // Swapping without temp variable
    a = a + b;
    b = a - b;
    a = a - b;

    // Display after swap
    cout << "\nAfter swapping:\n";
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    return 0;
}
