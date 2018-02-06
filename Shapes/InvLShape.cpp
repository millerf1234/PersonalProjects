//
//  InvLShape.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "InvLShape.hpp"

#define POS0 0 //Position 0: base
#define POS1 1 //Position 1: Rotate right 1 or left 3
#define POS2 2 //Position 2: Rotate right 2 or left 2
#define POS3 3 //Position 3: Rotate right 3 or left 1

InvLShape::InvLShape(int colPair) : Shape(colPair) {
    this->size = 4; //Apparently all tetris shapes are size 4
    
    //To make this shapes rotation line up better with LShape
    this->xOrgin += 1; //Not sure if I want this or not
    
    //Get memory to store offset's
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Shape's constr will set a starting orientation, so just need
    //to compute the offsets
    computeNewOffsets();
}

InvLShape::InvLShape(int colPair, int orginY, int orginX) : Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    
    //To make this shapes rotation line up better with LShape
    this->xOrgin += 1; //Not sure if I want this or not
    
    
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Shape's cstr will set orientation, so just need to compute the offsets
    computeNewOffsets();
}

InvLShape::InvLShape(int colPair, int orginY, int orginX, int startingOrientation)
: Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //To make this shapes rotation line up better with LShape
    this->xOrgin += 1; //Not sure if I want this or not
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Set to correct starting orientation
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

InvLShape::~InvLShape() {
    delete [] xCoorOffsets;
    delete [] yCoorOffsets;
}

int InvLShape::getHeight() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 3;
    }
    else {
        return 2;
    }
}

int InvLShape::getWidth() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 2;
    }
    else {
        return 3;
    }
}

//I really could just write rotateLeft and rotateRight in Shape since it
//will be same for each child object (i.e. not have it be virtual).
//I'm going to leave it like this for now until I decide how I will handle
//rotations for shapes at the edge of the screen

void InvLShape::rotateLeft() {
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

void InvLShape::rotateRight() {
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


void InvLShape::translateOrgin(){
    //    //See header file for how orgin and POS# are related
    //    //8 cases to handle
    if (previousOrientation == POS0) { //If we were in POS0
        if (orientation == POS1) { //And are now in POS1
            //Compute Translation from POS0 to POS1
            this->xOrgin -= 1;
            this->yOrgin += 1;
        }
        else { //we went from POS0 to POS3, so compute the
            //Translation from POS0 to POS3
            this->xOrgin -= 1;
        }
    }
    else if (previousOrientation == POS1) { //If we were in POS1
        if (orientation == POS2) { //And are now in POS2
            //Compute Translation from POS1 to POS2
            this->yOrgin -= 1;
        }
        else { //we went from POS1 to POS0, so compute the
            //Translation from POS1 to POS0
            this->xOrgin += 1;
            this->yOrgin -= 1;
        }
    }
    else if (previousOrientation == POS2) { //If we were in POS2
        if (orientation == POS3) { //And are now in POS3
            //Compute Translation from POS2 to POS3
            //this->xOrgin += 0; //(the orgin doesn't move)
        }
        else { //we went from POS2 to POS1, so compute the
            //Translation from POS2 to POS1
            this->yOrgin += 1;
        }
    }
    else if (previousOrientation == POS3) { //If we were in POS3
        if (orientation == POS0) { //And are now in POS0
            //Compute Translation from POS3 to POS0
            this->xOrgin += 1;
        }
        else { //We must have rotated left to POS2, so compute the
            //Translation from POS3 to POS2
            //this->xOrgin += 0; //(The orgin doesn't move)
        }
    }
    //Now that the orgin has been moved to its new location, let's compute the new
    //offsets
    computeNewOffsets();
}

void InvLShape::computeNewOffsets() {
    //    //See header file for my offset numbering scheme
    //    //Four cases to compute offsets for
    if (POS0 == orientation) {
        //Offset 0
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = -1;
        //Offset 1
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = 1;
        //Offset 2
        xCoorOffsets[2] = -1;
        yCoorOffsets[2] = 1;
    }
    else if (POS1 == orientation) {
        
        xCoorOffsets[0] = 1;
        yCoorOffsets[0] = 0;
        
        xCoorOffsets[1] = -1;
        yCoorOffsets[1] = 0;
        
        xCoorOffsets[2] = -1;
        yCoorOffsets[2] = -1;
    }
    else if (POS2 == orientation) {
        
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = 1;
        
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = -1;
        
        xCoorOffsets[2] =  1;
        yCoorOffsets[2] = -1;
    }
    else if (POS3 == orientation) {
        
        xCoorOffsets[0] = -1;
        yCoorOffsets[0] = 0;
        
        xCoorOffsets[1] = 1;
        yCoorOffsets[1] = 0;
        
        xCoorOffsets[2] = 1;
        yCoorOffsets[2] = 1;
    }
}
