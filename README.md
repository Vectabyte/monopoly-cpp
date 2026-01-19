# Monopoly C++ (monopoly-cpp)

A command-line implementation of Monopoly in C++, designed to be played through a Text User Interface (TUI). This project is built for educational purposes and aims to simulate the classic board game with a custom set of features and rules.

## Recommendations

For optimal display of the gameboard, use a **monospace font**. Without a monospace font, the board might appear misaligned.

## Compatibility

This project was developed and tested on Linux. Due to hardware limitations, testing on macOS was not performed.

### Windows Compatibility:
Windows may encounter issues with UTF-8 characters in the console. While it may work depending on fonts and terminal settings, some symbols might not display correctly.

## Compile Instructions

To compile the game, follow these instructions:

### Using g++ (Linux):

```bash
g++ monopoly.cpp -o monopoly --std=c++11
```

### Using CMake (Linux):

1. Create a build directory:

   ```bash
   mkdir build
   cd build
   ```

2. Run CMake:

   ```bash
   cmake ..
   ```

3. Build the project:

   ```bash
   cmake --build .
   ```

## How to Play

Upon starting the game you will either be prompted to load the last saved game or to set up players for a new game. Menu's can be navigated with their respective numbered entry. 

### Example Menu:

```
Do you want to load last saved game?
┌────────┬────────┐
│ 1: YES │ 0: NO  │
└────────┴────────┘
```

Here, entering "1" will load the last saved game, while entering "0" starts a new game.

### Player Setup:

Each player chooses a name, symbol, and color. The color will be used to highlight the player's position on the gameboard.
This step is skipped, if loading a game from file.

### Turn Sequence:

1. The turn order is randomized.
2. Each player takes their turn, and during the turn, they can perform various actions like moving, buying properties, or paying rent.
3. The game continues in this way until a player wins or all players agree to quit.

## Load and Save

* **Saving**: The game automatically saves when quitting early, and there is a failsafe that ensures the game will save if invalid input is given.
* **Loading**: You can load a previously saved game if all the necessary `.csv` files (like `Players.csv`, `Tiles.csv`, etc.) are located in the same folder.

## Deviations from the Original Base Game

While the game closely follows the rules of Monopoly, a few small deviations were implemented to enhance the gameplay experience:

* **Railroad and Utility Cards**: In the original game, players are required to pay double the rent or 10x the rolled die amount when landing on certain cards. In this version, we calculate rent using the standard rules.
* **Bankruptcy and Cash Raising**: When a player goes bankrupt, there is an option to try to raise cash, but selling properties to other players is not yet implemented.
* **Free Parking**: When landing on the "Free Parking" space, players receive money accumulated in the parking. This is a **house rule** commonly used in Monopoly but is not part of the original rules.

## Missing Features

The following feature is not yet implemented:

* **Player vs. Bot Mode**: AI-controlled bots are not available, as developing a competitive AI for Monopoly is complex. A purely random decision-making AI could lead to repetitive or boring gameplay.

## Files

The following files are part of the project:

* **monopoly.cpp**: Contains the main game loop and logic for playing the game.
* **FileImport.hpp**: Handles loading data from CSV files (e.g., game board, player data).
* **FileExport.hpp**: Handles saving data to CSV files.
* **player.hpp**: Defines the `player` struct and methods for player setup and actions.
* **tile.hpp**: Defines the `tile` struct and methods for setting up the game board.
* **card.hpp**: Defines the structures and functionality for Community and Chance cards.
