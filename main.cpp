#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Board.h"
#include "Tile.h"
#include "TextureMng.h"
#include "Random.h"
#include <cmath>
using namespace std;


int ConvertToInt(string &s);
vector<vector<sf::Sprite>> FillWithSprites(string &s, Board &b);
void RevealMines(Board &b,  sf::RenderWindow &w, vector<vector<sf::Sprite>> &mines, vector<vector<sf::Sprite>> &hidden);
void DrawTiles(Board &b,  sf::RenderWindow &w);
void ReadBoard(string &filepath, Board &b);
Board ReadFile(string &filepath);
void SetDigits(Board &b, sf::RenderWindow &w, vector<sf::Sprite>& digits);

// set win conditions and fix test board 1: get neighbors not working.

//TODO  fix drawTile in Tile.cpp and setting flag (if state is 1 or 2, tiles unresponsive)
// FIXED: passed tiles as reference for getTileGrid in boardcpp. could not use copy for processing tiles/drawing tiles in mousepressed
// set win conditions
// fix digits counter (line 78)

int main() {

    string file = "boards/config.cfg";
    string board1 = "boards/testboard1.brd";
    string board2 =  "boards/testboard2.brd";
    string board3 =  "boards/testboard3.brd";
    string boardMines = "board/lots_o_mines.brd";


    Board gameBoard = ReadFile(file);
    cout << endl;



    string r = "tile_revealed";
    string flag = "flag";
    string mine = "mine";
    string h = "tile_hidden";

    vector<vector<sf::Sprite>> hiddenSprites = (FillWithSprites(h, gameBoard));
    vector<vector<sf::Sprite>> mineSprites = (FillWithSprites(mine, gameBoard));



    // why is it giving smileys???
    vector<sf::Sprite> digitSprites;
 for (int i = 0; i < 11; i ++) // digitSprites store 11 things: indexes 0-9 and then 10, '-'
 {

     sf::Sprite s(TextureMng::GetTexture("digits"));
     s.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
     digitSprites.push_back(s);
 }



   // game window
    sf::RenderWindow window(sf::VideoMode(gameBoard.GetWindowW(), gameBoard.GetWindowH()), "Game display");


    sf::Sprite happyface(TextureMng::GetTexture("face_happy"));
    happyface.setPosition(gameBoard.GetWindowW()/2 - 32, gameBoard.GetRows() * 32);

    sf::Sprite loseface(TextureMng::GetTexture("face_lose"));
    loseface.setPosition(gameBoard.GetWindowW()/2 - 32, gameBoard.GetRows() * 32);

    sf::Sprite winface(TextureMng::GetTexture("face_win"));
    winface.setPosition(gameBoard.GetWindowW()/2 - 32, gameBoard.GetRows() * 32);

    sf::Sprite test1(TextureMng::GetTexture("test_1"));
    test1.setPosition(gameBoard.GetWindowW()/2 + 96 + 64, gameBoard.GetRows() * 32);
    sf::Sprite test2(TextureMng::GetTexture("test_2"));
    test2.setPosition(gameBoard.GetWindowW()/2 + 96 + 128, gameBoard.GetRows() * 32);
    sf::Sprite test3(TextureMng::GetTexture("test_3"));
    test3.setPosition(gameBoard.GetWindowW()/2 + 96 + 64*3, gameBoard.GetRows() * 32);

    sf::Sprite debug(TextureMng::GetTexture("debug")); //show mines
    debug.setPosition(gameBoard.GetWindowW()/2 + 96, gameBoard.GetRows() * 32);


    //nobody needs to know state except tile itself.
    bool gameOver = false;
    bool gameWin = false;
    bool revealMines = false;


// begin window loop =============================================================================================================


        // run the program as long as the window is open
        while (window.isOpen()) {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event:: MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);


                    if (gameOver) { //1. if gameover
                        if (loseface.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                //reset game.
                                gameBoard = ReadFile(file);
                                gameOver = false;
                            }
                        }
                    }
                        //- All tiles with mines are revealed (and display on top of any flags you may have place)
                   //- The smiley face changes to the dead face (he's just acting, don't worry!)
                   // - No further interactions with the game board are possible.
                   // The player CAN click the dead smiley face to start a new game, or use any of the testing buttons. (The debug button shouldn’t do anything at this point…)

                    else if (gameWin) // 2 if gamewin
                    {
                        if (winface.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                gameBoard = ReadFile(file);
                                gameWin = false;
                            }
                            }

                    }

                    else {
                        if (mousePos.y < gameBoard.GetRows() * 32) { //if clicked within tiles
                            Tile selectedTile;
                            int xt, yt;
                            for (int i = 0; i < gameBoard.GetRows(); i++) {
                                for (int j = 0; j < gameBoard.GetCol(); j++) {
                                    if (hiddenSprites[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                        selectedTile = gameBoard.GetTileGrid()[i][j]; // [y][x]
                                        xt = j;
                                        yt = i;

                                    }
                                }
                            }

                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (gameBoard.GetTileGrid()[yt][xt].GetState() != 1) { // cannot click on flagged spaces
                                    if (gameBoard.GetTileGrid()[yt][xt].isaMine()) {
                                        gameOver = true;
                                        // gameover function- end game, make some buttons unavailable, and reveal mines
                                    } else {
                                        // set to revealed

                                        if (gameBoard.GetTileGrid()[yt][xt].GetNeighbors() == 0) {
                                            // recursive reveal function gameBoard.RR(xt, yt)
                                            gameBoard.RecursiveReveal(xt, yt);

                                        } else {
                                            gameBoard.GetTileGrid()[yt][xt].SetState(2); // set to reveal
                                        }
                                    }
                                }
                            }
                            if (event.mouseButton.button == sf::Mouse::Right) {
                                if (gameBoard.GetTileGrid()[yt][xt].GetState() == 1) { //FLAG REMOVE
                                    gameBoard.GetTileGrid()[yt][xt].SetState(0);
                                    gameBoard.flagsRemaining++;

                                } else if (selectedTile.GetState() == 0) { //  st = "hidden";

                                    gameBoard.GetTileGrid()[yt][xt].SetState(1); //FLAG SET
                                    gameBoard.flagsRemaining--; // if we set down a flag, then goes from 50 to 49 and so on. can go negative.
//                                    cout << "state" << gameBoard.GetTileGrid()[yt][xt].GetState();


                                }
                            }

                        } else if (debug.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            cout << "debug click";
                            if (event.mouseButton.button == sf::Mouse::Left) // if left click
                            {
                                if (revealMines) { // debug toggle off
                                    revealMines = false;
                                } else {
                                    // if debug clicked, show mines-- debug toggle on
                                    revealMines = true;
                                }

                            }
                        } else if (happyface.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                cout << "happy click";
                                gameBoard = ReadFile(file); // overWrites gameboard with a new board

                                gameOver = false;
                                revealMines = false;

                            }
                        }
                    }
                    // access test buttons regardless of gameover
                         if (test1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left)
                                // reset game
                                gameBoard = ReadFile(file);
                            // this function copies tiles into gameboard's tile vectors vv
                            ReadBoard(board1, gameBoard);
                            gameBoard.Neighbors();
                        } else if (test2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left)
                                // reset game function
                                gameBoard = ReadFile(file);
                            ReadBoard(board2, gameBoard);
                            gameBoard.Neighbors();
                        } else if (test3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (event.mouseButton.button == sf::Mouse::Left)
                                // reset game function
                                gameBoard = ReadFile(file);
                            ReadBoard(board3, gameBoard);
                            gameBoard.Neighbors();
                        }
                    gameWin = gameBoard.CheckWin();

                    // update board after click?
                } // end ifMousePressed



            // draw function
            //smiley at x (Wwidth/2 - smileysize);
            window.clear();
            //before redrawing the board after user action
            // all drawing here


            if (gameOver){
                window.draw(loseface);
            }
            else if (gameWin) {
                window.draw(winface);
            }
            else {
                window.draw(happyface);
                }

            SetDigits(gameBoard, window, digitSprites);

            window.draw(debug);
            window.draw(test1);
            window.draw(test2);
            window.draw(test3);

            // iterate through tiles and draw tiles
            DrawTiles(gameBoard,  window); // draw tiles
             if (revealMines || gameOver) {
                 RevealMines(gameBoard, window, mineSprites, hiddenSprites);
             }


            window.display();
        }
        }
