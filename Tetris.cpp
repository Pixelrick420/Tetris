#include <iostream>
#include <cstdlib>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <set>
#include <mmsystem.h>

int screenWidth = 60;
int screenHeight = 40;

int tetromino[7][16] = {
    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // line
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0}, // L
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0}, // J
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0}, // Square
    {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0}, // S
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, // T
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0}  // Z
};

const int width = 10;
const int height = 24;
unsigned int board[height][width]; // 0 empty, else block

int level = 0;
int score = 0;
int linesCleared = 0;
int lineThreshold = 12;
int pixelrick = 150640;
int player = 0;

std::set<int> lineNumbers;
int nextPiece;

const int COLORS[5][8] = {
    {0, 15, 2, 4, 15, 4, 15, 2},     // level 0
    {0, 11, 15, 12, 11, 12, 11, 15}, // level 1
    {0, 13, 11, 7, 13, 7, 13, 11},   // level 2
    {0, 2, 6, 15, 2, 15, 2, 6},      // level 3
    {0, 15, 4, 1, 15, 1, 15, 4}      // level 4
};

int rotatePiece(int px, int py, int r)
{
    switch (r % 4)
    {
    case 0:
        return py * 4 + px; // 0 deg
    case 1:
        return 12 + py - (px * 4); // 90 deg
    case 2:
        return 15 - (py * 4) - px; // 180 deg
    case 3:
        return 3 - py + (px * 4); // 270 degrees
    }
    return 0;
}

void moveCursor(short x, short y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(consoleHandle, position);
    SetConsoleTextAttribute(consoleHandle, 15);
}

inline void setCursorVisibility(bool visible)
{
    CONSOLE_CURSOR_INFO info = {100, visible};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void init()
{
    moveCursor(0, 0);
    score = linesCleared = 0;
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(board[0][0]), 0);

    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            std::cout << "  ";
        }
    }
    moveCursor(0, 0);
}

bool canMove(int piece, int rotation, int x, int y)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pieceIndex = rotatePiece(px, py, rotation);
            int boardX = x + px, boardY = y + py;

            if (tetromino[piece][pieceIndex] != 0)
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
            if (tetromino[piece][pieceIndex] != 0)
            {
                board[y + py][x + px] = remove ? 0 : piece + 1;
            }
        }
    }
}

void displayBoard(std::string message)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    moveCursor(0, 0);
    int rightMenu = 4;
    for (int i = 0; i < height; i++)
    {
        std::cout << "            ##";
        for (int j = 0; j < width; j++)
        {
            if (lineNumbers.count(i) > 0)
            {
                std::cout << "==";
            }
            else
            {
                SetConsoleTextAttribute(consoleHandle, COLORS[level % 5][board[i][j]]);
                std::cout << (board[i][j] == 0 ? " ." : "[]");
                SetConsoleTextAttribute(consoleHandle, 15);
            }
        }
        std::cout << "##";
        if ((i > rightMenu + 6) && (i < rightMenu + 10))
        {
            std::cout << ((nextPiece == 0 || nextPiece == 3) ? "  " : " ");
            for (int px = 0; px < 4; px++)
            {
                int pieceIndex = rotatePiece(px, i - (rightMenu + 6), 0);
                if (tetromino[nextPiece][pieceIndex] != 0)
                {
                    SetConsoleTextAttribute(consoleHandle, COLORS[level % 5][nextPiece + 1]);
                    std::cout << "[]";
                    SetConsoleTextAttribute(consoleHandle, 15);
                }
                else
                {
                    std::cout << "  ";
                }
            }
            std::cout << "      ";
        }

        if (i == rightMenu + 5)
        {
            std::cout << "  NEXT PIECE ";
        }
        if (i == rightMenu)
        {
            std::cout << "  LINES : " << linesCleared;
        }
        if (i == rightMenu + 1)
        {
            if (message.empty())
            {
                std::cout << "  LEVEL : " << level << "         ";
            }
            else
            {
                std::cout << message;
            }
        }
        if (i == rightMenu + 2)
        {
            std::cout << "  SCORE : " << score;
        }
        if (i == rightMenu + 3 && player > 0)
        {
            std::cout << "  HIGHSCORE : " << player << "              ";
        }
        std::cout << "\n";
    }
    std::cout << "            ########################";

    SetConsoleTextAttribute(consoleHandle, 15);
}

int clearLines()
{
    int clearLines = 0;
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

        if (isFull)
        {
            lineNumbers.insert(i);
            clearLines++;
        }
    }

    displayBoard("");
    if (clearLines > 0)
    {
        Sleep(((level < 10) ? 10 - level : 0));
    }
    int erased = 0;
    for (int i = height - 1, erased = 0; i >= 0; --i)
    {
        if (lineNumbers.count(i - erased))
        {
            ++erased;
            for (int j = i; j > 0; --j)
                std::copy(&board[j - 1][0], &board[j - 1][width], &board[j][0]);
        }
    }

    lineNumbers.clear();
    linesCleared += clearLines;
    return ((1 << clearLines) * 50 * (level + 1));
}

