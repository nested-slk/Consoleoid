#include <iostream>
#include <Windows.h>
#include <String.h>
#include <cmath>

using namespace std;

#define mWidth 40
#define mHeight 20
#define BRICKS_WIDTH 20//mWidth - 5
#define BRICKS_HEIGHT 1//mHeight / 5
#define PLAYER_SIZE 7
#define PLAYER_LIVES 3

void MoveXY(int x, int y) // set cursor in console
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct GameMap
{
    // TODO: Add design map
    // TODO: create map with vector

    char map[mHeight][mWidth + 1];

    void addBorder(char leftWallSymbol, char rightWallSymbol, char bottomtWallSymbol, char topWallSymbol) // add specific borders
    {
        for (int j = 0; j < mHeight; j++)
        {
            for (int i = 0; i < mWidth; i++)
            {
                if (j == 0 && i == 0) // find corner positions
                {
                    this->map[j][i] = 'T';
                }
                else if (j == 0 && i == mWidth - 1)
                {
                    this->map[j][i] = 'T';
                }
                else if (j == mHeight - 1 && i == 0)
                {
                    this->map[j][i] = 'A';
                }
                else if (j == mHeight - 1 && i == mWidth - 1)
                {
                    this->map[j][i] = 'A';
                }
                else if (j == 0) // find borders positions
                {
                    this->map[j][i] = topWallSymbol;
                }
                else if (j == mHeight - 1)
                {
                    this->map[j][i] = bottomtWallSymbol;
                }
                else if (i == 0)
                {
                    this->map[j][i] = leftWallSymbol;
                }
                else if (i == mWidth - 1)
                {
                    this->map[j][i] = rightWallSymbol;
                }
                else
                {
                    this->map[j][i] = ' ';
                }
            }
            this->map[j][mWidth] = '\n'; // add new line symbol in end of line
        }
    }

    void clearInsideBorder()
    {
        for (int j = 1; j < mHeight - 1; j++)
        {
            for (int i = 1; i < mWidth - 1; i++)
            {
                this->map[j][i] = ' ';
            }
        }
    }
    void show() // sending characters to the console
    {
        COORD pos;
        pos.X = 0;
        pos.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        cout << this->map[0];
    }
};

class Player
{
public:
    Player(int start_pos_x, int start_pos_y, short int size, char design, short int lives, GameMap *_map)
    {
        map_ = _map;
        pos_x_ = start_pos_x;
        pos_y_ = start_pos_y;
        lives_ = lives;
        coins_ = 0;
        draw(size, design);
    }

    void draw(short int size = 4, char design = '=') // add player to the global map
    {
        design_ = design;
        size_ = size;
        for (int j = 0; j < size; j++)
        {
            map_->map[pos_y_][pos_x_ + j] = design_;
        }
    }
    // TODO: merge changePos and move
    void changePos(void) // update player position on global map
    {
        for (int j = 0; j < size_; j++)
        {
            map_->map[pos_y_][pos_x_ + j] = design_;
        }
    }
    void move(const char up = 'w', char left = 'A', char down = 's', char right = 'D') // player control from keyboard
    {
        if ((GetKeyState(left) & 0x8000) && pos_x_ > 1) // check pressed key and player position to stay between borders
        {
            pos_x_ -= 1;
        }
        else if ((GetKeyState(right) & 0x8000) && pos_x_ < mWidth - 1 - size_) // check pressed key and player position to stay between borders
        {
            pos_x_ += 1;
        }
        changePos();
    }
    int getPosX()
    {
        return pos_x_;
    }
    int getPosY()
    {
        return pos_y_;
    }
    int getLives()
    {
        return lives_;
    }
    int getCoins()
    {
        return coins_;
    }
    int getSize()
    {
        return size_;
    }
    bool setCoins(int coins)
    {
        coins_ = coins;
    }
    bool isLastLive() // reduse lives and check that they are out
    {
        if (lives_ <= 1)
        {
            return true;
        }
        else
        {
            lives_--;
            return false;
        }
    }
    // ~Player();
private:
    GameMap *map_;
    int pos_x_;
    int pos_y_;
    int coins_;
    short int size_, lives_;
    char design_;
};

class Block
{
private:
    int armor, points;

public:
    Block(unsigned short int armor, unsigned short int points)
    {
        this->armor = armor;
        this->points = points;
    }
    Block(unsigned short int level)
    {
        if (level == 1)
        {
            this->armor = 1;
            this->points = 1;
        }
        else if (level == 2)
        {
            this->armor = 2;
            this->points = 3;
        }
        else if (level == 3)
        {
            this->armor = 3;
            this->points = 5;
        }
    }
};

