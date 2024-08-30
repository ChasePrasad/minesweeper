#pragma once
#include "tile.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <string>
#include <set>
#include <cstdlib>
using namespace std;
using namespace sf;

struct Board
{
    int columns;
    int rows;
    int mines;
    vector<vector<Tile>> tiles;
    bool paused = false;

    Board (int c, int r, int m, int x, int y)
    {
        columns = c;
        rows = r;
        mines = m;

        int startCol = x;
        int startRow = y;
        
        for (int i = 0; i < r; i++)
        {
            vector<Tile> row;

            for (int j = 0; j < c; j++)
            {
                Tile t(j * 32, i * 32);
                row.push_back(t);
            }

            tiles.push_back(row);
        }

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                Tile* t = &tiles[i][j];

                if (i > 0)
                {
                    t->upper = &tiles[i - 1][j];
                    if (j > 0)
                    {
                        t->upperLeft = &tiles[i - 1][j - 1];
                    }
                    if (j < c - 1)
                    {
                        t->upperRight = &tiles[i - 1][j + 1];
                    }
                }
                if (i < r - 1)
                {
                    t->lower = &tiles[i + 1][j];
                    if (j > 0)
                    {
                        t->lowerLeft = &tiles[i + 1][j - 1];
                    }
                    if (j < c - 1)
                    {
                        t->lowerRight = &tiles[i + 1][j + 1];
                    }
                }
                if (j > 0)
                {
                    t->left = &tiles[i][j - 1];
                }
                if (j < c - 1)
                {
                    t->right = &tiles[i][j + 1];
                }
            }
        }

        set<int> mineIndices;
        set<int> excludedIndices;

        for (int i = startRow - 1; i <= startRow + 1; i++)
        {
            for (int j = startCol - 1; j <= startCol + 1; j++)
            {
                if (i >= 0 && i < r && j >= 0 && j < c)
                {
                    excludedIndices.insert(i * c + j);
                }
            }
        }

        for (int i = 0; i < m; i++)
        {
            int index = rand() % (c * r);
            while (mineIndices.find(index) != mineIndices.end() || excludedIndices.find(index) != excludedIndices.end())
            {
                index = rand() % (c * r);
            }
            mineIndices.insert(index);
        }

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                Tile* t = &tiles[i][j];
                if (mineIndices.find(i * c + j) != mineIndices.end())
                {
                    t->mine = true;
                }
            }
        }

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                Tile* t = &tiles[i][j];

                if (!t->mine)
                {
                    int count = 0;

                    if (t->upperLeft != nullptr && t->upperLeft->mine)
                    {
                        count++;
                    }
                    if (t->upper != nullptr && t->upper->mine)
                    {
                        count++;
                    }
                    if (t->upperRight != nullptr && t->upperRight->mine)
                    {
                        count++;
                    }
                    if (t->left != nullptr && t->left->mine)
                    {
                        count++;
                    }
                    if (t->right != nullptr && t->right->mine)
                    {
                        count++;
                    }
                    if (t->lowerLeft != nullptr && t->lowerLeft->mine)
                    {
                        count++;
                    }
                    if (t->lower != nullptr && t->lower->mine)
                    {
                        count++;
                    }
                    if (t->lowerRight != nullptr && t->lowerRight->mine)
                    {
                        count++;
                    }

                    t->num = count;
                }
            }
        }
    }

    void print(RenderWindow &win, Texture &t)
    {
        if (!paused)
        {
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[i].size(); j++)
                {
                    Tile tile = tiles[i][j];
                    Sprite tileSprite;

                    t.loadFromFile("files/images/tile_revealed.png");
                    tileSprite.setTexture(t);
                    tileSprite.setPosition(tile.x, tile.y);
                    win.draw(tileSprite);
                    if(tile.num > 0)
                    {
                        t.loadFromFile("files/images/number_" + to_string(tile.num) + ".png");
                        tileSprite.setTexture(t);
                        tileSprite.setPosition(tile.x, tile.y);
                        win.draw(tileSprite);
                    }
                    if (tile.mine)
                    {
                        t.loadFromFile("files/images/mine.png");
                        tileSprite.setTexture(t);
                        tileSprite.setPosition(tile.x, tile.y);
                        win.draw(tileSprite);
                    }
                    if (tile.hidden)
                    {
                        t.loadFromFile("files/images/tile_hidden.png");
                        tileSprite.setTexture(t);
                        tileSprite.setPosition(tile.x, tile.y);
                        win.draw(tileSprite);
                    }
                    if (tile.flagged)
                    {
                        t.loadFromFile("files/images/flag.png");
                        tileSprite.setTexture(t);
                        tileSprite.setPosition(tile.x, tile.y);
                        win.draw(tileSprite);
                    }
                    if (tile.lost || tile.debug)
                    {
                        t.loadFromFile("files/images/mine.png");
                        tileSprite.setTexture(t);
                        tileSprite.setPosition(tile.x, tile.y);
                        win.draw(tileSprite);
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[i].size(); j++)
                {
                    Tile tile = tiles[i][j];
                    Sprite tileSprite;

                    t.loadFromFile("files/images/tile_revealed.png");
                    tileSprite.setTexture(t);
                    tileSprite.setPosition(tile.x, tile.y);
                    win.draw(tileSprite);
                }
            }
        }
    }

    void debug()
    {
        if (!paused)
        {
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[i].size(); j++)
                {
                    if (tiles[i][j].mine)
                    {
                        tiles[i][j].debug = !tiles[i][j].debug;
                    }
                }
            }
        }
    }

    void pause()
    {
        paused = !paused;
    }

    bool flag(int c, int r)
    {
        if (!paused)
        {
            if (tiles[r][c].hidden)
            {
                tiles[r][c].flagged = !tiles[r][c].flagged;
                return tiles[r][c].flagged;
            }
        }
    }

    bool reveal(int c, int r)
    {
        if (!paused)
        {
            if (tiles[r][c].flagged)
            {
                return true;
            }
            else if (tiles[r][c].hidden)
            {
                tiles[r][c].hidden = false;

                if (tiles[r][c].mine)
                {
                    for (int i = 0; i < tiles.size(); i++)
                    {
                        for (int j = 0; j < tiles[i].size(); j++)
                        {
                            if (tiles[i][j].mine)
                            {
                                tiles[i][j].hidden = false;
                                tiles[i][j].lost = true;
                            }
                        }
                    }

                    return false;
                }
                if (tiles[r][c].num == 0)
                {
                    if (tiles[r][c].upperLeft != nullptr && tiles[r][c].upperLeft->hidden)
                    {
                        reveal(tiles[r][c].upperLeft->x / 32, tiles[r][c].upperLeft->y / 32);
                    }
                    if (tiles[r][c].upper != nullptr && tiles[r][c].upper->hidden)
                    {
                        reveal(tiles[r][c].upper->x / 32, tiles[r][c].upper->y / 32);
                    }
                    if (tiles[r][c].upperRight != nullptr && tiles[r][c].upperRight->hidden)
                    {
                        reveal(tiles[r][c].upperRight->x / 32, tiles[r][c].upperRight->y / 32);
                    }
                    if (tiles[r][c].left != nullptr && tiles[r][c].left->hidden)
                    {
                        reveal(tiles[r][c].left->x / 32, tiles[r][c].left->y / 32);
                    }
                    if (tiles[r][c].right != nullptr && tiles[r][c].right->hidden)
                    {
                        reveal(tiles[r][c].right->x / 32, tiles[r][c].right->y / 32);
                    }
                    if (tiles[r][c].lowerLeft != nullptr && tiles[r][c].lowerLeft->hidden)
                    {
                        reveal(tiles[r][c].lowerLeft->x / 32, tiles[r][c].lowerLeft->y / 32);
                    }
                    if (tiles[r][c].lower != nullptr && tiles[r][c].lower->hidden)
                    {
                        reveal(tiles[r][c].lower->x / 32, tiles[r][c].lower->y / 32);
                    }
                    if (tiles[r][c].lowerRight != nullptr && tiles[r][c].lowerRight->hidden)
                    {
                        reveal(tiles[r][c].lowerRight->x / 32, tiles[r][c].lowerRight->y / 32);
                    }

                    return true;
                }

                return true;
            }
        }
    }

    bool checkWin()
    {
        if (!paused)
        {
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[i].size(); j++)
                {
                    if (!tiles[i][j].mine && tiles[i][j].hidden)
                    {
                        return false;
                    }
                }
            }

            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[i].size(); j++)
                {
                    if (tiles[i][j].mine && !tiles[i][j].flagged)
                    {
                        tiles[i][j].flagged = true;
                    }
                    tiles[i][j].debug = false;
                }
            }

            return true;
        }

        return false;
    }
};