#include <iostream>
#include <cstdlib>
#include <chrono>
#include <conio.h>
#include <windows.h>

int screenWidth = 80;
int screenHeight = 40;

std::wstring tetromino[7];

const int width = 10;
const int height = 25;
unsigned int board[height][width] = {0}; // 0 empty, 1 block

int level = 0;
int score = 0;
int scoreThreshold[10] = {500, 1750, 4000, 9000, 15000, 25000, 40000, 80000, 150000, 500000};

void init()
{
    tetromino[0].append(L"........■■■■...."); // line
    tetromino[1].append(L".....■■■...■...."); // L
    tetromino[2].append(L".....■■■.■......"); // J
    tetromino[3].append(L".....■■..■■....."); // Square
    tetromino[4].append(L"......■■.■■....."); // S
    tetromino[5].append(L".....■■■..■....."); // T
    tetromino[6].append(L".....■■...■■...."); // Z

    std::cout << "\033[H";
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            std::cout << "  ";
        }
    }
    std::cout << "\033[H";
}

int rotatePiece(int px, int py, int r)
{
    switch (r % 4)
    {
    case 0:
        return py * 4 + px; // 0 degrees
    case 1:
        return 12 + py - (px * 4); // 90 degrees
    case 2:
        return 15 - (py * 4) - px; // 180 degrees
    case 3:
        return 3 - py + (px * 4); // 270 degrees
    }
    return 0;
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

const int COLORS[5][8] = {
    {
        0,  // Black/Empty
        15, // Line
        2,  // L
        4,  // J
        15, // Square
        4,  // S
        15, // T
        2   // Z
    },
    {
        0,  // Black/Empty
        11, // Line
        15, // L
        12, // J
        11, // Square
        12, // S
        11, // T
        15  // Z
    },
    {
        0,  // Black/Empty
        13, // Line
        11, // L
        7,  // J
        13, // Square
        7,  // S
        13, // T
        11  // Z
    },
    {
        0,  // Black/Empty
        2,  // Line
        6,  // L
        15, // J
        2,  // Square
        15, // S
        2,  // T
        6   // Z
    },
    {
        0,  // Black/Empty
        15, // Line
        4,  // L
        1,  // J
        15, // Square
        1,  // S
        15, // T
        4   // Z
    }};

bool canMove(int piece, int rotation, int x, int y)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pieceIndex = rotatePiece(px, py, rotation);
            int boardX = x + px, boardY = y + py;

            if (tetromino[piece][pieceIndex] != L'.')
            {
                if (boardX < 0 || boardX >= width || boardY < 0 || boardY >= height)
                    return false;
                if (board[boardY][boardX] != 0)
                    return false;
            }
        }
    }
    return true;
}

void placePiece(int piece, int rotation, int x, int y, bool remove)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pieceIndex = rotatePiece(px, py, rotation);
            if (tetromino[piece][pieceIndex] != L'.')
            {
                board[y + py][x + px] = remove ? 0 : piece + 1;
            }
        }
    }
}

int clearLines()
{
    int targetRow = height - 1;
    int linesCleared = 0;

    for (int i = height - 1; i >= 0; i--)
    {
        bool isFull = true;
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] == 0)
            {
                isFull = false;
                break;
            }
        }

        if (!isFull)
        {
            for (int j = 0; j < width; j++)
            {
                board[targetRow][j] = board[i][j];
            }
            targetRow--;
        }
        else
        {
            linesCleared++;
        }
    }

    return ((1 << linesCleared) * 50 * (level + 1));
}

int nextPiece;

