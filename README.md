# Tetris Game

A simple Tetris game implemented in C++ using basic console graphics and logic. This was largely possible thanks to [this](https://youtu.be/8OK8_tHeCIA?si=s-Qwhgbe2SkM95fB) video.

### Features
- Fully functional Tetris gameplay.
- Score tracking based on cleared lines.
- Next Piece overlay
- Coloured text-based graphics.
- Level based scoring system and colour schemes.
- Game speed increases after each level.

### Preview of the game (very proffesional gameplay)

![image](https://github.com/user-attachments/assets/87b0d18e-5900-4630-95c5-1150d916b6e2)   ![image](https://github.com/user-attachments/assets/909c5aef-0e01-451b-acc5-ff4ac3994d5d)

### Requirements
- A Windows environment with support for WinAPI (e.g., Windows 10 or newer).
- A C++ compiler (e.g., g++, clang++).
- Basic knowledge of the terminal/command prompt.

### How to Run
1. Clone or download the repository to your local machine.
2. Compile the program using your preferred C++ compiler. Example:
```
g++ -o Tetris Tetris.cpp
```
3. Run the compiled executable:
```
./Tetris
```

### Or
1. Download the .exe file to your machine.
2. Run it using terminal or cmd using :
```
start Tetris.exe
```

### Gameplay Instructions
- Use the following keys to control the game:
- Right Arrow: Move the piece to the right.
- Left Arrow: Move the piece to the left.
- Down Arrow: Move the piece down.
- Z: Rotate the piece counterclockwise.
- X: Rotate the piece clockwise.
- Clear lines by filling rows completely with blocks to earn points.
- The game ends when a new piece cannot spawn at the top of the board.

### Scoring
- Points are awarded for clearing lines:
- 1 line: 50 
- 2 lines: 100 
- 3 lines: 200 
- 4 lines: 400 
The score is updated after applying level multiplier.
Pushdown points are also awarded based on level.

### Code Highlights
- Tetromino rotation is implemented with a rotation matrix to handle different orientations.
- The game loop uses a fixed time-step for smooth gameplay.
- Console graphics are achieved using WinAPI functions (`SetConsoleTextAttribute` and `SetConsoleCursorInfo`).

### Customizations
- Modify `screenWidth` and `screenHeight` for different console sizes.
- Adjust the colors of tetromino pieces by editing the `COLORS` array.
- Change the game speed by altering `targetFrameTime` in `gameLoop`.

### Known Limitations
- This game is limited to Windows systems due to its use of WinAPI for console handling.
- The game logic does not include advanced Tetris mechanics like T-spins, hold functionality.

### Future Improvements
- Add a hold queue to allow players to swap pieces.
- Implement additional gameplay mechanics such as hard drop.
- Port the game to be platform-independent.
