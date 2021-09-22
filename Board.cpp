#include "Board.h"
#include "Random.h"

// TILES[][]!!!!
//// REVERSAL! position is (x, y) COL, ROW!!!! but tiles is stored as [y][x]
//// only use (x,y) when drawing to Window


Board::Board() {
    columns = 0;
    rows = 0;

}
Board::Board(int col, int row, int mineCount) {
    this->columns = col;
    this->rows = row;
    this->mineCount = mineCount;
    windowW = col * 32;
    windowH = (row * 32) + 88;
    tileCount = row * col;
    flagsRemaining = mineCount;  // use to track digits.png


    // initialize tiles
    for  (int i = 0; i < rows; i++) {
        vector<Tile> rowofTiles;
        for (int j = 0; j < columns; j++) {
            Tile t(j, i); // default state 0
            rowofTiles.push_back(t);
 }
        tiles.push_back(rowofTiles);
 }
    // [y][x] access [row],[col]
    //place mines and set neighbors
    placeMines();
    Neighbors();

}

int Board::GetWindowW() const {
    return windowW;

}
int Board::GetWindowH() const {
    return windowH;
}


void Board::placeMines()
{
    //tiles
    int m = 0;
    while (m < mineCount) {

        int randomX = Random::Int(0, columns-1);
        int randomY = Random::Int(0, rows-1);

        if (!tiles[randomY][randomX].isaMine()) { // if this tile is a NOT a mine
            tiles[randomY][randomX].SetMine(); // need to set mines in actual board.
            m++;
        } else
            continue;
    }
}


int Board::GetCol () const {
    return columns;
} int Board::GetRows () const {
    return rows;
}
vector<vector<Tile>>& Board::GetTileGrid() {
    return tiles;

}
void Board:: SetTileGrid(vector<vector<Tile>> &t) {
    tiles = t;
}

int Board::GetMineCount() const {
    return mineCount;
}


void Board::Neighbors() {
// in main: iterate through tile vectorvector, if (!tile.isaMine()), tile.SetNeighbors(board.CountNeighborMines(x,y)); x y is pos of tile

    for  (int i = 0; i < rows; i++) { //accessing tiles
        for (int j = 0; j < columns; j++) {
            if (!(tiles[i][j].isaMine())) {
                tiles[i][j].SetNeighbors(CountNeighborMines(j, i));
//                cout << tiles[i][j].GetNeighbors() << " ";
            }
        }}
    // NOTE!!! COUNTNEIGHBORS CRASHED A WHOLE BUNCH BECAUSE I FORGOT ABOUT TILES[Y][X]

}


bool Board::CheckWin() {
    //cout << "Checking for win..." << endl;
    int nonMineCt = 0;
    int revealedTileCt = 0;

    for  (int i = 0; i < rows; i++) { //accessing tiles
        for (int j = 0; j < columns; j++) {
            if (!(tiles[i][j].isaMine())) {
                nonMineCt++;
            }

            if (tiles[i][j].GetState() == 2) {
                revealedTileCt++;
            }

        }}
    //cout << "1. non mines: " << nonMineCt << " 2. revealed tiles:" << revealedTileCt << endl;
    if (nonMineCt == revealedTileCt) { // WIN!
        cout << endl;
        cout << "Win!! :)";
        for  (int i = 0; i < rows; i++) { //accessing tiles
            for (int j = 0; j < columns; j++) {
                if (tiles[i][j].isaMine()) {
                tiles[i][j].SetState(1); // set flags to remaining mines.
                }
            }}
        flagsRemaining = 0;
        return true;
    }

    else return false;

}

void Board::RecursiveReveal(int x, int y) { // reveals all non-mine adjacent spaces!

    if (tiles[y][x].isaMine() || (tiles[y][x].GetState() != 0)) { // if tile is a mine or if it is flag/already revealed (not hidden) ... STOP
     return;
 }
    else {
            // if we make it to this part of the function, then tiles[x][y] must have valid index AND no neighbors, nonmines, nonflags
            // can start recursive calls

        tiles[y][x].SetState(2); // string 2 = "revealed";
        if (tiles[y][x].GetNeighbors() > 0) {
            return;
        } else {

            if (y != 0) {
                if (x != 0) {
                    RecursiveReveal(x - 1, y - 1);
                }
                RecursiveReveal(x, y - 1);

                if (x < columns - 1) {
                    RecursiveReveal(x + 1, y - 1);
                }
            }
            if (x != 0) {
                RecursiveReveal(x - 1, y);
            }
            if (x < columns - 1) {
                RecursiveReveal(x + 1, y);
            }

            if (y < rows - 1) {
                if (x != 0) {
                    RecursiveReveal(x - 1, y + 1);
                }
                RecursiveReveal(x, y + 1);

                if (x < columns - 1) {
                    RecursiveReveal(x + 1, y + 1);
                }
            }
        }
    }

}


int Board::CountNeighborMines(int &x, int &y) {
    int mineCt = 0;
//    cout << "call Neighborcount ";
//[x-1, y-1][x, y-1][x+1, y-1]
//[x-1, y][x, y][x+1, y]
//[x-1, y+1][x, y+1][x+1, y+1]

//tiles[y][x]

// x < col
// y < row
// edge cases
//if (x != 0)
//if (x != columns - 1)
//if (y != 0)
//if (y != rows - 1)

//cout << x << " " << y << endl;

    if  (x != 0) {
        if (y != 0) {
            if (tiles[y - 1][x - 1].isaMine()) {
                mineCt++;
            }
    }

    if (tiles[y][x - 1].isaMine()) {
        mineCt++;
    }
        if (y < rows - 1) {
    if (tiles[y + 1][x - 1].isaMine()) {
        mineCt++;
    }
    }
}
    if (y != 0) {
        if (tiles[y - 1][x].isaMine()) {
            mineCt++;
        }
    }
    if (y < rows - 1) {
        if (tiles[y + 1][x].isaMine()) {
            mineCt++;
        }
    }


    if (x < columns - 1) {
        if (y != 0) {
            if (tiles[y - 1][x + 1].isaMine()) {
                mineCt++;
            }
        }
        if (tiles[y][x + 1].isaMine()) {
            mineCt++;
        }
        if (y < rows - 1) {
            if (tiles[y + 1][x + 1].isaMine()) {
                mineCt++;
            }
        }
    }

   return mineCt;
};





