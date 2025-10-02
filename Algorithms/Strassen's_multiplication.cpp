#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>

/**
 * Strassen Matrix Multiplication Algorithm Implementation
 * * Strassen's algorithm is a divide-and-conquer algorithm for matrix multiplication
 * that is asymptotically faster than the standard O(n³) algorithm.
 * It reduces the number of recursive multiplications from 8 to 7.
 * * Time Complexity: O(n^log₂7) ≈ O(n^2.807)
 * Space Complexity: O(log n) for recursion stack + O(n²) for temporary matrices
 * * @author Hacktoberfest2025 Contributor
 */

// Define a matrix type alias for cleaner code (vector of vectors of ints)
using Matrix = std::vector<std::vector<int>>;

// Threshold below which to use standard multiplication
const int THRESHOLD = 64;

// --- Helper Functions Declaration ---

// Forward declarations for helper functions used by strassenRecursive
Matrix standardMultiply(const Matrix& A, const Matrix& B);
Matrix add(const Matrix& A, const Matrix& B);
Matrix subtract(const Matrix& A, const Matrix& B);
Matrix getSubMatrix(const Matrix& matrix, int row, int col, int size);
Matrix combineMatrices(const Matrix& C11, const Matrix& C12, const Matrix& C21, const Matrix& C22);
int nextPowerOfTwo(int n);
bool isValidForMultiplication(const Matrix& A, const Matrix& B);
Matrix padMatrix(const Matrix& matrix, int newSize);
Matrix extractMatrix(const Matrix& matrix, int rows, int cols);

// --- Core Strassen Logic ---

/**
 * Recursive implementation of Strassen's algorithm
 * * @param A First matrix (must be square and power of 2)
 * @param B Second matrix (must be square and power of 2)
 * @return Result matrix A × B
 */
Matrix strassenRecursive(const Matrix& A, const Matrix& B) {
    int n = A.size();

    // Base case: use standard multiplication for small matrices
    if (n <= THRESHOLD) {
        return standardMultiply(A, B);
    }

    int half = n / 2;

    // --- 1. Divide matrices into quadrants ---
    Matrix A11 = getSubMatrix(A, 0, 0, half);
    Matrix A12 = getSubMatrix(A, 0, half, half);
    Matrix A21 = getSubMatrix(A, half, 0, half);
    Matrix A22 = getSubMatrix(A, half, half, half);

    Matrix B11 = getSubMatrix(B, 0, 0, half);
    Matrix B12 = getSubMatrix(B, 0, half, half);
    Matrix B21 = getSubMatrix(B, half, 0, half);
    Matrix B22 = getSubMatrix(B, half, half, half);

    // --- 2. Calculate the 7 products (M1 to M7) using Strassen's formulas ---
    // M1 = (A11 + A22) * (B11 + B22)
    Matrix M1 = strassenRecursive(add(A11, A22), add(B11, B22)); 
    // M2 = (A21 + A22) * B11
    Matrix M2 = strassenRecursive(add(A21, A22), B11);
    // M3 = A11 * (B12 - B22)
    Matrix M3 = strassenRecursive(A11, subtract(B12, B22));
    // M4 = A22 * (B21 - B11)
    Matrix M4 = strassenRecursive(A22, subtract(B21, B11));
    // M5 = (A11 + A12) * B22
    Matrix M5 = strassenRecursive(add(A11, A12), B22);
    // M6 = (A21 - A11) * (B11 + B12)
    Matrix M6 = strassenRecursive(subtract(A21, A11), add(B11, B12));
    // M7 = (A12 - A22) * (B21 + B22)
    Matrix M7 = strassenRecursive(subtract(A12, A22), add(B21, B22));

    // --- 3. Calculate result quadrants (C11, C12, C21, C22) ---
    // C11 = M1 + M4 - M5 + M7
    Matrix C11 = add(subtract(add(M1, M4), M5), M7);
    // C12 = M3 + M5
    Matrix C12 = add(M3, M5);
    // C21 = M2 + M4
    Matrix C21 = add(M2, M4);
    // C22 = M1 + M3 - M2 + M6
    Matrix C22 = add(subtract(add(M1, M3), M2), M6);

    // --- 4. Combine quadrants into result matrix ---
    return combineMatrices(C11, C12, C21, C22);
}

/**
 * Multiply two matrices using Strassen's algorithm (main public method)
 * * @param A First matrix
 * @param B Second matrix
 * @return Result matrix A × B
 * @throws std::invalid_argument if matrices cannot be multiplied
 */