// user move: update state

    TextureMng :: Clear();
    return 0;
}


void SetDigits(Board &b, sf::RenderWindow &w, vector<sf::Sprite>& digits) {
// digits[0] = 0  ... digits[9] = 9, digits[10] = -
vector<sf::Sprite> digits2 = digits; //make copies
vector<sf::Sprite> digits3 = digits;
vector<sf::Sprite> digitsN = digits;
int negativeSign = 10;
int veryfirstDigit;
int firstDigit;
int secondDigit;

//cout << b.flagsRemaining;
// use integer division
if (b.flagsRemaining < 0) {
    digitsN[negativeSign].setPosition(0, b.GetRows() * 32);
    w.draw(digitsN[negativeSign]);
}

veryfirstDigit = abs(b.flagsRemaining / 100); //usually 0
   secondDigit  = abs( b.flagsRemaining % 10);

if (abs(b.flagsRemaining >= 100)) {
    firstDigit =abs( (b.flagsRemaining % 100) / 10);
}
else {
    firstDigit = abs( b.flagsRemaining / 10);
}
// does not let you use repeat digits because sprite is already in use.
//if (secondDigit == firstDigit || veryfirstDigit == secondDigit), need to use multiple digits vectors

    digits[veryfirstDigit].setPosition(21, b.GetRows() * 32);
    digits2[firstDigit].setPosition(21*2, b.GetRows() * 32);
    digits3[secondDigit].setPosition(21*3, b.GetRows() * 32);

    w.draw(digits[veryfirstDigit]);
    w.draw(digits2[firstDigit]);
    w.draw(digits3[secondDigit]);




}

