#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <map>

using namespace std;

// Global Declarations
enum Direction { Top = 1, Down, Right, Left };
map<int, int> level_up_count;

// Map and Snake Structures
struct Map {
    int Width = 50;
    int Height = 25;
    int FruitX;
    int FruitY;
};

struct Snake {
    int HeadX;
    int HeadY;
    int TailLength = 0;
    int TailX[50];
    int TailY[50];
    Direction Dir;
};

// Player Structure
struct Player {
    int Score = 0;
    int Speed = 100;
    bool Lose = false;
};

Map gameMap;
Snake snake;
Player player;

// Function Prototypes
void MoveTail(int x[], int size);
void DropFruit();
void InitializeGame();
void Draw();
void HandleInput();
void UpdateLevel();
void MoveSnake();
void StartScreen();
void GameOverScreen();

// Function Definitions
void MoveTail(int x[], int size) {
    for (int i = size - 2; i >= 0; i--) {
        x[i + 1] = x[i];
    }
}

void DropFruit() {
    srand(static_cast<unsigned int>(time(NULL)));
    gameMap.FruitX = rand() % (gameMap.Width - 2) + 1;
    gameMap.FruitY = rand() % (gameMap.Height - 2) + 1;
    snake.TailLength++;
}

void InitializeGame() {
    snake.HeadX = gameMap.Width / 2;
    snake.HeadY = gameMap.Height / 2;
    player.Score = 0;
    player.Lose = false;
    DropFruit();
}

void Draw() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    SetConsoleTextAttribute(consoleHandle, 12);
    cout << endl << "     Score: ";
    SetConsoleTextAttribute(consoleHandle, 15);
    cout << player.Score;

    SetConsoleTextAttribute(consoleHandle, 12);
    cout << "                   Level: ";
    SetConsoleTextAttribute(consoleHandle, 15);
    cout << player.Score / 2 << endl << endl;

    for (size_t i = 0; i < gameMap.Height; i++) {
        for (size_t j = 0; j < gameMap.Width; j++) {
            if (player.Score % 2 == 0 && player.Score > 1 && i == 1 && j == 18 && level_up_count[player.Score] < 10) {
                cout << "Level up";
                level_up_count[player.Score]++;
                j += 8;
            }
            if (i == 0 || i == gameMap.Height - 1) {
                SetConsoleTextAttribute(consoleHandle, 9);
                cout << "*";
            } else if (j == 0 || j == gameMap.Width - 1) {
                SetConsoleTextAttribute(consoleHandle, 11);
                cout << "#";
            } else if (j == snake.HeadX && i == snake.HeadY) {
                SetConsoleTextAttribute(consoleHandle, 15);
                cout << "O";
            } else if (j == gameMap.FruitX && i == gameMap.FruitY) {
                SetConsoleTextAttribute(consoleHandle, 4);
                cout << "&";
            } else {
                bool tail = false;
                for (int k = 0; k < snake.TailLength; k++) {
                    if (snake.TailX[k] == j && snake.TailY[k] == i) {
                        SetConsoleTextAttribute(consoleHandle, 10);
                        cout << "o";
                        tail = true;
                        break;
                    }
                }
                if (!tail) cout << " ";
            }
        }
        cout << endl;
    }
}

void HandleInput() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': snake.Dir = Top; break;
            case 's': snake.Dir = Down; break;
            case 'd': snake.Dir = Right; break;
            case 'a': snake.Dir = Left; break;
            case 'x': exit(0);
        }
    }
}

void UpdateLevel() {
    if (player.Score % 2 == 0) {
        player.Speed -= 5;
        gameMap.Height--;
        gameMap.Width--;
    }
}

void MoveSnake() {
    MoveTail(snake.TailX, 50);
    MoveTail(snake.TailY, 50);
    snake.TailX[0] = snake.HeadX;
    snake.TailY[0] = snake.HeadY;

    switch (snake.Dir) {
        case Top: snake.HeadY--; break;
        case Down: snake.HeadY++; break;
        case Right: snake.HeadX++; break;
        case Left: snake.HeadX--; break;
    }

    if (snake.HeadY >= gameMap.Height - 1 || snake.HeadY <= 0 || snake.HeadX >= gameMap.Width - 1 || snake.HeadX <= 0) {
        player.Lose = true;
    }

    for (size_t i = 0; i < snake.TailLength; i++) {
        if (snake.TailX[i] == snake.HeadX && snake.TailY[i] == snake.HeadY) {
            player.Lose = true;
        }
    }

    if (snake.HeadY == gameMap.FruitY && snake.HeadX == gameMap.FruitX) {
        player.Score++;
        UpdateLevel();
        DropFruit();
    }
}

void StartScreen() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, 15);

    cout << endl;
    cout << "    SSSSSS  NN   NN    AAA    KK   KK  EEEEEEE" << endl;
    cout << "   SS       NN   NN   AA AA   KK  KK   EE" << endl;
    cout << "   SS       NNN  NN  AA   AA  KK KK    EE" << endl;
    cout << "    SSSSS   NN N NN  AAAAAAA  KKK      EEEEEE" << endl;
    cout << "        SS  NN  NNN  AA   AA  KK KK    EE" << endl;
    cout << "        SS  NN   NN  AA   AA  KK  KK   EE" << endl;
    cout << "   SSSSSS   NN   NN  AA   AA  KK   KK  EEEEEEE" << endl;

    cout << endl << "          **** Snake Game **** " << endl;
    SetConsoleTextAttribute(consoleHandle, 12);
    cout << endl << "  * Game Controls" << endl << endl;

    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     1 - W to move up" << endl;
    SetConsoleTextAttribute(consoleHandle, 9);
    cout << "     2 - S to move down" << endl;
    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     3 - D to move right" << endl;
    SetConsoleTextAttribute(consoleHandle, 9);
    cout << "     4 - A to move left" << endl;
    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     5 - X to exit" << endl;

    SetConsoleTextAttribute(consoleHandle, 12);
    cout << endl << "  * Game Instructions" << endl << endl;

    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     1 - Eat the food to grow longer." << endl;
    SetConsoleTextAttribute(consoleHandle, 9);
    cout << "     2 - Avoid the walls to stay alive." << endl;
    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     3 - Avoid biting yourself." << endl;
    SetConsoleTextAttribute(consoleHandle, 9);
    cout << "     4 - The play area decreases each level." << endl;
    SetConsoleTextAttribute(consoleHandle, 11);
    cout << "     5 - Snake speed increases each level." << endl;
    SetConsoleTextAttribute(consoleHandle, 9);
    cout << "     6 - Level up after eating two apples." << endl;

    SetConsoleTextAttribute(consoleHandle, 14);
    cout << endl << "            To start the game, press X " << endl << endl;
}

void GameOverScreen() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, 13);
    cout << endl << "        **** Game Over ****" << endl;
    SetConsoleTextAttribute(consoleHandle, 11);
    cout << endl << "       Do you want to play again? (y/n) ";
}

// Main Function
int main() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    StartScreen();

    char startKey;
    cin >> startKey;

    if (startKey == 'x' || startKey == 'X') {
        do {
            InitializeGame();

            while (!player.Lose) {
                Draw();
                HandleInput();
                MoveSnake();
                Sleep(player.Speed);
            }

            GameOverScreen();
            char replayKey;
            cin >> replayKey;

            if (replayKey == 'y' || replayKey == 'Y') {
                player.Speed = 100;
            } else {
                break;
            }
        } while (true);
    }

    SetConsoleTextAttribute(consoleHandle, 7);
    return 0;
}
