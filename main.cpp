// snek game
#include <iostream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <fstream>
#include <string>

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
    int x, y, appleX, appleY, highscore;
    int runEveryMillis = 250;
    int score = -1;  // start on -1 because it is incremented immediately when the first apple is spawned
    int length = -1; // tail length, ^
    int bodyX[500];
    int bodyY[500];
    string deathMessage;
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
        length++;
        int tempAppleX;
        int tempAppleY;
        bool suitable = false;
        while (!suitable) // make sure the apple doesn't spawn on the snakes body
        {
            srand((int)time(0)); // rand was not actually random
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

    void end()
    {
        if (score > highscore)
        {
            ofstream highscore_file("highscore.txt");

            highscore_file << to_string(score);
            highscore_file.close();
        }

        system("cls");
        if (score > highscore)
            cout << "NEW HIGHSCORE!" << endl;
        cout << deathMessage << endl;
        cout << "Final Score: " << score << endl;
        if (score > highscore)
            cout << "Highscore: " << score << endl;
        else
            cout << "Highscore: " << highscore << endl;
    }

    void input()
    {
        // handle inputs
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'a':
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
                if (dir != UP)
                    dir = DOWN;
                break;
            case 'x':
                run = false;
                deathMessage = "You exited the game";
                break;
            }
        }
    }

    void logic()
    {
        // check for apple intersect
        if (x == appleX && y == appleY)
        {
            handleApple();
        }
        // handle the tail

        int headPrevX = bodyX[0];
        int headPrevY = bodyY[0];

        int headPrevX2, headPrevY2;

        bodyX[0] = x;
        bodyY[0] = y;
        for (int i = 0; i < length - 1; i++) // move each value down the vector and set the first value to the coords of the head
        {

            headPrevX2 = bodyX[i + 1];
            bodyX[i + 1] = headPrevX;
            headPrevX = headPrevX2;

            headPrevY2 = bodyY[i + 1];
            bodyY[i + 1] = headPrevY;
            headPrevY = headPrevY2;
        }
        // change head coords
        switch (dir)
        {
        case 1: // LEFT
            x--;
            break;
        case 2: // RIGHT
            x++;
            break;
        case 3: //  UP
            y--;
            break;
        case 4: // DOWN
            y++;
            break;
        }

        // check if it has hit a wall
        if (x < 0 || x > 25 || y < 0 || y > 20)
        {
            run = false;
            deathMessage = "Boinked your head to hard. Now you Dead.";
            end();
        }

        // check if it hits itself
        for (int i = 0; i < length; i++)
        {
            if (x == bodyX[i] && y == bodyY[i])
            {
                run = false;
                deathMessage = "Got a bit tangled. Now you dead.";
                end();
            }
        }
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
        for (int i = 0; i < height; i++) // draw middle, i is y and j is x
        {
            cout << "#";
            bool open = true;
            for (int j = 0; j < width; j++)
            {
                if (x == j && y == i)
                    cout << "O";
                else if (j == appleX && i == appleY)
                    cout << "A";
                else
                {
                    open = true;
                    for (int k = 0; k < length; k++)
                    {
                        if (bodyX[k] == j && bodyY[k] == i)
                        {
                            cout << "o";
                            open = false;
                        }
                    }
                    if (open)
                        cout << " ";
                }
            }

            cout << "#" << endl;
        }
        for (int i = 0; i < width + 2; i++) // draw bottom border
        {
            cout << "#";
        }
        cout << endl;
        cout << "Score: " << score << endl;
        if (score > highscore)
            cout << "Highscore: " << score << endl;
        else
            cout << "Highscore: " << highscore << endl;
    }

    void setup()
    {
        x = 10;
        y = 10;
        handleApple();

        ifstream highscore_file("highscore.txt");
        string str_highscore;
        while (getline(highscore_file, str_highscore))
        {
            highscore = stoi(str_highscore);
        }
    }

    void step()
    {
        input();
        logic();
        if (run)
            draw();
        else
            return;
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