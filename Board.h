#pragma once
#include "TextureMng.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Board {
    int windowW;
    int windowH;
    int tileCount;
    int columns; // directly from document
    int rows;
    vector<vector<Tile>> tiles;
    vector<vector<sf::Sprite>> tileHidden;

public:
    int flagsRemaining;
    int mineCount;
    Board();
    Board(int col, int row, int mineCount);
    int GetWindowW() const;
    int GetWindowH() const;
    void placeMines();
    bool CheckWin();
    int GetRows() const;
    int GetCol() const;
    vector<vector<Tile>>& GetTileGrid();
    void SetTileGrid(vector<vector<Tile>> &t);
    int GetMineCount() const;
    int CountNeighborMines(int &x, int &y);
    void Neighbors();
    // in main: iterate through tile vectorvector, if (!tile.isaMine()), tile.SetNeighbors(board.CountNeighborMines(x,y)); x y is pos of tile
    void RecursiveReveal(int x, int y);

};

