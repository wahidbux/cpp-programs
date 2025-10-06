# Wordle C++ - Enhanced Console Game

A feature-rich implementation of the popular Wordle game in C++ with advanced features including hard mode, statistics tracking, and animated feedback.

## 🎮 Features

- **Classic Wordle Gameplay**: Guess a 5-letter word in 6 attempts
- **Hard Mode**: Must reuse yellow letters and keep green letters in place
- **Visual Feedback**: Color-coded letters (Green = correct position, Yellow = wrong position, Grey = not in word)
- **Animated Reveals**: Letters appear one by one with timing effects
- **Statistics Tracking**: Win rate, current streak, and best streak
- **Persistent Stats**: Game statistics saved to file
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **ANSI Color Support**: Automatically detects and enables color on Windows

## 🚀 Quick Start

### Prerequisites

- C++ compiler with C++11 support or higher
- Standard library support for chrono, thread, and file operations

### Compilation

**Using g++:**

```bash
g++ -std=c++11 -o wordle wordle.cpp
```

**Using clang++:**

```bash
clang++ -std=c++11 -o wordle wordle.cpp
```

**Using Visual Studio (Windows):**

```cmd
cl /EHsc wordle.cpp
```

### Running the Game

```bash
./wordle
```

## 🎯 How to Play

1. **Start the game** - The program will select a random 5-letter word
2. **Enter your guess** - Type a 5-letter word and press Enter
3. **Read the feedback**:
   - 🟩 **Green**: Letter is correct and in the right position
   - 🟨 **Yellow**: Letter is in the word but wrong position  
   - ⬜ **Grey**: Letter is not in the word
4. **Use the keyboard display** - Shows which letters you've tried
5. **Hard Mode** (default): Must reuse yellow letters and keep green letters

## 🛠️ Technical Details

### Game Architecture

- **WordleGame Class**: Main game logic and state management
- **Stats Structure**: Persistent statistics tracking
- **Cross-Platform Color**: Windows console color enablement
- **Memory Management**: Efficient use of STL containers

### Key Components

```cpp
class WordleGame {
private:
    std::vector<std::string> wordList;              // Available words
    std::string targetWord;                         // Current target
    std::vector<std::pair<std::string, std::vector<char>>> board;  // Game board
    std::unordered_map<char, LetterState> keyboard; // Letter states
    Stats stats;                                    // Game statistics
    bool hardMode;                                  // Difficulty setting
};
```

### Features Implementation

- **Word Validation**: Checks against predefined word list
- **Feedback Algorithm**: Handles duplicate letters correctly
- **Keyboard Tracking**: Visual representation of tried letters
- **File I/O**: Persistent statistics storage
- **Animation**: Timed letter reveals for dramatic effect

## 📊 Statistics

The game tracks and displays:

- **Games Played**: Total number of games
- **Games Won**: Successful completions
- **Win Percentage**: Success rate calculation
- **Current Streak**: Consecutive wins
- **Best Streak**: Highest consecutive wins ever

Statistics are automatically saved to `stats.txt` in the same directory.

## 🎨 Customization

### Adding Words

Modify the `wordList` vector in the constructor:

```cpp
std::vector<std::string> wordList = {
    "apple", "river", "storm", "plane", "bring",
    // Add your words here
    "custom", "words", "here"
};
```

### Adjusting Difficulty

Change game constants at the top of the file:

```cpp
#define WORD_LEN 5        // Word length
#define MAX_ATTEMPTS 6    // Number of attempts
```

### Color Customization

Modify ANSI color codes:

```cpp
#define GREEN  "\033[1;32m"  // Bright green
#define YELLOW "\033[1;33m"  // Bright yellow  
#define GREY   "\033[1;90m"  // Bright black (grey)
#define RESET  "\033[0m"     // Reset to default
```

## 🔧 Troubleshooting

### Colors Not Working

- **Windows**: The program automatically enables ANSI color support
- **Linux/Mac**: Should work by default in most terminals
- **Fallback**: Modify color definitions to use plain text symbols

### Compilation Issues

- **C++11 Required**: Ensure your compiler supports C++11 or higher
- **Missing Headers**: All required headers are included in the source
- **Linking**: Standard library linking should be automatic

### File Permissions

- Ensure the program can write to `stats.txt` in the current directory
- Run with appropriate permissions if needed

## 🚀 Building with Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = wordle
SOURCE = wordle.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) stats.txt

.PHONY: clean
```

## 📈 Performance

- **Memory Usage**: Minimal memory footprint using efficient STL containers
- **Speed**: Fast word lookup and feedback generation
- **Scalability**: Easily expandable word list and game features

## 🤝 Contributing

This implementation is part of the **cpp-programs** repository for Hacktoberfest 2025!

### Contribution Ideas

- Add more word lists (different languages, themes)
- Implement multiplayer functionality
- Add sound effects (system beeps)
- Create GUI version using FLTK or Qt
- Add word definitions from dictionary API
- Implement different word lengths (4, 6, 7 letters)
- Add daily challenge mode with fixed words

## 📄 License

This project is part of the cpp-programs repository. Please check the main repository for license information.

## 🎉 Acknowledgments

- Inspired by the original Wordle game by Josh Wardle
- Built for educational purposes and Hacktoberfest 2025
- Uses modern C++ features while maintaining compatibility

---

**Happy Wordling! 🎮✨**