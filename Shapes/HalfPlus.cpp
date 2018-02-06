//
//  HalfPlus.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "HalfPlus.hpp"

#define POS0 0 //Position 0: base
#define POS1 1 //Position 1: Rotate right 1 or left 3
#define POS2 2 //Position 2: Rotate right 2 or left 2
#define POS3 3 //Position 3: Rotate right 3 or left 1

HalfPlus::HalfPlus(int colPair) : Shape(colPair) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get memory to store offset's
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Shape's cstr will set orientation, so just need to compute the offsets
    computeNewOffsets();
}

HalfPlus::HalfPlus(int colPair, int orginY, int orginX) : Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Shape's cstr will set orientation, so just need to compute the offsets
    computeNewOffsets();
}

HalfPlus::HalfPlus(int colPair, int orginY, int orginX, int startingOrientation)
: Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    switch (startingOrientation % 4) {
        case POS0:
            break;
        case POS2:
            rotateRight();
            //Break; //Don't break here, since need to rotate again
        case POS1:
            rotateRight();
            break;
        case POS3:
            rotateLeft();
            break;
    }
    computeNewOffsets();
}

HalfPlus::~HalfPlus() {
    delete [] xCoorOffsets;
    delete [] yCoorOffsets;
}

int HalfPlus::getHeight() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 2;
    }
    else {
        return 3;
    }
}

int HalfPlus::getWidth() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 3;
    }
    else {
        return 2;
    }
}

//I really can just write rotateLeft and rotateRight in shape since it will be same each time

void HalfPlus::rotateLeft() {
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

void HalfPlus::rotateRight() {
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


void HalfPlus::translateOrgin() {
    //See header file for how orgin and POS# are related
    //8 cases to handle
    if (previousOrientation == POS0) {
        if (orientation == POS1) {
            this->yOrgin -= 1;
        }
        else { //we have POS3
            this->yOrgin -= 1;
        }
    }
    else if (previousOrientation == POS1) {
        if (orientation == POS0) {
            this->yOrgin += 1;
        }
        else { //we have POS2
            //this->yOrgin -= 0;
        }
    }
    else if (previousOrientation == POS2) {
        if (orientation == POS3) {
            //this->xOrgin -= 0;
        }
        else {// we have POS1
            //this->yOrgin += 0;
        }
    }
    else if (previousOrientation == POS3) {
        if (orientation == POS0) {
            this->yOrgin += 1;
        }
        else { //we have POS2
            //this->xOrgin += 0;
        }
    }
    //Now that the orgin has been moved to its new location, let's compute the new
    //offsets
    computeNewOffsets();
}


void HalfPlus::computeNewOffsets() {
    //See header file for my offset numbering scheme
    //Four cases to compute offsets for
    if (POS0 == orientation) {
        //Left of orgin
        xCoorOffsets[0] = -1;
        yCoorOffsets[0] = 0;
        //Above orgin
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = -1;
        //right of orgin
        xCoorOffsets[2] = 1;
        yCoorOffsets[2] = 0;
    }
    else if (POS1 == orientation) {
        //above orgin
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = -1;
        //Right of orgin
        xCoorOffsets[1] = 1;
        yCoorOffsets[1] = 0;
        //below orgin
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = 1;
    }
    else if (POS2 == orientation) {
        
        xCoorOffsets[0] = 1;
        yCoorOffsets[0] = 0;
        
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = 1;
        
        xCoorOffsets[2] = -1;
        yCoorOffsets[2] = 0;
    }
    else if (POS3 == orientation) {
        //
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = -1;
        
        xCoorOffsets[1] = -1;
        yCoorOffsets[1] = 0;
        
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = 1;
    }
}
