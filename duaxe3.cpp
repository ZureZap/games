#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 15;
const int SCREEN_PADDING = 10;
const string SCORES_FILE = "scores.txt";

struct ScoreEntry {
    string name;
    int score;

    ScoreEntry(string n, int s) : name(n), score(s) {}
};

class Car {
private:
    int posX, posY;

public:
    Car(int x = WIDTH / 2, int y = HEIGHT - 4) : posX(x), posY(y) {}

    void moveLeft() { if (posX > 1) posX--; }
    void moveRight() { if (posX < WIDTH - 3) posX++; }
    void moveUp() { if (posY > 1) posY--; }
    void moveDown() { if (posY < HEIGHT - 3) posY++; }

    int getX() const { return posX; }
    int getY() const { return posY; }
};

class Obstacle {
private:
    int posX, posY;

public:
    Obstacle() { resetPosition(); }

    void moveDown() { posY++; }

    void resetPosition() {
        posX = rand() % (WIDTH - 4) + 1;
        posY = 1;
    }

    int getX() const { return posX; }
    int getY() const { return posY; }
};

class Game {
private:
    Car car;
    Obstacle obstacle;
    bool isGameOver;
    int score;
    string playerName;
    vector<ScoreEntry> scores;

    void loadScores() {
        ifstream inFile(SCORES_FILE);
        scores.clear();
        if (inFile.is_open()) {
            string name;
            int score;
            while (inFile >> name >> score) {
                scores.emplace_back(name, score);
            }
            inFile.close();
        }
    }

    void saveScores() {
        ofstream outFile(SCORES_FILE);
        if (outFile.is_open()) {
            for (const auto& entry : scores) {
                outFile << entry.name << " " << entry.score << endl;
            }
            outFile.close();
        }
    }

    void addScore(const string& name, int score) {
        scores.emplace_back(name, score);
        saveScores();
    }

public:
    Game(const string& name) : playerName(name) {
        resetGame();
        loadScores();
    }

    void resetGame() {
        car = Car();
        obstacle.resetPosition();
        isGameOver = false;
        score = 0;
    }

    void draw() {
        system("cls");
        for (int i = 0; i < SCREEN_PADDING; i++) cout << " ";
        for (int x = 0; x < WIDTH; x++) cout << "-";
        cout << endl;

        for (int y = 0; y < HEIGHT; y++) {
            for (int i = 0; i < SCREEN_PADDING; i++) cout << " ";
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1)
                    cout << "|";
                else if (y == car.getY() && x == car.getX())
                    cout << "A";
                else if ((y == car.getY() + 1 && (x == car.getX() - 1 || x == car.getX() + 1)) ||
                         (y == car.getY() + 2 && (x == car.getX() - 1 || x == car.getX() + 1)))
                    cout << "0";
                else if ((y == car.getY() + 1 && x == car.getX()) ||
                         (y == car.getY() + 2 && x == car.getX()))
                    cout << "#";
                else if ((y == obstacle.getY() && (x == obstacle.getX() || x == obstacle.getX() + 1 || x == obstacle.getX() + 2)) ||
                         (y == obstacle.getY() + 1 && (x == obstacle.getX() || x == obstacle.getX() + 1 || x == obstacle.getX() + 2)))
                    cout << "#";
                else
                    cout << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < SCREEN_PADDING; i++) cout << " ";
        for (int x = 0; x < WIDTH; x++) cout << "-";
        cout << endl;
        cout << "Score: " << score << endl;
        cout << "Su dung 'a', 'd', 'w', 's' de di chuyen.\n";
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a': car.moveLeft(); break;
                case 'd': car.moveRight(); break;
                case 'w': car.moveUp(); break;
                case 's': car.moveDown(); break;
            }
        }
    }

    void logic() {
        obstacle.moveDown();
        if (obstacle.getY() >= HEIGHT - 1) {
            obstacle.resetPosition();
            score++;
        }

        if ((obstacle.getY() == car.getY() || obstacle.getY() + 1 == car.getY() ||
             obstacle.getY() == car.getY() + 1 || obstacle.getY() + 1 == car.getY() + 2) &&
            (obstacle.getX() == car.getX() || obstacle.getX() + 1 == car.getX() || obstacle.getX() + 2 == car.getX() ||
             obstacle.getX() == car.getX() - 1 || obstacle.getX() + 1 == car.getX() - 1 || obstacle.getX() + 2 == car.getX() - 1 ||
             obstacle.getX() == car.getX() + 1 || obstacle.getX() + 1 == car.getX() + 1 || obstacle.getX() + 2 == car.getX() + 1)) {
            isGameOver = true;
            addScore(playerName, score);
        }
    }

    void run() {
        while (!isGameOver) {
            draw();
            input();
            logic();
            Sleep(100);
        }
        cout << "Game Over! Score: " << score << "\n";
    }

    bool playAgainPrompt() {
    char choice;
    cout << "Choi lai? (y/n): ";
    cin >> choice;
    while (choice != 'y' && choice != 'n') {
        cout << "Lua chon khong hop le. Chon 'y' de choi lai hoac 'n' de thoat: ";
        cin >> choice;
    }
    if (choice == 'y' || choice == 'Y') {
        resetGame();
        return true;
    }
    return false;
    }

    void showScores() {
        system("cls");
        cout << left << setw(15) << "Ten" << "Diem\n";
        cout << "=======================\n";
        for (const auto& entry : scores) {
            cout << left << setw(15) << entry.name << entry.score << endl;
        }
        system("pause");
    }
};

void showInstructions() {
    system("cls");
    cout << "Instructions:\n";
    cout << "1. Su dung cac phim:\n";
    cout << "   - 'a': Di chuyen trai\n";
    cout << "   - 'd': Di chuyen phai\n";
    cout << "   - 'w': Di chuyen len\n";
    cout << "   - 's': Di chuyen xuong\n";
    cout << "2. Trach cac chuong ngai vat (#).\n";
    cout << "3. Dat diem cao nhat co the.\n";
    system("pause");
}

int main() {
    srand(time(0));
    int choice;

    do {
        system("cls");
        cout << "Racing Game\n";
        cout << "1. Choi\n";
        cout << "2. Huong dan\n";
        cout << "3. Xem diem\n";
        cout << "4. Thoat\n";
        cout << "Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Nhap ten cua ban: ";
                string name;
                cin >> name;
                Game game(name);
                do {
                    game.run();
                } while (game.playAgainPrompt());
                break;
            }
            case 2:
                showInstructions();
                break;
            case 3: {
                Game dummy("");
                dummy.showScores();
                break;
            }
            case 4:
                cout << "Thoat. Tam biet!\n";
                break;
            default:
                cout << "Lua chon khong hop le. Thu lai.\n";
                system("pause");
                break;
        }
    } while (choice != 4);

    return 0;
}