class BlockPlaser
{
public:
    BlockPlaser(GameMap *map, int offset_x, int offset_y) // set bricks on brick map and set the offest from the global map
    {
        bricks_map_ = map;
        offset_x_ = offset_x;
        offset_y_ = offset_y;
        short int rand_int;
        max_coins_ = 0;
        // TODO: add clear method in GameMap
        for (int j = 0; j < BRICKS_HEIGHT; j += 1) // clear brick map
        {
            for (int i = 0; i < BRICKS_WIDTH; i += 1)
            {

                bricks_map_->map[j][i] = ' ';
            }
        }
        for (int j = 0; j < BRICKS_HEIGHT; j += 2) // add on map various blocks
        {
            for (int i = 0; i < BRICKS_WIDTH; i += 2)
            {
                this->max_coins_++;
                rand_int = (rand() % 3 + 1);
                if (rand_int == 1)
                    bricks_map_->map[j][i] = 'V';
                else if (rand_int == 2)
                    bricks_map_->map[j][i] = 'U';
                else
                    map->map[j][i] = 'v';
            }
        }
    }
    void update(GameMap *map) // add bricks on global map
    {
        for (int j = 0; j < BRICKS_HEIGHT; j++)
        {
            for (int i = 0; i < BRICKS_WIDTH; i++)
            {
                map->map[j + offset_y_][i + offset_x_] = bricks_map_->map[j][i];
            }
        }
    }
    int getMaxCoins()
    {
        return max_coins_;
    }
    // ~BlockPlaser();
private:
    GameMap *bricks_map_;
    int offset_x_, offset_y_, max_coins_;
};

class Ball
{
public:
    Ball(char design = 'O', int start_pos_x = 10, int start_pos_y = 15, int speed = 1)
    {
        x_ = start_pos_x;
        y_ = start_pos_y;
        speed_ = speed;
        dir_ = 80.0;
        design_ = design;
    }
    void update(GameMap *map) // update ball position on map and check collision
    {
        map_ = map;
        x_end_ = x_ + speed_ * cos((dir_ * M_PI) / 180.0); // calculate new ball coordinates
        y_end_ = y_ + speed_ * sin((dir_ * M_PI) / 180.0);
        if (x_end_ < 1 || y_end_ < 1 || x_end_ > mWidth - 1 || y_end_ > mHeight - 1) // check out of border
        {
            x_end_ = mWidth / 2;
            y_end_ = mHeight / 2;
        }
        if (x_end_ < 2 || x_end_ > mWidth - 3) // check collision with border and bounce from border
        {
            if (dir_ >= 0 && dir_ < 90)
            {
                dir_ = 180 - dir_;
            }
            else if (dir_ >= 90 && dir_ < 180)
            {
                dir_ =  (180 - dir_);
            }
            else if (dir_ >= 180 && dir_ < 270)
            {
                dir_ = 360 - (dir_ - 180);
            }
            else if (dir_ >= 270 && dir_ < 360)
            {
                dir_ = 270 - (dir_ - 270);
            }
        }
        else if (y_end_ < 2)
        {
            if (dir_ >= 0 && dir_ < 90)
            {
                dir_ = 360 - dir_;
            }
            else if (dir_ >= 90 && dir_ < 180)
            {
                dir_ = 180 + (180 - dir_);
            }
            else if (dir_ >= 180 && dir_ < 270)
            {
                dir_ = 90 + (270 - dir_);
            }
            else if (dir_ >= 270 && dir_ < 360)
            {
                dir_ = (360 - dir_);
            }
        }
        map_->map[int(y_end_)][int(x_end_)] = design_; // update ball position
        x_ = x_end_;
        y_ = y_end_;
    }
    bool CheckPlayerCollision(GameMap *map, int player_pos_x, int player_pos_y, int player_size) // check collision with player
    {
        short int rand_int = 1;
        if (player_pos_y == int(y_)) // check player and ball line
        {
            for (int i = player_pos_x; i < player_pos_x + player_size; i++) // check side of player collision with ball
            {
                rand_int = (rand() % 45 + 1);
                if (i == int(x_) && (i < int(player_pos_x + (player_size / 2)))) // bounce from left side of the player
                {
                    if (dir_ >= 0 && dir_ < 90)
                    {
                        dir_ = 270 - (90 - dir_);
                    }
                    else if (dir_ >= 90 && dir_ < 180)
                    {
                        if (dir_ == 90)
                        {
                            dir_ = rand_int + 202;
                        }
                        else
                        {
                            dir_ = 270 - (dir_ - 90);
                        }
                    }
                }
                else if (i == int(x_) && (i == int(player_pos_x + (player_size / 2)))) // bounce from center of the player
                {
                    if (dir_ >= 0 && dir_ < 90)
                    {
                        dir_ = 270;
                    }
                    else if (dir_ >= 90 && dir_ < 180)
                    {
                        dir_ = 270;
                    }
                }
                else if (i == int(x_) && (i > int(player_pos_x + (player_size / 2)))) // bounce from right side of the player
                {
                    if (dir_ >= 0 && dir_ < 90)
                    {
                        dir_ = 270 + (90 - dir_);
                    }
                    else if (dir_ >= 90 && dir_ < 180)
                    {
                        if (dir_ == 90)
                        {
                            dir_ = rand_int + 292;
                        }
                        else
                        {
                            dir_ = 270 + (dir_ - 90);
                        }
                    }
                }
            }
        }
        else if (y_ > mHeight - 2) // if ball under player
        {
            // TODO: do lost ball event better
            y_ = 5;
            return false;
        }
        return true;
    }

