#pragma once

struct Tile
{
    int x;
    int y;

    Tile* upperLeft = nullptr;
    Tile* upper = nullptr;
    Tile* upperRight = nullptr;
    Tile* left = nullptr;
    Tile* right = nullptr;
    Tile* lowerLeft = nullptr;
    Tile* lower = nullptr;
    Tile* lowerRight = nullptr;

    bool hidden = true;
    int num = 0;
    bool mine = false;
    bool flagged = false;
    bool debug = false;
    bool lost = false;

    Tile (int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};