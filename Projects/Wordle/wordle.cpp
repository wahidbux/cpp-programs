// (Paste this entire file as wordle.cpp)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <random>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#define WORD_LEN 5
#define MAX_ATTEMPTS 6

// ANSI color codes
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define GREY   "\033[1;90m"
#define RESET  "\033[0m"

// Alternative symbols for systems without color support
#define GREEN_ALT  "✓"
#define YELLOW_ALT "?"
#define GREY_ALT   "✗"

// Keyboard states
enum LetterState { UNKNOWN, GREYED, YELLOWED, GREENED };

struct Stats {
    int played = 0;
    int won = 0;
    int streak = 0;
    int best = 0;
};

class WordleGame {
private:
    std::vector<std::string> wordList = {
        "apple","river","storm","plane","bring",
        "table","crane","peace","light","sound",
        "brave","flame","sword","track","sharp"
    };

    std::string targetWord;
    std::vector<std::pair<std::string, std::vector<char>>> board;
    std::unordered_map<char, LetterState> keyboard;
    Stats stats;
    bool hardMode = true;

    // --- Utility Functions ---
    std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return s;
    }

    void pickRandomWord() {
        static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
        std::uniform_int_distribution<int> dist(0, (int)wordList.size() - 1);
        targetWord = wordList[dist(rng)];
    }

    bool isValidWord(const std::string &word) {
        return std::find(wordList.begin(), wordList.end(), word) != wordList.end();
    }

    void loadStats() {
        std::ifstream file("stats.txt");
        if (file)
            file >> stats.played >> stats.won >> stats.streak >> stats.best;
    }

    void saveStats() {
        std::ofstream file("stats.txt");
        file << stats.played << " " << stats.won << " "
             << stats.streak << " " << stats.best;
    }

    // --- Game Logic ---
    std::vector<char> evaluateGuess(const std::string &guess) {
        std::vector<bool> used(WORD_LEN, false);
        std::vector<char> feedback(WORD_LEN, ' ');

        // Mark greens
        for (int i = 0; i < WORD_LEN; ++i) {
            if (guess[i] == targetWord[i]) {
                feedback[i] = 'G';
                used[i] = true;
            }
        }

        // Mark yellows
        for (int i = 0; i < WORD_LEN; ++i) {
            if (feedback[i] == 'G') continue;
            for (int j = 0; j < WORD_LEN; ++j) {
                if (!used[j] && guess[i] == targetWord[j]) {
                    feedback[i] = 'Y';
                    used[j] = true;
                    break;
                }
            }
        }

        updateKeyboard(guess, feedback);
        return feedback;
    }

    void updateKeyboard(const std::string &guess, const std::vector<char> &feedback) {
        for (int i = 0; i < WORD_LEN; ++i) {
            char c = std::toupper(guess[i]);
            if (feedback[i] == 'G')
                keyboard[c] = GREENED;
            else if (feedback[i] == 'Y' && keyboard[c] != GREENED)
                keyboard[c] = YELLOWED;
            else if (feedback[i] == ' ')
                if (keyboard[c] != GREENED && keyboard[c] != YELLOWED)
                    keyboard[c] = GREYED;
        }
    }

    void printKeyboard() {
        std::string rows[] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
        for (auto &row : rows) {
            for (char c : row) {
                switch (keyboard[c]) {
                    case GREENED: std::cout << GREEN << c << " " << RESET; break;
                    case YELLOWED: std::cout << YELLOW << c << " " << RESET; break;
                    case GREYED: std::cout << GREY << c << " " << RESET; break;
                    default: std::cout << c << " "; break;
                }
            }
            std::cout << "\n";
        }
    }

    void printBoard() {
        std::cout << "\n";
        for (const auto &entry : board) {
            const std::string &word = entry.first;
            const std::vector<char> &feedback = entry.second;
            for (int i = 0; i < WORD_LEN; ++i) {
                char ch = std::toupper(word[i]);
                if (feedback[i] == 'G')
                    std::cout << GREEN << "[" << ch << "]" << RESET;
                else if (feedback[i] == 'Y')
                    std::cout << YELLOW << "[" << ch << "]" << RESET;
                else
                    std::cout << GREY << "[" << ch << "]" << RESET;
            }
            std::cout << "\n";
        }
        for (int i = (int)board.size(); i < MAX_ATTEMPTS; ++i)
            std::cout << "[_] [_] [_] [_] [_]\n";
    }

    // Updated hard-mode enforcement: aggregate hints from *all* previous guesses
    bool obeysHardMode(const std::string &guess) {
        if (!hardMode || board.empty()) return true;

        // 1) required green positions ('.' = no requirement)
        std::string requiredPos(WORD_LEN, '.');

        // 2) required minimum counts for letters revealed as green or yellow
        std::unordered_map<char, int> requiredCounts;

        for (const auto &entry : board) {
            const std::string &prevWord = entry.first;
            const std::vector<char> &feedback = entry.second;
            for (int i = 0; i < WORD_LEN; ++i) {
                char ch = std::toupper(prevWord[i]);
                if (feedback[i] == 'G') {
                    requiredPos[i] = ch;
                    requiredCounts[ch]++; // this green contributes to required count
                } else if (feedback[i] == 'Y') {
                    requiredCounts[ch]++; // yellow contributes to required count
                }
            }
        }

        // Check green positions are preserved
        for (int i = 0; i < WORD_LEN; ++i) {
            if (requiredPos[i] != '.' && std::toupper(guess[i]) != requiredPos[i]) {
                std::cout << GREY << "Hard mode: must keep green letter "
                          << requiredPos[i] << " at position " << (i + 1) << "\n" << RESET;
                return false;
            }
        }

        // Check required counts: guess must include at least the required number of each letter
        std::unordered_map<char,int> guessCounts;
        for (char c : guess) guessCounts[std::toupper(c)]++;

        for (const auto &p : requiredCounts) {
            char ch = p.first;
            int needed = p.second;
            int have = guessCounts[ch];
            if (have < needed) {
                std::cout << GREY << "Hard mode: guess must include at least "
                          << needed << " instance(s) of '" << ch << "' (revealed earlier)\n" << RESET;
                return false;
            }
        }

        return true;
    }