    bool CheckBrickCollision(GameMap *map, GameMap *bricks_map, int offset_y, int offset_x) // check ball and brick collision
    {
        brick_map_ = bricks_map;
        brick_on_map_y_ = int(y_) - offset_y;
        brick_on_map_x_ = int(x_) - offset_x;
        if (brick_on_map_y_ < 0 || brick_on_map_x_ < 0 || brick_on_map_y_ > BRICKS_HEIGHT - 1 || brick_on_map_x_ > BRICKS_WIDTH - 1)
            return false;
        if (brick_map_->map[brick_on_map_y_][brick_on_map_x_] != ' ')
        {
            // TODO: add bounce brom brick
            brick_map_->map[brick_on_map_y_][brick_on_map_x_] = ' '; // clear brick position after hit them
            return true;
        }
        return false;
    }
    // ~Ball();

private:
    float x_, y_, x_end_, y_end_, dir_, speed_;
    int brick_on_map_y_, brick_on_map_x_;
    GameMap *map_;
    GameMap *brick_map_;
    char design_ = 'O';
};
class Game
{

};   

int main()
{
    GameMap global_map; // create map
    GameMap brick_map;
    global_map.addBorder('|', '|', '=', '='); // add border on map
    global_map.show();                        // show map on screen

    Player Pl((mWidth / 2), (mHeight - 3), PLAYER_SIZE, 'T', PLAYER_LIVES, &global_map); // create player
    Pl.setCoins(0);

    BlockPlaser Blocks(&brick_map, 4, 2);   // create bricks
    Ball Orb('O', mHeight / 2, mWidth / 2); // create orb

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // console object

    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false; // invisible cursor
    cursor.dwSize = 1;
    SetConsoleCursorInfo(h, &cursor); // update cursor in console

    MoveXY((mWidth / 2) - 10, mHeight / 2); // print info
    cout << "Game start in: ";
    SetConsoleTextAttribute(h, 4); // change text color
    for (int i = 0; i < 3; i++)
    {
        cout << 3 - i;
        Sleep(1000);
    }
    cout << " --> GO!";
    Sleep(1000);
    SetConsoleTextAttribute(h, 7); // change text color

    while (GetKeyState(VK_ESCAPE) >= 0) // ESC key to close console
    {
        global_map.clearInsideBorder();
        Blocks.update(&global_map); // update positions
        Orb.update(&global_map);
        if (!Orb.CheckPlayerCollision(&global_map, Pl.getPosX(), Pl.getPosY(), Pl.getSize())) // check that the player hit the ball
        {
            if (Pl.isLastLive()) // if its last life of the player then exit from game
            {
                return 0;
            }
        }
        if (Orb.CheckBrickCollision(&global_map, &brick_map, 2, 4)) // add a coin for hiting the brick
            Pl.setCoins(Pl.getCoins() + 1);
        if (Pl.getCoins() == Blocks.getMaxCoins()) // when the blocks are over
        {
            MoveXY((mWidth / 2) - 5, mHeight / 2);
            cout << "You win!";
            Sleep(5000);

            return 0;
        }
        Pl.move(); // check palyer moving

        SetConsoleTextAttribute(h, 7); // set color and show changes in console
        global_map.show();
        if (Pl.getCoins() == Blocks.getMaxCoins()) // when the blocks are over
        {
            MoveXY((mWidth / 2) - 5, mHeight / 2);
            cout << "You win!";
            Sleep(5000);

            return 0;
        }
        SetConsoleTextAttribute(h, 14); // show additional information in console
        MoveXY(mWidth + 1, 1);
        cout << "Lives: " << Pl.getLives();
        MoveXY(mWidth + 1, 2);
        cout << "Coins: " << Pl.getCoins();
        MoveXY(mWidth + 1, 4);
        cout << "Max coins: " << Blocks.getMaxCoins();
        MoveXY(mWidth + 1, mHeight - 2);
        cout << "ESC: exit";

        // TODO: add a speed control
        Sleep(100); // set game speed
    }

    return 0;
}
