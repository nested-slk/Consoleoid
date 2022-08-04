#include <iostream>
#include <Windows.h>
#include <String.h>
#include <cmath>
#include <vector>
using namespace std;

struct GameMap;

#define MAP_WIDHT 40
#define MAP_HEIGHT 20
#define BLOCKS_OFFSET_X 4
#define BLOCKS_OFFSET_Y 3

#define PLAYER_MOVE_LEFT_BTN 'A'
#define PLAYER_MOVE_RIGHT_BTN 'D'
#define PLAYER_SELECT_BTN VK_SPACE
#define PLAYER_EXIT_BTN VK_ESCAPE

#define PLAYER_SIZE 7
#define PLAYER_LIVES 3
#define PLAYER_POSITION_X ((MAP_WIDHT / 2) - (PLAYER_SIZE / 2))
#define PLAYER_POSITION_Y (MAP_HEIGHT - 3)
#define PLAYER_SYMBOL 'T'

#define BALL_SPEED 1
#define BLOCK_SIZE 2

#define MAX_MODE 2

struct FloatCoord
{
    float x;
    float y;
};
struct GameMap
{
    vector<vector<char>> globalMapVec;
    vector<vector<char>> borderMapVec;
    vector<vector<int>> globalMapDesignVec;

    vector<char> playerDesignMapVec;
    char ballDesign;
    char playerDesign;

    COORD mapSize;
    COORD ballPose;
    COORD playerPose;

    short int playerSize;
    void placeOnMap(COORD position, char symbol, WORD textColor, WORD bgColor)
    {
        if (position.X > 0 && position.X <= this->mapSize.X && position.Y > 0 && position.Y <= this->mapSize.Y) //
        {
            globalMapVec[position.Y][position.X] = symbol;
            globalMapDesignVec[position.Y][position.X] = setForeGroundAndBackGroundColor(textColor, bgColor);
        }
    }
    void placeOnMap(COORD position, char symbol, WORD dataColor)
    {
        if (position.X > 0 && position.X <= this->mapSize.X && position.Y > 0 && position.Y <= this->mapSize.Y) //
        {
            globalMapVec[position.Y][position.X] = symbol;
            globalMapDesignVec[position.Y][position.X] = dataColor;
        }
    }
    void createBorder(char leftWallSymbol, char rightWallSymbol, char bottomtWallSymbol, char topWallSymbol)
    {
        for (int i = 0; i < this->mapSize.Y; i++)
        {
            for (int j = 0; j < this->mapSize.X; j++)
            {
                if (i == 0) // find borders positions
                {
                    globalMapVec[i][j] = topWallSymbol;
                    globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(0, 8);
                }
                else if (i == mapSize.Y - 1)
                {
                    globalMapVec[i][j] = bottomtWallSymbol;
                    globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(0, 8);
                }
                else if (j == 0)
                {
                    globalMapVec[i][j] = leftWallSymbol;
                    globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(0, 8);
                }
                else if (j == mapSize.X - 1)
                {
                    globalMapVec[i][j] = rightWallSymbol;
                    globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(0, 8);
                }
            }
        }
    }
    void createMap(int x, int y)
    {
        this->mapSize.X = x;
        this->mapSize.Y = y;

        globalMapVec.resize(this->mapSize.Y);
        for (int i = 0; i < this->mapSize.Y; i++)
        {
            globalMapVec[i].resize(this->mapSize.X + 1);
            for (int j = 0; j < this->mapSize.X; j++)
            {
                globalMapVec[i][j] = (' ');
            }
            globalMapVec[i][this->mapSize.X] = ('\n');
        }

        globalMapDesignVec.resize(this->mapSize.Y);
        for (int i = 0; i < this->mapSize.Y; i++)
        {
            globalMapDesignVec[i].resize(this->mapSize.X + 1);
            for (int j = 0; j < this->mapSize.X; j++)
            {
                globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(15, 1);
            }
            globalMapDesignVec[i][mapSize.X] = setForeGroundAndBackGroundColor(15, 0);
        }
    }