public:
    WordleGame() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
        loadStats();
        pickRandomWord();
    }

    void play() {
        std::cout << "\n===== WORDLE (C++) ENHANCED =====\n";
        std::cout << "Guess the " << WORD_LEN << "-letter word in " << MAX_ATTEMPTS << " attempts!\n";
        std::cout << (hardMode ? GREY "(Hard Mode ON)\n" RESET : "") << "\n";

        auto start = std::chrono::steady_clock::now();
        int attempts = 0;
        bool won = false;

        while (attempts < MAX_ATTEMPTS) {
            printBoard();
            std::cout << "\n";
            printKeyboard();

            std::cout << "\nAttempt " << (attempts + 1) << "/" << MAX_ATTEMPTS << ": ";
            std::string guess;
            std::getline(std::cin, guess);
            guess = toLower(guess);

            if (guess.length() != WORD_LEN) {
                std::cout << GREY << "Word must be " << WORD_LEN << " letters.\n" << RESET;
                continue;
            }

            if (!isValidWord(guess)) {
                std::cout << GREY << "Invalid word. Try again.\n" << RESET;
                continue;
            }

            if (!obeysHardMode(guess))
                continue;

            auto feedback = evaluateGuess(guess);
            board.push_back({guess, feedback});
            attempts++;

            // Animated feedback reveal (sleep_for instead of busy-wait)
            std::cout << "Result: ";
            for (int i = 0; i < WORD_LEN; ++i) {
                // std::this_thread::sleep_for(std::chrono::milliseconds(150));
                std::clock_t start_time = std::clock();
while (static_cast<double>(std::clock() - start_time) / CLOCKS_PER_SEC < 0.15) {}

                char ch = std::toupper(guess[i]);
                if (feedback[i] == 'G')
                    std::cout << GREEN << ch << " " << RESET;
                else if (feedback[i] == 'Y')
                    std::cout << YELLOW << ch << " " << RESET;
                else
                    std::cout << GREY << ch << " " << RESET;
                std::cout.flush();
            }
            std::cout << "\n";

            if (guess == targetWord) {
                won = true;
                break;
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto timeTaken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

        stats.played++;
        if (won) {
            stats.won++;
            stats.streak++;
            stats.best = std::max(stats.best, stats.streak);
            std::cout << GREEN << "\n🎉 You guessed it in " << attempts << " attempts!\n" << RESET;
        } else {
            stats.streak = 0;
            std::cout << "\nThe correct word was: " << YELLOW << targetWord << RESET << "\n";
            std::cout << GREY << "❌ Better luck next time!\n" << RESET;
        }

        std::cout << GREY << "Time taken: " << timeTaken << "s\n" << RESET;

        saveStats();
        std::cout << "\n📊 Stats: Played " << stats.played
                  << " | Won " << stats.won
                  << " | Win% " << std::fixed << std::setprecision(1)
                  << (stats.played ? (100.0 * stats.won / stats.played) : 0)
                  << "% | Streak " << stats.streak
                  << " | Best " << stats.best << "\n";
    }
};

int main() {
    char again;

    do {
        WordleGame game;
        game.play();

        std::cout << "\nPlay again? (Y/N): ";
        std::cin >> again;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        again = std::tolower(again);

    } while (again == 'y');

    std::cout << "\nThanks for playing Wordle++ in C++!\n";
    return 0;
}
