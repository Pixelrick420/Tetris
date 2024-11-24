# Tetris Game

A simple Tetris game implemented in C++ using basic console graphics and logic.

**Features**
- Fully functional Tetris gameplay.
- Rotatable tetromino pieces in various shapes.
- Score tracking based on cleared lines.
- Game-over detection.
- Minimalistic text-based graphics.

**Requirements**
- A Windows environment with support for WinAPI (e.g., Windows 10 or newer).
- A C++ compiler (e.g., g++, clang++).
- Basic knowledge of the terminal/command prompt.

**How to Run**
1. Clone or download the repository to your local machine.
2. Compile the program using your preferred C++ compiler. Example:
```
g++ -o Tetris Tetris.cpp
```
3. Run the compiled executable:
```
./Tetris
```

**Gameplay Instructions**
- Use the following keys to control the game:
- Right Arrow: Move the piece to the right.
- Left Arrow: Move the piece to the left.
- Z: Rotate the piece counterclockwise.
- X: Rotate the piece clockwise.
- Q: Quit the game.
- Clear lines by filling rows completely with blocks to earn points.
- The game ends when a new piece cannot spawn at the top of the board.

**Scoring**
- Points are awarded for clearing lines:
- 1 line: 50 points
- 2 lines: 100 points
- 3 lines: 200 points
- 4 lines: 400 points

**Code Highlights**
- Tetromino rotation is implemented with a rotation matrix to handle different orientations.
- The game loop uses a fixed time-step for smooth gameplay.
- Console graphics are achieved using WinAPI functions (`SetConsoleTextAttribute` and `SetConsoleCursorInfo`).

**Customizations**
- Modify `screenWidth` and `screenHeight` for different console sizes.
- Adjust the colors of tetromino pieces by editing the `COLORS` array.
- Change the game speed by altering `targetFrameTime` in `gameLoop`.

**Known Limitations**
- This game is limited to Windows systems due to its use of WinAPI for console handling.
- The game logic does not include advanced Tetris mechanics like T-spins, hold functionality or next piece.

**Future Improvements**
- Add a hold queue to allow players to swap pieces.
- Implement additional gameplay mechanics such as soft drop and hard drop.
- Port the game to be platform-independent.