    void showMap()
    {
        COORD pos;
        pos.X = 0;
        pos.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        for (int i = 0; i < mapSize.Y; i++)
        {
            for (int j = 0; j <= mapSize.X; j++)
            {
                setcolor(globalMapDesignVec[i][j]);
                cout << globalMapVec[i][j];
            }
        }
    }
    COORD getSize()
    {
        return mapSize;
    }
    void clearMap()
    {
        for (int i = 1; i < mapSize.Y - 1; i++)
        {
            for (int j = 1; j < mapSize.X - 1; j++)
            {
                globalMapVec[i][j] = ' ';
                globalMapDesignVec[i][j] = setForeGroundAndBackGroundColor(15, 1);
            }
        }
    }

    void setcolor(WORD color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    //     colors:
    //     0 = Black
    //     1 = Blue
    //     2 = Green
    //     3 = Cyan
    //     4 = Red
    //     5 = Magenta
    //     6 = Yellow
    //     7 = LightGray
    //     8 = DarkGray
    //     9 = LightBlue
    //     10 = LightGreen
    //     11 = LightCyan
    //     12 = LightRed
    //     13 = LightMagenta
    //     14 = LightYellow
    //     15 = White
    int setForeGroundAndBackGroundColor(int ForeGroundColor, int BackGroundColor)
    {
        int color = 16 * BackGroundColor + ForeGroundColor;
        return (color);
    }
};
class Player
{
public:
    Player(int start_pos_x, int start_pos_y, short int size, char design, short int lives, GameMap *gameMap)
    {
        gameMap_ = gameMap;
        this->position_.X = start_pos_x;
        this->position_.Y = start_pos_y;
        lives_ = lives;
        coins_ = 0;
        size_ = size;
        coins_ = 0;
        // draw(size, design);
    }

