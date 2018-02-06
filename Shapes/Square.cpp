//
//  Square.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "Square.hpp"

#define POS0 0 //Position 0: base
#define POS1 1 //Position 1: Rotate right 1 or left 3
#define POS2 2 //Position 2: Rotate right 2 or left 2
#define POS3 3 //Position 3: Rotate right 3 or left 1


Square::Square(int colPair) : Shape(colPair) {
    this->size = 4; //2x2 square
    //Shape should set orientation to 0
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    computeNewOffsets();
}

Square::Square(int colPair, int orginY, int orginX) : Shape(colPair, orginY, orginX) {
    //Shape constructor should set orientation to 0
    this->size = 4;
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    computeNewOffsets();
}
Square::Square(int colPair, int orginY, int orginX, int startingOrientation)
: Shape(colPair, orginY, orginX) {
    //Shape constructor should set orientation to 0
    this->size = 4;
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    switch (startingOrientation % 4) {
        case POS0:
            break;
        case POS2:
            rotateRight();
            //(no) Break; //Don't break here, since need to rotate again
        case POS1:
            rotateRight();
            break;
        case POS3:
            rotateLeft();
            break;
    }
    computeNewOffsets();
}

Square::~Square() {
    //From the heap you hath come, to the heap you shall return
    delete [] xCoorOffsets;
    delete [] yCoorOffsets;
}

void Square::rotateLeft() {
    //Logic to perform here:
    //   1) Set previousOrientation to orientation when this method is called
    //   2) Increment orientation to the left by 1 (make orientation wrap around)
    //   3) Compute any translating of the orgin
    this->previousOrientation = this->orientation;
    this->orientation -= 1; //Left decreases it by 1
    if (this->orientation < POS0) { //Make POS0 go to POS3
        this->orientation = POS3;
    }
    translateOrgin();
}

void Square::rotateRight() {
    //Logic to perform here:
    //   1) Set previousOrientation to orientation when this method is called
    //   2) Increment orientation to the right by 1 (make orientation wrap around)
    //   3) Compute any translating of the orgin
    this->previousOrientation = this->orientation;
    this->orientation += 1; //Right increases it by 1
    if (this->orientation > POS3) { //Make POS3 go to POS0
        this->orientation = POS0;
    }
    translateOrgin();
}

void Square::translateOrgin() { //See header file for how orgin and POS# are related
    //8 cases to handle
    if (previousOrientation == POS0) {
        if (orientation == POS1) {
            this->xOrgin += 1;
        }
        else { //we have POS3
            this->yOrgin -= 1;
        }
    }
    else if (previousOrientation == POS1) {
        if (orientation == POS0) {
            this->xOrgin -= 1;
        }
        else { //we have POS2
            this->yOrgin -= 1;
        }
    }
    else if (previousOrientation == POS2) {
        if (orientation == POS3) {
            this->xOrgin -= 1;
        }
        else {// we have POS1
            this->yOrgin += 1;
        }
    }
    else if (previousOrientation == POS3) {
        if (orientation == POS0) {
            this->yOrgin += 1;
        }
        else { //we have POS2
            this->xOrgin += 1;
        }
    }
    //Now that the orgin has been moved to its new location, let's compute the new
    //offsets
    computeNewOffsets();
}

void Square::computeNewOffsets() {//See header file for my offset numbering scheme
    //Four cases to compute offsets for
    if (POS0 == orientation) { //Orgin is top left corner of square
        //Top right corner coor
        xCoorOffsets[0] = 1;
        yCoorOffsets[0] = 0;
        //Bottom right corner coor
        xCoorOffsets[1] = 1;
        yCoorOffsets[1] = -1;
        //Bottom left corner coor
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = -1;
    }
    else if (POS1 == orientation) { //Orgin is top right corner of square
        //Bottom right corner
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = -1;
        //Bottom left corner
        xCoorOffsets[1] = -1;
        yCoorOffsets[1] = -1;
        //Top Right Corner
        xCoorOffsets[2] = -1;
        yCoorOffsets[2] = 0;
    }
    else if (POS2 == orientation) { //Orgin is bottom right corner of square
        //Bottom left corner
        xCoorOffsets[0] = -1;
        yCoorOffsets[0] = 0;
        //Top Left corner
        xCoorOffsets[1] = -1;
        yCoorOffsets[1] = 1;
        //Top Right corner
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = 1;
    }
    else if (POS3 == orientation) { //Orgin is bottom left corner of square
        //Top left corner
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = 1;
        //Top Right corner
        xCoorOffsets[1] = 1;
        yCoorOffsets[1] = 1;
        //Bottom Right corner
        xCoorOffsets[2] = 1;
        yCoorOffsets[2] = 0;
    }
}
