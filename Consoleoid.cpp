#include <iostream>
#include <Windows.h>
#include <String.h>
#include <vector>
#include <cmath>

using namespace std;

#define mWidth 40
#define mHeight 20
#define bWidth mWidth - 5
#define bHeight mHeight / 3
#define PlayerSize 7
#define PlayerLives 3

void MoveXY(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
struct TMap
{
    char map1[mHeight][mWidth + 1];
    void Clear();
    void Show();
    void Border();
};

void TMap::Border()
{
    char wall = '|';
    for (int j = 0; j < mHeight; j++)
    {
        for (int i = 0; i < mWidth; i++)
        {
            if (j == 0 && i == 0)
            {
                map1[j][i] = 'T';
            }
            else if (j == 0 && i == mWidth - 1)
            {
                map1[j][i] = 'T';
            }
            else if (j == mHeight - 1 && i == 0)
            {
                map1[j][i] = 'A';
            }
            else if (j == mHeight - 1 && i == mWidth - 1)
            {
                map1[j][i] = 'A';
            }
            else if (j == 0 || j == mHeight - 1)
            {
                map1[j][i] = '=';
            }
            else if (i == 0 || i == mWidth - 1)
            {
                map1[j][i] = wall;
            }
            else
            {
                map1[j][i] = ' ';
            }
        }
        map1[j][mWidth] = '\n';
    }
}

void TMap::Clear()
{
    // COORD w_size;
    // w_size.X = mWidth;
    // w_size.Y = mHeight;
    // SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), w_size);
    for (int j = 1; j < mHeight - 1; j++)
    {
        for (int i = 1; i < mWidth - 1; i++)
        {
            map1[j][i] = ' ';
        }
    }
}
void TMap::Show()
{
    MoveXY(0, 0);
    cout << map1[0];
}

class Player
{
public:
    Player(int start_pos_x, int start_pos_y, short int size, char design, short int lives, TMap *_map)
    {
        map = _map;
        pos_x = start_pos_x;
        pos_y = start_pos_y;
        this->lives = lives;
        this->coins = 0;
        Draw(size, design);
    }

    void Draw(short int size = 4, char design = '=')
    {
        this->design = design;
        this->size = size;
        for (int j = 0; j < size; j++)
        {
            map->map1[this->pos_y][this->pos_x + j] = this->design;
        }
    }
    void ChangePos(void)
    {
        for (int j = 0; j < this->size; j++)
        {
            map->map1[this->pos_y][this->pos_x + j] = this->design;
        }
    }
    void Move(const char up = 'w', char left = 'A', char down = 's', char right = 'D')
    {
        if ((GetKeyState(left) & 0x8000) && pos_x > 1)
        {
            this->pos_x -= 1;
        }
        else if ((GetKeyState(right) & 0x8000) && pos_x < mWidth - 1 - this->size)
        {
            this->pos_x += 1;
        }
        ChangePos();
    }
    int GetPosX()
    {
        return this->pos_x;
    }
    int GetPosY()
    {
        return this->pos_y;
    }
    int GetLives()
    {
        return this->lives;
    }
    int GetCoins()
    {
        return this->coins;
    }
    int GetSize()
    {
        return this->size;
    }

    bool SetCoins(int coins)
    {
        this->coins = coins;
    }
    bool IsLastLive()
    {
        if (this->lives <= 0)
        {
            return true;
        }
        else
        {
            this->lives--;
            return false;
        }
    }

private:
    TMap *map;
    int pos_x;
    int pos_y;
    int coins;
    short int size, lives;
    char design;
    vector<char> player_design;
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
    BlockPlaser(TMap *map, int offset_x, int offset_y)
    {
        this->bricks_map = map;
        this->offset_x = offset_x;
        this->offset_y = offset_y;
        short int rand_int;
        for (int j = 0; j < bHeight; j += 1)
        {
            for (int i = 0; i < bWidth; i += 1)
            {

                bricks_map->map1[j][i] = ' ';
            }
        }
        for (int j = 0; j < bHeight; j += 2)
        {
            for (int i = 0; i < bWidth; i += 2)
            {
                rand_int = (rand() % 3 + 1);
                if (rand_int == 1)
                    bricks_map->map1[j][i] = 'V';
                else if (rand_int == 2)
                    bricks_map->map1[j][i] = 'U';
                else 
                    map->map1[j][i] = 'v';
            }
        }
    }
    void Update(TMap *_map)
    {
        for (int j = 0; j < bHeight; j++)
        {
            for (int i = 0; i < bWidth; i++)
            {
                _map->map1[j + offset_y][i + offset_x] = bricks_map->map1[j][i];
            }
        }
    }

private:
    TMap *bricks_map;
    int offset_x, offset_y;
};

class Ball
{
public:
    Ball(char design = 'O', int start_pos_x = 10, int start_pos_y = 15, int speed = 1)
    {
        this->x = start_pos_x;
        this->y = start_pos_y;
        this->speed = speed;
        this->dir = 80.0;
        this->design = design;
    }
    void Update(TMap *_map)
    {
        map = _map;
        x_end = x + speed * cos((dir * M_PI) / 180.0);
        float t = cos((dir * M_PI) / 180.0);
        y_end = y + speed * sin((dir * M_PI) / 180.0);
        if (x_end < 1 || y_end < 1 || x_end > mWidth - 1 || y_end > mHeight - 1)
        {
            x_end = mWidth / 2;
            y_end = mHeight / 2;
        }
        if (x_end < 2 || x_end > mWidth - 3)
        {
            if (dir >= 0 && dir < 90)
            {
                dir = 180 - dir;
            }
            else if (dir >= 90 && dir < 180)
            {
                dir = 90 - (180 - dir);
            }
            else if (dir >= 180 && dir < 270)
            {
                dir = 360 - (dir - 180);
            }
            else if (dir >= 270 && dir < 360)
            {
                dir = 270 - (dir - 270);
            }
        }
        else if (y_end < 2)
        {
            if (dir >= 0 && dir < 90)
            {
                dir = 360 - dir;
            }
            else if (dir >= 90 && dir < 180)
            {
                dir = 180 + (180 - dir);
            }
            else if (dir >= 180 && dir < 270)
            {
                dir = 90 + (270 - dir);
            }
            else if (dir >= 270 && dir < 360)
            {
                dir = (360 - dir);
            }
        }
        map->map1[int(y_end)][int(x_end)] = design;
        this->x = x_end;
        this->y = y_end;
    }
    bool CheckPlayerCollision(TMap *_map, int player_pos_x, int player_pos_y, int player_size)
    {
        short int rand_int;
        if (player_pos_y == int(this->y))
        {
            for (int i = player_pos_x; i < player_pos_x + player_size; i++)
            {
                rand_int = (rand() % 45 + 1);
                if (i == int(this->x) && (i < int(player_pos_x + (player_size / 2))))
                {
                    if (dir >= 0 && dir < 90)
                    {
                        dir = 270 - (90 - dir);
                    }
                    else if (dir >= 90 && dir < 180)
                    {
                        if (dir == 90)
                        {
                            dir = rand_int + 202;
                        }
                        else
                        {
                            dir = 270 - (dir - 90);
                        }
                    }
                }
                else if (i == int(this->x) && (i == int(player_pos_x + (player_size / 2))))
                {
                    if (dir >= 0 && dir < 90)
                    {
                        dir = 270;
                    }
                    else if (dir >= 90 && dir < 180)
                    {
                        dir = 270;
                    }
                }
                else if (i == int(this->x) && (i > int(player_pos_x + (player_size / 2))))
                {
                    if (dir >= 0 && dir < 90)
                    {
                        dir = 270 + (90 - dir);
                    }
                    else if (dir >= 90 && dir < 180)
                    {
                        if (dir == 90)
                        {
                            dir = rand_int + 292;
                        }
                        else
                        {
                            dir = 270 + (dir - 90);
                        }
                    }
                }
            }
        }
        else if (this->y > mHeight - 2)
        {
            this->y = 10;
            return false;
        }
        return true;
    }

    bool CheckBrickCollision(TMap *_map, TMap *_bricks_map, int offset_y, int offset_x)
    {
        brick_map = _bricks_map;
        brick_on_map_y = int(this->y) - offset_y;
        brick_on_map_x = int(this->x) - offset_x;
        if (brick_on_map_y < 0 || brick_on_map_x < 0 || brick_on_map_y > bHeight -1 || brick_on_map_x > bWidth -1)
            return false;
        if (brick_map->map1[brick_on_map_y][brick_on_map_x] != ' ')
        {
            brick_map->map1[brick_on_map_y][brick_on_map_x] = ' ';
            return true;
        }
        return false;
    }

private:
    float x, y, x_end, y_end, dir, speed;
    int brick_on_map_y, brick_on_map_x;
    TMap *map;
    TMap *brick_map;
    char design = 'O';
};

int main()
{
    TMap global_map;
    TMap brick_map;
    global_map.Border();

    Player Pl((mWidth / 2), (mHeight - 3), PlayerSize, 'T', PlayerLives, &global_map);
    Pl.SetCoins(0);

    BlockPlaser Blocks(&brick_map, 2, 2);
    Ball Orb('O', mHeight/2, mWidth/2);
    while (GetKeyState(VK_ESCAPE) >= 0)
    {
        global_map.Clear();
        Blocks.Update(&global_map);
        Orb.Update(&global_map);
        if (!Orb.CheckPlayerCollision(&global_map, Pl.GetPosX(), Pl.GetPosY(), Pl.GetSize()))
        {
            if (Pl.IsLastLive())
            {
                return 0;
            }
        }
        if (Orb.CheckBrickCollision(&global_map, &brick_map, 2, 2))
            Pl.SetCoins(Pl.GetCoins() + 1);
        Pl.Move();
        global_map.Show();

        MoveXY(mWidth + 1, 1);
        cout << "Lives: " << Pl.GetLives();
        MoveXY(mWidth + 1, 2);
        cout << "Coins: " << Pl.GetCoins();
        MoveXY(mWidth + 1, mHeight - 1);
        cout << "ESC: exit";
        Sleep(100);
    }

    return 0;
}
// MoveXY(5, mHeight + 2);
// cout << "col. i=" << i << " x=" << int(this->x) << "  " << int(player_pos_x + (player_size / 2)) << endl;