    void draw(short int size = 4, char design = '=') // add player to the global map
    {
        design_ = design;
        size_ = size;
        for (int j = 0; j < size; j++)
        {
            // map_->map[pos_y_][pos_x_ + j] = design_;
        }
    }
    // TODO: merge changePos and move
    void changePosOnMap(void) // update player position on global map
    {
        for (int j = 0; j < size_; j++)
        {
            COORD pos;
            pos.X = position_.X + j;
            pos.Y = position_.Y;
            gameMap_->placeOnMap(pos, 'T', 0, 6);
        }
    }
    void move(int max_x, int max_y, char up = 'w', char left = 'A', char down = 'S', char right = 'D') // player control from keyboard
    {
        if ((GetKeyState(left) & 0x8000) && this->position_.X > 1) // check pressed key and player position to stay between borders
        {
            this->position_.X -= 1;
        }
        else if ((GetKeyState(right) & 0x8000) && this->position_.X < max_x - 1 - size_) // check pressed key and player position to stay between borders
        {
            this->position_.X += 1;
        }
        changePosOnMap();
    }
    COORD getPose()
    {
        return position_;
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
    void addCoins(int coins)
    {
        coins_ += coins;
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
    COORD position_;
    GameMap *gameMap_;
    int coins_;
    short int size_, lives_;
    char design_;
};

class BlockPlaser
{
public:
    BlockPlaser(int x, int y, GameMap *bricksMap) // set bricks on brick map and set the offest from the global map
    {
        bricksMap_ = bricksMap;
        mapSize.X = x;
        mapSize.Y = y;
        // this->createBricks(2, 1, 2);
        //     bricks_map_ = map;
        //     offset_x_ = offset_x;
        //     offset_y_ = offset_y;
        //     short int rand_int;
        //     max_coins_ = 0;
        //     // TODO: add clear method in GameMap
        //     for (int j = 0; j < BRICKS_HEIGHT; j += 1) // clear brick map
        //     {
        //         for (int i = 0; i < BRICKS_WIDTH; i += 1)
        //         {

        //             bricks_map_->map[j][i] = ' ';
        //         }
        //     }
        //     for (int j = 0; j < BRICKS_HEIGHT; j += 2) // add on map various blocks
        //     {
        //         for (int i = 0; i < BRICKS_WIDTH; i += 2)
        //         {
        //             this->max_coins_++;
        //             rand_int = (rand() % 3 + 1);
        //             if (rand_int == 1)
        //                 bricks_map_->map[j][i] = 'V';
        //             else if (rand_int == 2)
        //                 bricks_map_->map[j][i] = 'U';
        //             else
        //                 map->map[j][i] = 'v';
        //         }
        //     }
    }
    void createBricks(int brickSize, int stepX, int stepY, GameMap *bricksMap)
    {
        bricksMap_ = bricksMap;
        for (int i = 0; i < this->mapSize.Y; i += 2)
        {
            for (int j = 0; j < this->mapSize.X; j = j + brickSize + stepX)
            {
                for (int k = 0; k < brickSize; k++)
                {
                    bricksMap_->globalMapVec[i][j + k] = ('H');
                    bricksMap_->globalMapDesignVec[i][j + k] = bricksMap_->setForeGroundAndBackGroundColor(5, 14);
                }
                maxCoins_++;
            }
        }
    }

    void changePosOnMap(GameMap *map, int offset_x, int offset_y) // add bricks on global map
    {
        offset_x_ = offset_x;
        offset_y_ = offset_y;
        for (int j = 0; j < this->mapSize.Y; j++)
        {
            for (int i = 0; i < this->mapSize.X; i++)
            {
                COORD pos;
                pos.X = i + offset_x_;
                pos.Y = j + offset_y_;
                map->placeOnMap(pos, bricksMap_->globalMapVec[j][i], bricksMap_->globalMapDesignVec[j][i]);
            }
        }
    }
    int getMaxCoins()
    {
        return maxCoins_;
    }
    // ~BlockPlaser();
private:
    // Pl pl;
    GameMap *gameMap_;
    GameMap *bricksMap_;
    COORD mapSize;
    int offset_x_, offset_y_, maxCoins_ = 0;
};
class Button
{
public:
    Button(string text, COORD position)
    {
        position_ = position;
        text_ = text;
    }
    void placeButtonOnMap(GameMap *gameMap)
    {
        for (int k = 0; k < text_.length(); k++)
        {
            COORD pos;
            pos.X = position_.X + k;
            pos.Y = position_.Y;
            gameMap->placeOnMap(pos, text_[k], gameMap->setForeGroundAndBackGroundColor(5, 14));
        }
    }
    bool checkButtonPlayerCollision(Player *player)
    {
        for (int k = 0; k < text_.length(); k++)
        {
            if (position_.X + k == player->getPose().X)
            {
                return true;
            }
        }
    }

private:
    COORD position_;
    string text_;
};

class Ball
{
public:
    Ball(int start_pos_x, int start_pos_y, float speed, GameMap *gameMap, char design = 'O')
    {
        gameMap_ = gameMap;

        position_.x = start_pos_x;
        position_.y = start_pos_y;
        speed_ = speed;
        dir_ = 90.0; // start direction 0 - right, 90 - down, 270 up
        design_ = design;
    }
    void changePosOnMap() // update ball position on map and check collision
    {
        COORD pos;
        pos.X = position_.x;
        pos.Y = position_.y;
        gameMap_->placeOnMap(pos, design_, 4, 1);
    }
    void move(int max_x, int max_y) // update ball position on map and check collision
    {
        newPosition_.x = position_.x + speed_ * cos((dir_ * M_PI) / 180.0); // calculate new ball coordinates

        newPosition_.y = position_.y + speed_ * sin((dir_ * M_PI) / 180.0);
        if (newPosition_.x < 1 || newPosition_.y < 1 || newPosition_.x > max_x - 1 || newPosition_.y > max_y - 1) // check out of border
        {
            newPosition_.x = float(max_x / 2);
            newPosition_.y = float(max_y / 2);
        }
        if (newPosition_.x < 2 || newPosition_.x > max_x - 3) // check collision with border and bounce from border
        {
            if (dir_ >= 360)
                dir_ = 0;
            if (dir_ >= 0 && dir_ < 90)
            {
                dir_ = 180 - dir_;
            }
            else if (dir_ >= 90 && dir_ < 180)
            {
                dir_ = (180 - dir_);
            }
            else if (dir_ >= 180 && dir_ < 270)
            {
                dir_ = 360 - (dir_ - 180);
                if (dir_ == 360)
                    dir_ = 0;
            }
            else if (dir_ >= 270 && dir_ < 360)
            {
                dir_ = 270 - (dir_ - 270);
            }
        }
        else if (newPosition_.y < 2)
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
        position_.x = newPosition_.x;
        position_.y = newPosition_.y;
    }
    bool checkPlayerCollision(Player *player) // check collision with player
    {
        short int rand_int = 1;
        if (player->getPose().Y - 1 == int(this->position_.y)) // check player and ball line
        {
            for (int i = player->getPose().X; i < player->getPose().X + player->getSize(); i++) // check side of player collision with ball
            {
                rand_int = (rand() % 45 + 1);
                if (i == int(position_.x) && (i < int(player->getPose().X + (player->getSize() / 2)))) // bounce from left side of the player
                {
                    if (dir_ >= 0 && dir_ < 90)
                    {
                        dir_ = 270 - (90 - rand_int + 22);
                    }
                    else if (dir_ >= 90 && dir_ < 180)
                    {
                        if (dir_ == 90)
                        {
                            dir_ = rand_int + 202;
                        }
                        else
                        {
                            dir_ = 270 - (rand_int + 112 - 90);
                        }
                    }
                }
                else if (i == int(position_.x) && (i == int(player->getPose().X + (player->getSize() / 2)))) // bounce from center of the player
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
                else if (i == int(position_.x) && (i > int(player->getPose().X + (player->getSize() / 2)))) // bounce from right side of the player
                {
                    if (dir_ >= 0 && dir_ < 90)
                    {
                        dir_ = 270 + (90 - (rand_int + 22));
                    }
                    else if (dir_ >= 90 && dir_ < 180)
                    {
                        if (dir_ == 90)
                        {
                            dir_ = rand_int + 292;
                        }
                        else
                        {
                            dir_ = 270 + (rand_int + 112 - 90);
                        }
                    }
                }
            }
        }
        else if (position_.y > player->getPose().Y) // if ball under player
        {
            // TODO: do lost ball event better
            this->position_.y = MAP_HEIGHT / 2;
            this->dir_ = 90;
            return false;
        }
        return true;
    }

    bool checkBrickCollision(GameMap *bricks_map, int offset_x, int offset_y) // check ball and brick collision
    {
        // brick_map_ = bricks_map;
        ballInBricksMap.Y = int(position_.y) - offset_y;
        ballInBricksMap.X = int(position_.x) - offset_x;
        if (ballInBricksMap.Y < 0 || ballInBricksMap.X < 0 || ballInBricksMap.Y > bricks_map->mapSize.Y - 1 || ballInBricksMap.X > bricks_map->mapSize.X - 1)
            return false;
        if (bricks_map->globalMapVec[ballInBricksMap.Y][ballInBricksMap.X] != ' ')
        {
            // TODO: add bounce brom brick
            bricks_map->globalMapVec[ballInBricksMap.Y][ballInBricksMap.X] = ' ';                                                      // clear brick position after hit them
            bricks_map->globalMapDesignVec[ballInBricksMap.Y][ballInBricksMap.X] = bricks_map->setForeGroundAndBackGroundColor(15, 1); // clear brick position after hit them

            return true;
        }
        return false;
    }
    // ~Ball();

private:
    FloatCoord position_;
    FloatCoord newPosition_;
    COORD ballInBricksMap;
    GameMap *gameMap_;
    char design_;
    float dir_, speed_;
    // int ballInBricksMap.Y, ballInBricksMap.X;
    GameMap *map_;
    GameMap *brick_map_;
};

class Game
{
public:
    Game() : Pl(PLAYER_POSITION_X, PLAYER_POSITION_Y, PLAYER_SIZE, PLAYER_SYMBOL, PLAYER_LIVES, &gameMap),
             Orb(MAP_WIDHT / 2, MAP_HEIGHT / 2, BALL_SPEED, &gameMap),
             Blocks(MAP_WIDHT / 4, MAP_HEIGHT / 5, &bricksMap),
             NewGame("new game", COORD{3, PLAYER_POSITION_Y})
    {
        h_ = GetStdHandle(STD_OUTPUT_HANDLE); // console object;
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = false; // invisible cursor
        cursor.dwSize = 1;
        SetConsoleCursorInfo(h_, &cursor); // update cursor in console
    }
    bool menu()
    {
        this->show();
        this->showKeyInfo();
        Pl.move(MAP_WIDHT, MAP_HEIGHT);
        NewGame.placeButtonOnMap(&gameMap);
        if (NewGame.checkButtonPlayerCollision(&Pl))
            return false;
        return true;
    }
    bool doGame()
    {
        this->move();
        this->update();
        this->checkPlayerCollision();
        this->checkBrickCollision();
        this->show();
        this->showGameInfo();
        this->showKeyInfo();
        return true;
    }
    void createGameMap()
    {
        gameMap.createMap(MAP_WIDHT, MAP_HEIGHT);
        gameMap.createBorder('|', '|', '=', '-');
        bricksMap.createMap(MAP_WIDHT / 4, MAP_HEIGHT / 5);
        Blocks.createBricks(BLOCK_SIZE, 1, 1, &bricksMap);
        // gameMap.showMap();
    }
    void update()
    {
        Blocks.changePosOnMap(&gameMap, BLOCKS_OFFSET_X, BLOCKS_OFFSET_Y);
        Orb.changePosOnMap();
    }
    void move()
    {
        Pl.move(MAP_WIDHT, MAP_HEIGHT);
        Orb.move(MAP_WIDHT, MAP_HEIGHT);
    }
    void checkPlayerCollision()
    {
        if (!Orb.checkPlayerCollision(&Pl))
        {
            if (Pl.isLastLive())
            {
                Sleep(1000);
                // return 0;
            }
        }
    }
    void checkBrickCollision()
    {
        if (Orb.checkBrickCollision(&bricksMap, BLOCKS_OFFSET_X, BLOCKS_OFFSET_Y))
        {
            Pl.addCoins(1);
            if (Pl.getCoins() >= Blocks.getMaxCoins())
            {
                Sleep(1000);
                // return 0;
            }
        }
    }
    void show()
    {
        gameMap.showMap();
        gameMap.clearMap();
    }
    void showGameInfo()
    {
        SetConsoleTextAttribute(h_, 14); // show additional information in console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, 1});
        cout << "Lives: " << Pl.getLives();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, 2});
        cout << "Coins: " << Pl.getCoins();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, 4});
        cout << "Max coins: " << Blocks.getMaxCoins();
    }
    void showKeyInfo()
    {
        SetConsoleTextAttribute(h_, 14);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, MAP_HEIGHT - 2});
        cout << PLAYER_EXIT_BTN << ": exit";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, MAP_HEIGHT - 3});
        cout << PLAYER_SELECT_BTN << ": select";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, MAP_HEIGHT - 4});
        cout << PLAYER_MOVE_LEFT_BTN << ": left";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{MAP_WIDHT + 1, MAP_HEIGHT - 5});
        cout << PLAYER_MOVE_RIGHT_BTN << ": right";
    }

private:
    HANDLE h_;
    GameMap gameMap;
    GameMap bricksMap;
    Player Pl;
    Ball Orb;
    BlockPlaser Blocks;
    Button NewGame;
};
int main()
{
    Game Consoleoid;
    short int mode = 1;
    Consoleoid.createGameMap();
    while (GetKeyState(VK_ESCAPE) >= 0) // ESC key to close console
    {
        switch (mode)
        {
        case 1: // menu
            if (!Consoleoid.menu())
            {
                mode++;
            }
            break;
        case 2: // game
            if (!Consoleoid.doGame())
            {
                mode++;
            }
            break;
        default:
            break;
        }
        if (mode > MAX_MODE)
        {
            mode = 1;
        }
        Sleep(25); // rate
    }

    /*
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


        // TODO: add a speed control
        Sleep(100); // set game speed
    }
*/
    return 0;
}
