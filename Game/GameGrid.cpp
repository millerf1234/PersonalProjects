//
//  GameGrid.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "GameGrid.hpp"

#define NO_FULL_ROWS -1
#define NUM_OFFSETS 3

//For Reference (copied from the .h file):
////Each coordinate on the screen will be stored within the follow struct
//typedef struct GridElem {
//    //Fields (i.e. variables)
//    int xCoor;
//    int yCoor;
//    int colorPair;
//
//    //Default constructor
//    GridElem() = delete; //Can't be called
//    GridElem(int colPair, int y, int x) {
//        this->xCoor = x;
//        this->yCoor = y;
//        this->colorPair = colPair;
//    }
//    ~GridElem() {
//        ;
//    }
//} GirdElem;

//---------------------------------------------------------------------------
// IMPORTANT NOTE!!!     NOTE THAT grid is indexed as grid[row][col], not the other way around
//---------------------------------------------------------------------------


//Constructor
GameGrid::GameGrid(int height, int width, int topLeftCornerOffsetY, int topLeftCornerOffsetX) {
    if (height <= 0 || width <= 0) {
        throw new std::bad_function_call; //Gonna just use a built in exception for now    }
    }
    //Set fields
    this->rows = height;
    this->cols = width;
    this->topLeftCornerYOffset = topLeftCornerOffsetY + 1;
    this->topLeftCornerXOffset = topLeftCornerOffsetX + 1;
    //Create the game grid by
    // Create 2D array of pointers to GridElem:
    this->grid = new GridElem ** [height];
    for (int i = 0; i < this->rows; ++i) {
        grid[i] = new GridElem * [width];
    }
    
    // Null out the pointers contained in the array:
    for (int i = 0; i < this->cols; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j] = nullptr;
        }
    }
   // this->topColumnIsOccupied = false; //Start top column out as being unoccupied (unused)
}

