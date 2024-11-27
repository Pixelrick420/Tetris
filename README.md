# Tetris Game

A simple Tetris game implemented in C++ using basic console graphics and logic. This was largely possible thanks to [this](https://youtu.be/8OK8_tHeCIA?si=s-Qwhgbe2SkM95fB) video by javidx9.

### Features
- Fully functional Tetris gameplay.
- Delayed Auto Shift input handling.(kind of)
- Level selector menu and next piece overlay
- Ascii art console graphics.
- Level based scoring system, game speed and colour schemes.

### Preview of the game (very proffesional gameplay)

![image](https://github.com/user-attachments/assets/ab3ce152-c78e-4726-bf3b-1d94fb66c282)        ![image](https://github.com/user-attachments/assets/2cdec77c-2874-417e-b86e-e03157433ee6)

### Level Select Menu
![image](https://github.com/user-attachments/assets/858f52cc-0afe-4297-af03-b2d135a38917)


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
Tetris.exe
```

### Gameplay Instructions
- Right Arrow: Change focus in level select menu or Move the piece to the right in game.
- Left Arrow: Change focus in level select menu or  Move the piece to the left in game.
- Down Arrow: Decrement selected level in level select menu or Move the piece down in game.
- Up Arrow: Increment selected level in level select menu.
- Z: Rotate the piece counter clockwise in game.
- X: Rotate the piece clockwise in game.
- P: Pause the game.
- Q: Quit / Exit program.

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
- Implement additional gameplay mechanics such as hard drop and hold queue.
- Port the game to be platform-independent.
