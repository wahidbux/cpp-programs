// Sudoku_Solver.cpp
// Author: Ayush Kumar Anand 
// Description: Sudoku Solver using Backtracking
// Compile: g++ -std=c++17 Algorithms\Sudoku_Solver.cpp -o Sudoku_Solver.exe
// Run: .\Sudoku_Solver.exe < input.txt


#include <bits/stdc++.h>
using namespace std;

const int N = 9;

// Function to print the Sudoku board
void printBoard(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << grid[row][col] << " ";
        }
        cout << "\n";
    }
}

// Check if placing num at (row, col) is valid
bool isSafe(int grid[N][N], int row, int col, int num) {
    // Row check
    for (int x = 0; x < N; x++)
        if (grid[row][x] == num) return false;

    // Column check
    for (int x = 0; x < N; x++)
        if (grid[x][col] == num) return false;

    // 3x3 subgrid check
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

// Solve Sudoku using backtracking
bool solveSudoku(int grid[N][N], int row, int col) {
    // If reached end of board → solved
    if (row == N - 1 && col == N)
        return true;

    // Move to next row if column exceeds
    if (col == N) {
        row++;
        col = 0;
    }

    // Skip filled cells
    if (grid[row][col] != 0)
        return solveSudoku(grid, row, col + 1);

    // Try numbers 1–9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num; // Place number

            // Recur for next cell
            if (solveSudoku(grid, row, col + 1))
                return true;

            // Backtrack if fails
            grid[row][col] = 0;
        }
    }
    return false;
}

int main() {
    int grid[N][N];

    // Input: Sudoku with 0 as empty cell
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> grid[i][j];

    if (solveSudoku(grid, 0, 0))
        printBoard(grid);
    else
        cout << "No solution exists\n";

    return 0;
}

