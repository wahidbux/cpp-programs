// Author: Ayush Yadav
// GitHub: https://github.com/AyushYadav256
// Description: Number Guessing Game in C++ for MiniGames folder

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0)); // Seed for random number
    int secret = rand() % 100 + 1; // Random number between 1 and 100
    int guess;
    int attempts = 0;

    cout << "🎯 Welcome to the Number Guessing Game!" << endl;
    cout << "Guess a number between 1 and 100." << endl;

    while (true) {
        cout << "Enter your guess: ";
        cin >> guess;
        attempts++;

        if (guess < 1 || guess > 100) {
            cout << "❌ Invalid input. Please enter a number between 1 and 100." << endl;
            continue;
        }

        if (guess == secret) {
            cout << "✅ Correct! You guessed the number in " << attempts << " attempts." << endl;
            break;
        } else if (guess < secret) {
            cout << "🔼 Too low. Try again." << endl;
        } else {
            cout << "🔽 Too high. Try again." << endl;
        }
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}
