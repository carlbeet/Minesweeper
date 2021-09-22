#pragma once
#include <SFML/Graphics.hpp>
#include "TextureMng.h"
#include <iostream>

using namespace std;

class Tile {

    int x, y;
    bool isMine;
    bool flagged;
    int neighboringMineCt; // tells us what number to show, if zero, is empty.
    int state; // tells us which sprites to show
    // inputs: hidden 0, flagged 1, revealed 2

    string r = "tile_revealed";
    string flag = "flag";
    string mine = "mine";
    string h = "tile_hidden";
    sf::Sprite hiddenSprite;
    sf::Sprite revealedSprite;
    sf::Sprite flagSprite;
    vector<sf::Sprite> numberSprites;

//   sf::Texture::tileTexture;
    // load texture and then sprite
public:
    Tile();
    Tile(int& col, int& row);
    // Setters
    void SetState(int state); // 0 hidden, 1 flagged, 2 revealed
    void SetMine();
    void SetNeighbors(int neighbors);


    // Getters
    int GetState();
    bool isaMine();
    int GetNeighbors();
    void DrawTile(sf::RenderWindow &w, int col, int row);

    // case 1. is a mine
    // case 2. is empty (a. no number on top, or b. number)
    // case 3. is flagged


// each tile has a texture;
//1. set flag, set revealed, set number, set mine
// set texture to new texture

};