void DrawTiles(Board &b, sf::RenderWindow &w) {
    vector<vector<Tile>> t = b.GetTileGrid();

    for (int k = 0; k < b.GetRows(); k++) {
        for (int j = 0; j < b.GetCol(); j++) {
            t[k][j].DrawTile(w, j, k);
        }
    }
}

void RevealMines(Board &b,  sf::RenderWindow &w, vector<vector<sf::Sprite>> &mines, vector<vector<sf::Sprite>> &hidden) {
    vector<vector<Tile>> t = b.GetTileGrid();


    for (int k = 0; k < b.GetRows(); k++) {
        for (int j = 0; j < b.GetCol(); j++) {

            if (t[k][j].isaMine()) // row, col
            {
                //tiles and sprites are stored [y,x] so mine sprite should be loaded [x,y]
//               w.draw(hidden[k][j]);
               w.draw(mines[k][j]); // displayed [y,x]
            }
        }
    }
}

//function: fillwithsprites
//could make a vector of this double vector to pass to functions, containing full filled vectors w sprites.
// in: string, empty double vector of sprites, board; out: full double vector
vector<vector<sf::Sprite>> FillWithSprites(string &s, Board &b) {

    vector<vector<sf::Sprite>> sprites;
    for (int k = 0; k < b.GetRows(); k++) {
        vector<sf::Sprite> rowofSprites;
        for (int j = 0; j < b.GetCol(); j++) {
            sf::Sprite spriteVar;
            spriteVar.setTexture(TextureMng::GetTexture(s));
            spriteVar.setPosition(j * 32, k * 32); // NOTE*** MINES ARE LOADED WITH POSITION [X,Y]  INTO [Y,X] ARRAY

            rowofSprites.push_back(spriteVar);
// for loop moves one row down, continually filling with rows of spriteVars (stored in a vector)
        }
        sprites.push_back(rowofSprites); // vector complete.
    }
    return sprites;
}

void ReadBoard(string &filepath, Board &b) {
    vector<vector<Tile>> boardNums;
    b.mineCount = 0;

    ifstream file(filepath);
    if (file.is_open()) {
            vector<int> numbers;
            string line;
            int fileNum;

            while (getline(file, line)) {
                for ( int i = 0; i < b.GetCol(); i ++) {
                fileNum = stoi(line.substr(i, 1));
                numbers.push_back(fileNum); }

            }
            // this part onward is sus
                int i = 0;
                // make grid from 1D list of ints
                for (int k = 0; k < b.GetRows(); k++) {
                    vector<Tile> boardNumsRow;
                    for (int j = 0; j < b.GetCol(); j++) {
                        Tile t(j, k); // (x, y) initialize tiles
                        if (numbers[i] == 1) {
                           t.SetMine();
                           b.mineCount++;
                        }
                        boardNumsRow.push_back(t);
                        i++;

                    }
                    boardNums.push_back(boardNumsRow);
                }
                //=======================
                //load into gameboard tile vector
        }
    b.flagsRemaining = b.mineCount;
    b.SetTileGrid(boardNums);
    }


Board ReadFile(string &filepath) { // read three integer values from file, return Board object with stored data
    int col, row, mineCount;


    ifstream file(filepath);
    if (file.is_open()) {
       vector<string> value;
        string line;


        while (getline(file, line)) {
            value.push_back(line);
        }

        col = ConvertToInt(value[0]);
        row = ConvertToInt(value[1]);
        mineCount = ConvertToInt(value[2]);
//        cout << col << " " << row << " " << mineCount << endl;
    }

    // read file data into col, row, mineCount
    Board b(col, row, mineCount);
    return b;
}

int ConvertToInt(string &s) {
    int myInt;
    try {
        myInt = stoi(s);
    }
    catch (invalid_argument &e){
        myInt = -1;
    }
    return myInt;
}

//for  (int k = 0; k < gameBoard.columns; k++) {
//                for (int j = 0; j < gameBoard.rows; j++) {
//
//                }
//                }

//    vector<vector<vector<sf::Sprite>>> allSprites;
//
//    vector<vector<sf::Sprite>> hiddentileSprites = (FillWithSprites(h, gameBoard));
//    allSprites.push_back(hiddentileSprites); // hiddenTiles index = 0
//
//    vector<vector<sf::Sprite>> revealedtileSprites = (FillWithSprites(r, gameBoard));
//    allSprites.push_back(revealedtileSprites); // revealedTiles = 1
//
//    vector<vector<sf::Sprite>> flagSprites = (FillWithSprites(flag, gameBoard));
//    allSprites.push_back(flagSprites); // flags = 2
//
//    vector<vector<sf::Sprite>> mineSprites = (FillWithSprites(mine, gameBoard));
//    allSprites.push_back(mineSprites); // mines = 3
//