GameGrid::~GameGrid() {
    for (int i = 0; i < this->rows; ++i ) {
        for (int j = 0; j < this->cols; ++j) {
            if (grid[i][j] != nullptr) {
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
        delete [] grid[i];
    }
    delete[] grid;
}


//Public member functions:

int GameGrid::getColorAt(int y, int x) {
    if (this->grid[y][x] == nullptr) {
        return 0;
    }
    else {
        return this->grid[y][x]->colorPair;
    }
}

bool GameGrid::hasGridElemAt(int y, int x) {
    if (this->grid[y][x] == nullptr) {
        return false;
    }
    //else
    return true;
}

//Checks to see if the row is complete
bool GameGrid::checkIfRowComplete(int row) {
    bool isComplete = true;
    for (int j = 0; j < this->cols; ++j) {
        if (this->grid[row][j] == nullptr) {
            isComplete = false;
            break;
        }
    }
    return isComplete;
}

int GameGrid::checkForCompleteRows(void) { //Return -1 if no row is complete
    //returns the first complete row it finds if there are multiple complete rows
    int completeRow = NO_FULL_ROWS; //Default it to value for NO_FULL_ROWS
    for (int j = 0; j < this->rows; ++j) {
        if (checkIfRowComplete(j)) {
            completeRow = j;
            break;
        }
    }
    if (completeRow != NO_FULL_ROWS) {
        return completeRow;
    }
    //else
    return NO_FULL_ROWS;
}

//Removes the row from the gameGrid
void GameGrid::removeRow(int row) {
    for (int i = 0; i < this->cols; ++i) {
        if (this->grid[row][i] == nullptr) {
            throw new std::bad_function_call;
        }
        delete this->grid[row][i];
        this->grid[row][i] = nullptr;
    }
    
    bool somethingFell = makeBlocksFall(row);
    while(somethingFell) {
        somethingFell = makeBlocksFall(row); //Keep making blocks fall until nothing falls
    }
    
    //Buggy way of copying grid to a new one while moving all rows above deleted on down (This was before I added makeBlocksfall() )
    //
    //    //Need to make a new grid that is a copy of this grid
    //    GridElem *** temp = new GridElem ** [this->rows];
    //    for (int i = 0; i < this->rows; ++i) {
    //        temp[i] = new GridElem * [this->cols];
    //    }
    //
    //    // Null out the pointers contained in the array:
    //    for (int i = 0; i < this->cols; ++i) {
    //        for (int j = 0; j < this->rows; ++j) {
    //            grid[i][j] = nullptr;
    //        }
    //    }
    //
    //    //Now to copy the original grid over to temp, but with moving any rows that fell
    //    //from gravity down 1
    //    for (int i = 0; i < this->rows - 1; i++) {
    //        if (i < row) {
    //            for (int j = 0; j < this->cols; j++) {
    //                temp[i + 1][j] = this->grid[i] [j];
    //            }
    //        }
    //        else {
    //            for (int j = 0; j < this->cols; j++) {
    //                temp[i][j] = this->grid[i+1] [j];
    //            }
    //        }
    //    }
}

bool GameGrid::checkForGridCollosion(Shape * s) {
    int x,y;
    bool wasCollision = false; //Will become true if a collision is immanent
    //First need to get the components of each of the 4 pieces of the active 'shape'
    int xCoors[4]; //Tetris shapes will always be 4 in size
    int yCoors[4];
    xCoors[0] = s->getOrginX() - this->topLeftCornerXOffset;
    yCoors[0] = s->getOrginY() - this->topLeftCornerYOffset;
    for (int i = 0; i < 3; ++i) {
        xCoors[i+1] = xCoors[0] + s->xCoorOffsets[i];
        yCoors[i+1] = yCoors[0] + s->yCoorOffsets[i];
    }
    
    //Now that the 4 blocks are stored within the arrays
    //Check for current collisions on each of the 4 coordinates
    for (int i = 0; i < 4; ++i) {
        x = xCoors[i];
        y = yCoors[i];
        //If there is a current collision
        if (y >= 0) {
            if (this->grid[y][x] != nullptr) { //Oops then this shape will be on top of a shape
                s->moveShapeUpOne();
                break;
                //return checkForGridCollosion(s);
            }
        }
    }
    
    //So if no direct collisions are detected, need to check for potential collisions
    for (int i = 0; i < 4; i++) {
        x = xCoors[i];
        y = yCoors[i];
        if (y >= 0) {
            //Need to make sure my checking stays within the gameGrid
            if (y >= this->rows - 1) {
                continue;
                //return false;
            }
            if (this->grid[y+1][x] != nullptr) { //Check to see if the spot directly below the shape is occupied
                return true;
            }
        }
    }
    return wasCollision;
}

////This function can check to see if moving a shape will result in a collision
////Maybe make this function a friend of shape?
//void GameGrid::hasBlockBelowCoordinate(int x, int y) {
//
//}
//void GameGrid::hasBlockBelowCoordinate(int x, int y, int velocity) { //In case velocity isn't set at 1
//
//}

bool GameGrid::addShapeToGrid(Shape * s) { //Use s->xOffset and s->yOffset to create GirdElem's. Also use hasBlockBelowCoordinate in main
    bool shapeAboveGrid_GameOver = false; //If the shape is going to be above the grid, then game over
    int colorPair = s->getColorPair();
    int x,y, tempx, tempy;
    //Add the orgin
    x = s->getOrginX() - this->topLeftCornerXOffset;
    y = s->getOrginY() - this->topLeftCornerYOffset;
    //Check for gameOver conidition on orgin
    if (y < 0) {
        shapeAboveGrid_GameOver = true;
        return shapeAboveGrid_GameOver;
    }
    //prevent crashing by only evaluating the case if x and y remain within the array bounds
    if (x >= 0 && x < this->cols && y >= 0 && y < this->rows) {
        if (this->grid[y][x] != nullptr) {delete this->grid[y][x];} //This line prevents potential memory leaks
        this->grid[y][x] = new GridElem(colorPair, y, x);
    }
    //Add the offsets
    for (int i = 0; i < NUM_OFFSETS; i++) {
        //Check for gameOver condition on each offset
        if (y < 0) {
            shapeAboveGrid_GameOver = true;
            return shapeAboveGrid_GameOver; //Game over 
        }
        tempx = (x + s->xCoorOffsets[i]);
        tempy = (y + s->yCoorOffsets[i]);
        //prevent crashing
        if (tempx >= 0 && tempx < this->cols && tempy >= 0 && tempy < this->rows) {
            if (this->grid[tempy][tempx] != nullptr) {delete this->grid[tempy][tempx];} //This line prevents potential memory leaks
            this->grid[tempy][tempx] = new GridElem(colorPair, y, x);
        }
    }
    
    //I check for completed rows within my main instead
    //int row = checkForCompleteRows(); //See if adding block completed any rows
    //if (row != NO_FULL_ROWS) {
    //   while (row != NO_FULL_ROWS) {
    //      removeRow(row);
    //      row = checkForCompleteRows();
    //  }
    // }
    
    return shapeAboveGrid_GameOver;
}

//Warning! Debug test will cause program to crash now that all the game logic has
// been implemented.
//void GameGrid::setUpDebugTest()  {
//    for (int i = 0; i < this->cols; ++i) {
//        for (int j = 0; j < this->rows; ++j) {
//            //this->grid[j][i] = new GirdElem( rand() % 7, j, i);
//            this->grid[j][i] = new GridElem(1, j, i);
//        }
//    }
//}

void GameGrid::clear() {
    for (int i = 0; i < this->rows; ++i ) {
        for (int j = 0; j < this->cols; ++j) {
            if (grid[i][j] != nullptr) {
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

bool GameGrid::hasGridElemLeftOf(Shape * s) {
    int x,y;
    bool wasCollision = false; //Will become true if a collision is immanent
    //First need to get the components of each of the 4 pieces of 'shape'
    int xCoors[4]; //Tetris shapes will always be 4 in size
    int yCoors[4];
    xCoors[0] = s->getOrginX() - this->topLeftCornerXOffset;
    yCoors[0] = s->getOrginY() - this->topLeftCornerYOffset;
    for (int i = 0; i < 3; i++) {
        xCoors[i+1] = xCoors[0] + s->xCoorOffsets[i];
        yCoors[i+1] = yCoors[0] + s->yCoorOffsets[i];
    }
    //Check each of the four coordinate pairs for each subblock of shape s to see if anything is to the left of them
    for (int i = 0; i < s->getSize(); ++i) {
        //Set x and y to the coordinates of the subblock
        x = xCoors[i];
        y = yCoors[i];
        if (y < 0) {continue;} //Don't need to perform check on blocks outside the grid
        if (0 == x) {
            continue; //Don't need to check if block is against left wall
        }
        //Check to see what is to the left of these coordinates
        if (this->grid[y][x-1] != nullptr) {
            wasCollision = true;
            return wasCollision; //This might be slightly faster (and possibly less readable)
            //break;
        }
    }
    
    return wasCollision;
}

bool GameGrid::hasGridElemRightOf(Shape * s) {
    int x,y;
    bool wasCollision = false; //Will become true if a collision is immanent
    //First need to get the components of each of the 4 pieces of 'shape'
    int xCoors[s->getSize()]; //Tetris shapes will always be 4 in size
    int yCoors[s->getSize()];
    xCoors[0] = s->getOrginX() - this->topLeftCornerXOffset;
    yCoors[0] = s->getOrginY() - this->topLeftCornerYOffset;
    for (int i = 0; i < (s->getSize() - 1); i++) {
        xCoors[i+1] = xCoors[0] + s->xCoorOffsets[i];
        yCoors[i+1] = yCoors[0] + s->yCoorOffsets[i];
    }
    
    //Check each of the four coordinate pairs for each subblock of shape s to see if anything is to the right of them
    for (int i = 0; i < s->getSize(); i++) {
        //Set x and y to the coordinates of the subblock
        x = xCoors[i];
        y = yCoors[i];
        
        if (y < 0) { continue;} //Don't need to perform check on blocks outside the grid
        if (this->cols == x) {
            continue; //Don't need to check if block is against right wall
        }
        //Check to see what is to the right of these coordinates
        if (this->grid[y][x+1] != nullptr) {
            wasCollision = true;
            return wasCollision; //This might be slightly faster (and possibly less readable)
            //break;
        }
    }
    return wasCollision;
}

//Private member function(s):

bool GameGrid::makeBlocksFall(int row) {
    bool somethingFell = false;
    //For each grid elem, check to see if there is anything below it, if not,
    //then have it fall
    //for (int j = 0; j < this->rows - 1; j++) { //This works but makes everything on grid fall
    for (int j = 0; j < row; j++) { //This may work, needs to be tested
        for (int i = 0; i < this->cols; i++) {
            if (this->grid[j][i] != nullptr) { //So if a grid element exists at these coordinates
                //check to see if anything is below this grid elem
                if (this->grid[j+1][i] == nullptr) { //If nothing is below it
                    somethingFell = true;
                    this->grid[j+1][i] = this->grid[j][i]; //Then move it down one
                    //and empty the spot in the grid where it just was
                    this->grid[j][i] = nullptr;
                }
            }
        }
    }
    return somethingFell;
}