void displayMenu(bool start)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    moveCursor(0, 0);
    int dummy = 0;
    int levelColours[] = {4, 2, 3};
    std::string logo =
        "###########################################################             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "##                                                       ##             \n"
        "## TTTTTTTT EEEEEEEE TTTTTTTT RRRRRRRR IIIIIIII SSSSSSSS ##             \n"
        "##    TT    EE          TT    RR    RR    II    SS       ##             \n"
        "##    TT    EEEEEE      TT    RRRRRRRR    II    SSSSSSSS ##             \n"
        "##    TT    EE          TT    RR   RR     II          SS ##             \n"
        "##    TT    EEEEEEEE    TT    RR    RR IIIIIIII SSSSSSSS ##             \n"
        "##                                                       ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             \n"
        "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##             ";

    for (int i = 0; i < logo.length() - 1; i += 2)
    {
        char c = logo[i];
        if (c == 'T')
        {
            SetConsoleTextAttribute(consoleHandle, 4);
            std::cout << "[]";
            SetConsoleTextAttribute(consoleHandle, 15);
        }
        else if (c == 'E' || c == 'I')
        {
            SetConsoleTextAttribute(consoleHandle, 3);
            std::cout << "[]";
            SetConsoleTextAttribute(consoleHandle, 15);
        }
        else if (c == 'R' || c == 'S')
        {
            SetConsoleTextAttribute(consoleHandle, 2);
            std::cout << "[]";
            SetConsoleTextAttribute(consoleHandle, 15);
        }
        else
        {
            std::cout << c;
            i--;
        }
    }

    std::cout << "\n## . . . . [         ] . . . . . . . [         ] . . . . ##             " << "\n";
    std::cout << "## . . . . [";
    ((start) ? dummy = 1 : SetConsoleTextAttribute(consoleHandle, levelColours[(level % 3)]));
    std::cout << " LEVEL " << level << " ";
    SetConsoleTextAttribute(consoleHandle, 15);
    std::cout << "] . . . . . . . [";
    ((start) ? SetConsoleTextAttribute(consoleHandle, levelColours[(level % 3)]) : dummy = 0);
    std::cout << "  START  ";
    SetConsoleTextAttribute(consoleHandle, 15);
    std::cout << "] . . . . ##             " << "\n";
    std::cout << "## . . . . [         ] . . . . . . . [         ] . . . . ##           " << "\n";
    std::cout << "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##           " << "\n";
    std::cout << "## . . . . . .";
    if (player > pixelrick)
    {
        std::cout << "You beat pixelrick, what a nerd." << " . . . . . ##           " << "\n";
    }
    else
    {
        std::cout << " . . . . . . . . . . . . . . . . . . . . . ##           " << "\n";
    }
    std::cout << "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##           " << "\n";
    std::cout << "## . . . . . . . . . . . . . . . . . . . . . . . . . . . ##           " << "\n";
    std::cout << "###########################################################           " << "\n";
}

void gameLoop()
{
    bool rotateLeft = false;
    bool rotateRight = false;
    int movedLeft = -1;
    int movedRight = -1;
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

            bool bKey[7] = {false};
            for (int k = 0; k < 7; k++)
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28XZQP"[k]))) != 0;

            if (bKey[0])
                movedRight++;
            else
                movedRight = -1;

            if ((movedRight == 0 || movedRight > 2) && canMove(currentPiece, currentRotation, currentX + 1, currentY))
            {
                currentX++;
            }

            if (bKey[1])
                movedLeft++;
            else
                movedLeft = -1;

            if ((movedLeft == 0 || movedLeft > 2) && canMove(currentPiece, currentRotation, currentX - 1, currentY))
            {
                currentX--;
            }

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
            {
                exit(0);
            }

            if (bKey[6])
            {
                placePiece(currentPiece, currentRotation, currentX, currentY, false);
                displayBoard("  GAME PAUSED   ");
                do
                {
                    Sleep(50);
                    bKey[6] = (0x8000 & GetAsyncKeyState((unsigned char)('P'))) != 0;
                    Sleep(50);
                } while (!bKey[6]);
                placePiece(currentPiece, currentRotation, currentX, currentY, true);
            }

            if ((level < 8 && gametick % ((int)((32 - (level * 3)) / 8)) == 0) || level >= 8 || bKey[2])
            {
                if (canMove(currentPiece, currentRotation, currentX, currentY + 1))
                    currentY++;
                else
                {
                    placePiece(currentPiece, currentRotation, currentX, currentY, false);
                    score += clearLines();
                    if (linesCleared >= (lineThreshold * (level + 1)))
                    {
                        level++;
                        animationticks = 15;
                    }
                    displayBoard(((animationticks-- > 0) ? "  LEVEL UP   " : ""));

                    break;
                }
            }

            placePiece(currentPiece, currentRotation, currentX, currentY, false);
            displayBoard(((animationticks-- > 0) ? "  LEVEL UP    " : ""));

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
            displayBoard(((animationticks-- > 0) ? "LEVEL UP" : ""));
            player = ((score > player) ? score : player);
            level = 0;
            break;
        }
    }
}

void startMenu()
{
    bool startSelected = false;
    int frame = 0;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true)
    {
        displayMenu(startSelected);
        bool bKey[6] = {false};
        for (int i = 0; i < 6; i++)
            bKey[i] = ((0x8000 & GetAsyncKeyState((unsigned char)"\x26\x28\x25\x27\x0DQ"[i])) != 0);

        if ((frame % 2 == 0) && bKey[0] && !startSelected && level < 9)
            level++;

        else if ((frame % 2 == 0) && bKey[1] && !startSelected && level > 0)
            level--;

        else if (bKey[3] && !startSelected)
            startSelected = true;

        else if (bKey[2] && startSelected)
            startSelected = false;

        else if (bKey[4] && startSelected)
        {
            init();
            gameLoop();
        }

        else if (bKey[5])
            exit(0);

        Sleep(10);
        frame++;
    }
}

int main()
{
    srand(time(NULL));
    init();
    setCursorVisibility(false);
    startMenu();
    return 0;
}