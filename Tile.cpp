
#include "Tile.h"
Tile::Tile(){
    state = 0; // hidden [0] , flagged [1] or revealed [2].
    isMine = false;
    flagged = false;
    neighboringMineCt = 0;

    hiddenSprite.setTexture((TextureMng::GetTexture(h)));
    revealedSprite.setTexture((TextureMng::GetTexture(r)));
    flagSprite.setTexture(TextureMng::GetTexture(flag));
    for (int i = 1; i < 9; i++) { // numbers: 4-11
        string k = to_string(i);
        string s = "number_" + k;
        sf::Sprite numSprite((TextureMng::GetTexture(s)));
        numberSprites.push_back(numSprite); // indexes 0 - 7 for tile# 1 - 8
    }
}

Tile::Tile(int& col, int& row) { // in: Tile COL, ROW
    this->x = col * 32;
    this->y = (row * 32);
    state = 0; // hidden [0] , flagged [1] or revealed [2].
    isMine = false;
    flagged = false;
    neighboringMineCt = 0;

    hiddenSprite.setTexture((TextureMng::GetTexture(h)));
    revealedSprite.setTexture((TextureMng::GetTexture(r)));
    flagSprite.setTexture(TextureMng::GetTexture(flag));
    for (int i = 1; i < 9; i++) { // numbers: 4-11
        string k = to_string(i);
        string s = "number_" + k;
        sf::Sprite numSprite((TextureMng::GetTexture(s)));
        numberSprites.push_back(numSprite); // indexes 0 - 7 for tile# 1 - 8

    }

}

// sprite SFML documentation has a getbounds function
// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Sprite.php



void Tile::DrawTile(sf::RenderWindow &w, int col, int row) {
    // case 1. is a mine
    // case 2. is empty (a. no number on top, or b. number)
    // case 3. is flagged
    // states - hidden 0, flag 1, revealed 2
    sf::Sprite spriteVar1 = hiddenSprite;
    sf::Sprite spriteVar2 = hiddenSprite;

        if (this->state == 1) { // flagged
            spriteVar2 = flagSprite;
        }
        else if (this->state == 2) { // revealed
            spriteVar1 = revealedSprite;
            if (this->neighboringMineCt == 0) {
                spriteVar2 = revealedSprite;
            }
            else {
                spriteVar2 = numberSprites[neighboringMineCt - 1];
            }
    }

    spriteVar1.setPosition(col * 32, row * 32); // NOTE*** MINES ARE LOADED WITH POSITION (X,Y) INTO [Y,X] ARRAY
    spriteVar2.setPosition(col * 32, row * 32);

    w.draw(spriteVar1);
    w.draw(spriteVar2);

}
// change the state of the second tile.
// add a break point, see if change is persistent.
void Tile:: SetState(int s) {
        this->state = s;
        // inputs: hidden 0, flag 1, revealed 2
}

void Tile::SetNeighbors(int neighbors) {
    neighboringMineCt = neighbors;
}

void Tile::SetMine() {
    isMine = true;
    neighboringMineCt = -1;
}
bool Tile::isaMine() {
    return isMine;
}

int Tile::GetState() {
    return this->state;
}

int  Tile::GetNeighbors(){
    return neighboringMineCt;
}
;






