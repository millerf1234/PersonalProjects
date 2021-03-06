//
//  StraightShape.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "StraightShape.hpp"

#define POS0 0 //Position 0: base
#define POS1 1 //Position 1: Rotate right 1 or left 3
#define POS2 2 //Position 2: Rotate right 2 or left 2
#define POS3 3 //Position 3: Rotate right 3 or left 1

StraightShape::StraightShape(int colPair) : Shape(colPair) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get memory to store offset's
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    //Shape's constr will set a starting orientation, so just need
    //to compute the offsets
    //To prevent pushing left to cause shape to rotate right upon spawning in postion 2, switch position 2 to position 0
    if (this->orientation == POS2) {
        this->orientation = POS0;
        this->previousOrientation = POS0;
    }
    computeNewOffsets();
}

StraightShape::StraightShape(int colPair, int orginY, int orginX) : Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    
    //To prevent pushing left to cause shape to rotate right upon spawning in postion 2, switch position 2 to position 0
    if (this->orientation == POS2) {
        this->orientation = POS0;
        this->previousOrientation = POS0;
    }
    
    //Shape's constructor will set orientation, so just need to compute the offsets
    computeNewOffsets();
}

StraightShape::StraightShape(int colPair, int orginY, int orginX, int startingOrientation)
: Shape(colPair, orginY, orginX) {
    this->size = 4; //Apparently all tetris shapes are size 4
    //Get dat memory for dem ints fam
    xCoorOffsets = new int[3];
    yCoorOffsets = new int[3];
    switch (startingOrientation % 4) {
        case POS0:
        case POS2: //To have POS2 be same as POS0 do this line
        break;
        //To have the straight shape spawn in an upsidedown orientation so that hitting "rotate right" cause a left rotation, use next three lines:
        // case POS2:
        //      rotateRight();
        //     //Break; //Don't break here, since need to rotate again
        case POS1:
            rotateRight();
            break;
        case POS3:
            rotateLeft();
            break;
    }
    computeNewOffsets();
}

StraightShape::~StraightShape() {
    delete [] xCoorOffsets;
    delete [] yCoorOffsets;
}

int StraightShape::getHeight() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 4;
    }
    else {
        return 1;
    }
}

int StraightShape::getWidth() { //See header
    if (POS0 == this->orientation || POS2 == this->orientation) {
        return 1;
    }
    else {
        return 4;
    }
}

//I really could just write rotateLeft and rotateRight in Shape since it
//will be same for each child object (i.e. not have it be virtual).
//I'm going to leave it like this for now until I decide how I will handle
//rotations for shapes at the edge of the screen

void StraightShape::rotateLeft() {
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

void StraightShape::rotateRight() {
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


void StraightShape::translateOrgin(){
    //    //See header file for how orgin and POS# are related
    //    //8 cases to handle
    if (previousOrientation == POS0) {
        if (orientation == POS1) {
            //Translation from POS0 to POS1 (right turn)
            //Do Nothing?
        }
        else { //we have POS3
            //Translation from POS0 to POS3 (left turn)
           //Do Nothing?
        }
    }
    else if (previousOrientation == POS1) {
        if (orientation == POS2) {
            //Translation from POS1 to POS2 (right turn)
            //Do Nothing?
        }
        else { //we have POS0
            //Translation from POS1 to POS0 (left turn)
            //Do Nothing?
        }
    }
    else if (previousOrientation == POS2) {
        if (orientation == POS3) {
            //Translation from POS2 to POS3 (right turn)
            //this->yOrgin += 1;
            //Do Nothing?
        }
        else {// we have POS1
            //Translation from POS2 to POS1 (left turn)
            //Do Nothing?
        }
    }
    else if (previousOrientation == POS3) {
        if (orientation == POS0) {
            //Translation from POS3 to POS0   (right turn)
            //Do Nothing?
        }
        else { //we have POS2
            //Translation from POS3 to POS2   (left turn)
            //this->yOrgin -= 1;
            //Do Nothing?
        }
    }
    //Now that the orgin has been moved to its new location, let's compute the new
    //offsets
    computeNewOffsets();
}

void StraightShape::computeNewOffsets() {
    //    //See header file for my offset numbering scheme
    //    //Four cases to compute offsets for
    if (POS0 == orientation) {
        //Offset 0
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = 1;
        //Offset 1
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = -1;
        //Offset 2
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = -2;
    }
    else if (POS1 == orientation) {
        //Offset 0
        xCoorOffsets[0] = -1;
        yCoorOffsets[0] = 0;
        //Offset 1
        xCoorOffsets[1] = 1;
        yCoorOffsets[1] = 0;
        //Offset 2
        xCoorOffsets[2] = 2;
        yCoorOffsets[2] = 0;
    }
    else if (POS2 == orientation) {
        //Offset 0
        xCoorOffsets[0] = 0;
        yCoorOffsets[0] = 1; //Down
        //Offset 1
        xCoorOffsets[1] = 0;
        yCoorOffsets[1] = -1; //Up
        //Offset 2
        xCoorOffsets[2] = 0;
        yCoorOffsets[2] = -2; //Up 
    }
    else if (POS3 == orientation) {
        //Offset 0
        xCoorOffsets[0] = 1;
        yCoorOffsets[0] = 0;
        //Offset 1
        xCoorOffsets[1] = -1;
        yCoorOffsets[1] = 0;
        //Offset 2
        xCoorOffsets[2] = -2;
        yCoorOffsets[2] = 0;
    }
}