Matrix strassenMultiply(const Matrix& A, const Matrix& B) {
    // Validate input matrices
    if (!isValidForMultiplication(A, B)) {
        throw std::invalid_argument("Invalid matrices for multiplication: columns of A must equal rows of B.");
    }

    int aRows = A.size();
    int aCols = A[0].size();
    int bRows = B.size();
    int bCols = B[0].size();

    // Find the max dimension to determine the necessary padding size
    int maxDim = std::max({aRows, aCols, bRows, bCols});
    int paddedSize = nextPowerOfTwo(maxDim);

    // Pad matrices to be square with size power of 2
    Matrix paddedA = padMatrix(A, paddedSize);
    Matrix paddedB = padMatrix(B, paddedSize);

    // Run the recursive multiplication
    Matrix result = strassenRecursive(paddedA, paddedB);

    // Extract the actual result (remove padding)
    return extractMatrix(result, aRows, bCols);
}

// ----------------------------------
// --- IMPLEMENTATION OF HELPERS ---
// ----------------------------------

/**
 * Standard O(n³) matrix multiplication for base cases
 */
Matrix standardMultiply(const Matrix& A, const Matrix& B) {
    int rows = A.size();
    int cols = B[0].size();
    int common = B.size();

    Matrix result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < common; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

/**
 * Add two matrices (A + B)
 */
Matrix add(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix result(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

/**
 * Subtract two matrices (A - B)
 */
Matrix subtract(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix result(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

/**
 * Extract a submatrix from the given matrix
 */
Matrix getSubMatrix(const Matrix& matrix, int rowStart, int colStart, int size) {
    Matrix subMatrix(size, std::vector<int>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            subMatrix[i][j] = matrix[rowStart + i][colStart + j];
        }
    }

    return subMatrix;
}

/**
 * Combine four quadrant matrices into one matrix
 */
Matrix combineMatrices(const Matrix& C11, const Matrix& C12, const Matrix& C21, const Matrix& C22) {
    int half = C11.size();
    int n = 2 * half;
    Matrix result(n, std::vector<int>(n));

    // Copy C11 (Top-Left)
    for (int i = 0; i < half; i++) {
        std::copy(C11[i].begin(), C11[i].end(), result[i].begin());
    }

    // Copy C12 (Top-Right)
    for (int i = 0; i < half; i++) {
        std::copy(C12[i].begin(), C12[i].end(), result[i].begin() + half);
    }

    // Copy C21 (Bottom-Left)
    for (int i = 0; i < half; i++) {
        std::copy(C21[i].begin(), C21[i].end(), result[i + half].begin());
    }

    // Copy C22 (Bottom-Right)
    for (int i = 0; i < half; i++) {
        std::copy(C22[i].begin(), C22[i].end(), result[i + half].begin() + half);
    }

    return result;
}

/**
 * Pad matrix to make it square with size being power of 2,
 * filling new spaces with zeros.
 */
Matrix padMatrix(const Matrix& matrix, int newSize) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Initialize with zeros
    Matrix padded(newSize, std::vector<int>(newSize, 0)); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            padded[i][j] = matrix[i][j];
        }
    }

    return padded;
}

/**
 * Extract the actual result matrix (remove padding)
 */
Matrix extractMatrix(const Matrix& matrix, int rows, int cols) {
    Matrix result(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix[i][j];
        }
    }

    return result;
}

/**
 * Find the next power of 2 greater than or equal to n
 */
int nextPowerOfTwo(int n) {
    if (n <= 1) return 1;

    int power = 1;
    // Check if n is already a power of two
    if ((n & (n - 1)) == 0) return n;

    // Use bit manipulation (more efficient) to find the next power of two
    while (power < n) {
        power <<= 1; // equivalent to power *= 2;
    }

    return power;
    // An alternative way:
    // int power = 1;
    // while (power < n) power <<= 1;
    // return power;
}

/**
 * Check if two matrices can be multiplied (A[rows1][cols1] * B[rows2][cols2] => cols1 == rows2)
 */
bool isValidForMultiplication(const Matrix& A, const Matrix& B) {
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        return false;
    }

    return A[0].size() == B.size();
}

// -----------------------------------
// --- DEMONSTRATION & IO METHODS ---
// -----------------------------------

/**
 * Print a matrix in a formatted way
 */
void printMatrix(const Matrix& matrix, const std::string& name) {
    std::cout << "\n" << name << ":" << std::endl;

    if (matrix.empty() || matrix[0].empty()) {
        std::cout << "Matrix is empty." << std::endl;
        return;
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    if (rows > 10 || cols > 10) {
        std::cout << "Matrix is too large to display (" << rows << "x" << cols << ")" << std::endl;
        return;
    }

    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << std::setw(8) << val << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * Generate a random matrix for testing
 */
Matrix generateRandomMatrix(int rows, int cols, int maxValue) {
    Matrix matrix(rows, std::vector<int>(cols));
    // Use std::srand and std::rand for basic random numbers in C++
    // std::srand(std::time(0)); (usually called once in main)

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Generates a value between -maxValue and +maxValue
            matrix[i][j] = (std::rand() % (2 * maxValue + 1)) - maxValue;
        }
    }

    return matrix;
}

