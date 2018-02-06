//
//  GameGrid.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef GameGrid_hpp
#define GameGrid_hpp

#include <exception>
#include <functional>
#include "Shape.hpp"

//Each coordinate on the screen will be stored within the follow struct
typedef struct GridElem {
    //Fields (i.e. variables)
    int xCoor;
    int yCoor;
    int colorPair;
    
    //Default constructor
    GridElem() = delete; //Can't be called
    GridElem(int colPair, int y, int x) {
        this->xCoor = x;
        this->yCoor = y;
        this->colorPair = colPair;
    }
    ~GridElem() {
        ; //Nothing to remove from heap memory (i.e. no memory needs deallocating)
    }
} GirdElem;

//This class is for tracking shapes on the screen
class GameGrid {
private:
    //Fields
    //The game grid
    GridElem ***grid; //2D grid of GridElem pointers
    int rows;
    int cols;
    int topLeftCornerXOffset;
    int topLeftCornerYOffset; //Where the game grid exists on the screen
    bool topColumnIsOccupied; //Signals the game may soon be about to end
    
    //Private helper funcions
    //bool checkIfTopColumnIsOccupied(); //Never used
    bool makeBlocksFall(int row);
    
public:
    GameGrid(int height, int width, int topLeftCornerOffsetY, int topLeftCornerOffsetX);
    ~GameGrid(); //Destructor
    int getColorAt(int y, int x);
    bool hasGridElemAt(int y, int x);
    //These next 2 functions should be called before trying to move a block left/right
    bool hasGridElemLeftOf(Shape * s);
    bool hasGridElemRightOf(Shape * s);
    bool checkForGridCollosion(Shape * s); //Checks both at and 1 space below each component of shape s
    
    //One of the two should be implemented
    bool checkIfRowComplete(int row);
    int checkForCompleteRows(void); //Return -1 if no row is complete
    
    void removeRow(int row);
    
    //This function can check to seee if moving a shape will result in a collision
    //Maybe make this function a friend of shape?
    //void hasBlockBelowCoordinate(int x, int y);
    //void hasBlockBelowCoordinate(int x, int y, int velocity); //In case velocity isn't one
    
    
    bool addShapeToGrid(Shape * s); //Use s->xOffset and s->yOffset to create GirdElem's. Also use hasBlockBelowCoordinate in main
    void clear();
    
    // DEBUG TEST: completly fills grid with grid elements. see warning:
    //void setUpDebugTest(); //Warning!!! Don't use. Will make game crash now that full game
    //                         logic is in place. 
    
};

#endif /* GameGrid_hpp */