void displayBoard(int currentPiece, int currentRotation, int currentX, int currentY, std::string message)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << "\033[H";

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            SetConsoleTextAttribute(hConsole, COLORS[level % 5][board[i][j]]);
            std::cout << (board[i][j] == 0 ? " ." : "[]");
            SetConsoleTextAttribute(hConsole, 7);
        }
        std::cout << "  ";

        if ((i >= 4) && (i < 8))
        {
            for (int px = 0; px < 4; px++)
            {
                int pieceIndex = rotatePiece(px, i - 4, 0);
                if (tetromino[nextPiece][pieceIndex] != L'.')
                {
                    SetConsoleTextAttribute(hConsole, COLORS[level % 5][nextPiece + 1]);
                    std::cout << "[]";
                }
                else
                {
                    std::cout << "  ";
                }
            }
        }

        if (i == 3)
        {
            std::cout << "NEXT PIECE ";
        }
        if (i == 1)
        {
            if (message.empty())
            {
                std::cout << "LEVEL : " << level;
            }
            else
            {
                std::cout << message;
            }
        }
        if (i == 2)
        {
            std::cout << "SCORE : " << score;
        }
        std::cout << "\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
}

void gameLoop()
{
    bool rotateLeft = false;
    bool rotateRight = false;
    const int targetFrameTime = 40;

    int currentPiece = rand() % 7;
    nextPiece = rand() % 7;
    int gametick = 0;
    int animationticks = 0;

    while (true)
    {
        int currentRotation = 0;
        int currentX = width / 2 - 2;
        int currentY = 0;
        bool gameOver = false;

        while (!gameOver)
        {
            auto frameStart = std::chrono::steady_clock::now();
            placePiece(currentPiece, currentRotation, currentX, currentY, true);

            bool bKey[6] = {false};
            for (int k = 0; k < 6; k++)
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28XZQ"[k]))) != 0;

            if ((level > 5 || gametick % 2 == 0) && bKey[0] && canMove(currentPiece, currentRotation, currentX + 1, currentY))
                currentX++;
            if ((level > 5 || gametick % 2 == 0) && bKey[1] && canMove(currentPiece, currentRotation, currentX - 1, currentY))
                currentX--;
            if (gametick % 3 == 0 && bKey[2] && canMove(currentPiece, currentRotation, currentX, currentY + 1))
            {
                currentY++;
                score += level;
            }

            if (bKey[3] && !rotateLeft)
            {
                int newRotation = (currentRotation - 1 + 4) % 4;
                if (canMove(currentPiece, newRotation, currentX, currentY))
                    currentRotation = newRotation;
                rotateLeft = true;
            }
            else if (!bKey[3])
                rotateLeft = false;

            if (bKey[4] && !rotateRight)
            {
                int newRotation = (currentRotation + 1) % 4;
                if (canMove(currentPiece, newRotation, currentX, currentY))
                    currentRotation = newRotation;
                rotateRight = true;
            }
            else if (!bKey[4])
                rotateRight = false;

            if (bKey[5])
                break;

            if (gametick % ((10 - level) / 2) == 0 || bKey[2])
            {
                if (canMove(currentPiece, currentRotation, currentX, currentY + 1))
                    currentY++;
                else
                {
                    placePiece(currentPiece, currentRotation, currentX, currentY, false);
                    score += clearLines();
                    if (score >= (scoreThreshold[level]) && level < 9)
                    {
                        level++;
                        animationticks = 15;
                    }
                    displayBoard(currentPiece, currentRotation, currentX, currentY, ((animationticks-- > 0) ? "LEVEL UP" : ""));

                    break;
                }
            }

            placePiece(currentPiece, currentRotation, currentX, currentY, false);
            displayBoard(currentPiece, currentRotation, currentX, currentY, ((animationticks-- > 0) ? "LEVEL UP" : ""));

            auto frameEnd = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
            int sleepDuration = targetFrameTime - elapsed;
            if (sleepDuration > 0)
                Sleep(sleepDuration);
            gametick++;
        }

        currentPiece = nextPiece;
        nextPiece = rand() % 7;

        if (!canMove(currentPiece, 0, width / 2 - 2, 0))
        {
            std::cout << "\nGAME OVER!\nFINAL SCORE : " << score << "\n";
            break;
        }
    }
}

int main()
{
    srand(time(NULL));
    init();
    hideCursor();
    gameLoop();
    showCursor();
    return 0;
}