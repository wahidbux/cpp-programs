#include <iostream> // Including the input-output stream header file

using namespace std; // Using the standard namespace

int main() // Start of the main function
{
    cout << "\n\n Find Size of fundamental data types :\n"; // Outputting a message for finding the size of fundamental data types
	cout << "------------------------------------------\n"; // Outputting a separator line
    cout << " The sizeof(char) is :          " << sizeof(char) << " bytes \n" ; // Outputting the size of char
    cout << " The sizeof(short) is :         " << sizeof(short) << " bytes \n" ; // Outputting the size of short
    cout << " The sizeof(int) is :           " << sizeof(int) << " bytes \n" ; // Outputting the size of int
    cout << " The sizeof(long) is :          " << sizeof(long) << " bytes \n" ; // Outputting the size of long
    cout << " The sizeof(long long) is :     " << sizeof(long long) << " bytes \n"; // Outputting the size of long long
    cout << " The sizeof(float) is :         " << sizeof(float) << " bytes \n" ; // Outputting the size of float
    cout << " The sizeof(double) is :        " << sizeof(double) << " bytes \n"; // Outputting the size of double
    cout << " The sizeof(long double) is :   " << sizeof(long double) << " bytes \n"; // Outputting the size of long double
    cout << " The sizeof(bool) is :          " << sizeof(bool) << " bytes \n\n"; // Outputting the size of bool

    return 0; // Returning 0 to indicate successful program execution
}