/**
 * Compare two matrices for equality
 */
bool matricesEqual(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size() || A[0].size() != B[0].size()) {
        return false;
    }

    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[0].size(); j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }

    return true;
}

void handleManualInput() {
    int rows1, cols1, rows2, cols2;

    std::cout << "Enter rows of first matrix: ";
    std::cin >> rows1;
    std::cout << "Enter columns of first matrix: ";
    std::cin >> cols1;

    std::cout << "Enter rows of second matrix: ";
    std::cin >> rows2;
    std::cout << "Enter columns of second matrix: ";
    std::cin >> cols2;

    if (cols1 != rows2) {
        std::cout << "Error: Cannot multiply matrices. Columns of first matrix must equal rows of second matrix." << std::endl;
        return;
    }

    Matrix A(rows1, std::vector<int>(cols1));
    Matrix B(rows2, std::vector<int>(cols2));

    std::cout << "Enter elements of first matrix (row by row):" << std::endl;
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            std::cin >> A[i][j];
        }
    }

    std::cout << "Enter elements of second matrix (row by row):" << std::endl;
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            std::cin >> B[i][j];
        }
    }

    std::clock_t startTime = std::clock();
    Matrix result = strassenMultiply(A, B);
    double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC * 1000.0; // convert to ms

    printMatrix(A, "Matrix A");
    printMatrix(B, "Matrix B");
    printMatrix(result, "Result (A x B)");

    std::cout << "\nTime taken: " << duration << " ms" << std::endl;
}

void handleRandomMatrices() {
    int n, maxValue;

    std::cout << "Enter matrix size (n for n x n matrices): ";
    std::cin >> n;
    std::cout << "Enter maximum value for random elements: ";
    std::cin >> maxValue;

    Matrix A = generateRandomMatrix(n, n, maxValue);
    Matrix B = generateRandomMatrix(n, n, maxValue);

    std::cout << "Generated random " << n << " x " << n << " matrices" << std::endl;

    if (n <= 10) {
        printMatrix(A, "Matrix A");
        printMatrix(B, "Matrix B");
    }

    std::clock_t startTime = std::clock();
    Matrix result = strassenMultiply(A, B);
    double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC * 1000.0; // convert to ms

    if (n <= 10) {
        printMatrix(result, "Result (A x B)");
    }

    std::cout << "Time taken: " << duration << " ms" << std::endl;
}

void performanceComparison() {
    std::cout << "\n=== Performance Comparison: Strassen vs Standard ===" << std::endl;

    // Use a fixed seed for repeatable random matrices
    std::srand(std::time(0)); 

    std::vector<int> sizes = {64, 128, 256, 512};

    for (int n : sizes) {
        std::cout << "\nMatrix size: " << n << " x " << n << std::endl;

        Matrix A = generateRandomMatrix(n, n, 100);
        Matrix B = generateRandomMatrix(n, n, 100);

        // Test Strassen algorithm
        std::clock_t startTime = std::clock();
        Matrix strassenResult = strassenMultiply(A, B);
        double strassenTime = (std::clock() - startTime) / (double) CLOCKS_PER_SEC * 1000.0;

        // Test standard algorithm
        startTime = std::clock();
        Matrix standardResult = standardMultiply(A, B);
        double standardTime = (std::clock() - startTime) / (double) CLOCKS_PER_SEC * 1000.0;

        // Verify results are the same
        bool resultsMatch = matricesEqual(strassenResult, standardResult);

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Strassen time: " << strassenTime << " ms" << std::endl;
        std::cout << "Standard time: " << standardTime << " ms" << std::endl;
        
        if (strassenTime > 0) {
             std::cout << "Speedup: " << standardTime / strassenTime << "x" << std::endl;
        } else {
             std::cout << "Speedup: N/A (Time too short)" << std::endl;
        }
        
        std::cout << "Results match: " << (resultsMatch ? "true" : "false") << std::endl;
    }
}

/**
 * Main method to demonstrate Strassen Matrix Multiplication
 */
int main() {
    // Seed the random number generator once
    std::srand(std::time(0)); 
    
    int choice;

    std::cout << "=== Strassen Matrix Multiplication Algorithm ===" << std::endl;
    std::cout << "1. Manual input" << std::endl;
    std::cout << "2. Random matrices" << std::endl;
    std::cout << "3. Performance comparison" << std::endl;
    std::cout << "Choose an option (1-3): ";

    if (!(std::cin >> choice)) {
        // Handle non-integer input
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    try {
        switch (choice) {
            case 1:
                handleManualInput();
                break;
            case 2:
                handleRandomMatrices();
                break;
            case 3:
                performanceComparison();
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::bad_alloc& e) {
        // Catch memory allocation errors for very large matrices
        std::cerr << "Memory Allocation Error: Matrix size likely too large." << std::endl;
    }
    
    return 0;
}