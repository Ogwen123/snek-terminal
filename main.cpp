// snek game
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

using namespace std;
class Game
{
public:
    bool run = true;
    const int height = 20; // inside of the grid will be 20 by 20, not the outside
    const int width = 25;
    int x, y, appleX, appleY;
    int runEveryMillis = 250;
    int score = -1; // start on -1 because it is incremented immediately when the first apple is spawned
    int length = 0; // tail length
    vector<int> bodyX;
    vector<int> bodyY;
    enum Direction
    {
        STOP = 0,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    Direction dir;

    void handleApple()
    {
        score++;
        int tempAppleX;
        int tempAppleY;
        bool suitable = false;
        while (!suitable) // make sure the apple doesn't spawn on the snakes body
        {
            tempAppleX = rand() % width;
            tempAppleY = rand() % height;
            bool check1 = true;
            bool check2 = true;
            if (tempAppleX == x && tempAppleY == y) // check for head intersect
                check1 = false;
            for (int i = 0; i < length; i++) // check for body intersect
            {
                if (bodyX[i] == tempAppleX && bodyY[i] == tempAppleY)
                    check2 = false;
            }
            if (check1 && check2)
                suitable = true;
        }
        appleX = tempAppleX;
        appleY = tempAppleY;
    }

    void input()
    {
        // TODO 1 handle inputs
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                run = false;
                break;
            }
        }
    }

    void logic()
    {
        // TODO 3 check if snake dies - hits its self, hits wall
        if (x == appleX && y == appleY)
        {
            handleApple();
        }
        // TODO 2 move snake
    }

    void draw()
    {
        system("cls"); // clear the terminal
        // draw walls

        for (int i = 0; i < width + 2; i++) // draw top border
        {
            cout << "#";
        }
        cout << endl;
        for (int i = 0; i < height; i++) // draw middle
        {
            cout << "#";

            for (int j = 0; j < width; j++)
            {
                if (x == j && y == i)
                    cout << "O";
                else if (count(bodyX.begin(), bodyX.end(), j) && count(bodyY.begin(), bodyY.end(), i))
                    cout << "o";
                else if (j == appleX && i == appleY)
                    cout << "F";
                else
                    cout << " ";
            }

            cout << "#" << endl;
        }
        for (int i = 0; i < width + 2; i++) // draw bottom border
        {
            cout << "#";
        }
        cout << endl;
        printf("Score: %d\n", score);
    }

    void setup()
    {
        x = 10;
        y = 10;
        handleApple();
    }

    void step()
    {
        input();
        logic();
        draw();
    }
};

void sleep(int millis)
{
#ifdef _WIN32
    Sleep(millis);
#else
    usleep(millis * 1000);
#endif
}

int main()
{
    Game game = Game();
    game.setup();
    while (game.run)
    {
        // do stuff
        sleep(game.runEveryMillis);
        game.step();
    }
}