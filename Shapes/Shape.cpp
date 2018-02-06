//
//  Shape.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "Shape.hpp"

#define POS0 0 //Position 0: base
#define POS1 1 //Position 1: Rotate right 1 or left 3
#define POS2 2 //Position 2: Rotate right 2 or left 2
#define POS3 3 //Position 3: Rotate right 3 or left 1

Shape::Shape(int colPair) {
    this->colorPair = colPair;
    this->xOrgin = 0;
    this->yOrgin = 0;
    this->orientation = POS0; //Might want to make this random later on
    this->previousOrientation = POS0;
}

Shape::Shape(int colPair, int orginY, int orginX) {
    this->colorPair = colPair;
    this->xOrgin = orginX;
    this->yOrgin = orginY;
    this->orientation = POS0; //Might want to make this random later on
    this->previousOrientation = POS0;
}

void Shape::setOrgin(int y, int x) {
    this->xOrgin = x;
    this->yOrgin = y;
}

void Shape::moveFromGravity(int amount) {
    //What should happen
   // this->yOrgin += amount;
    //However, I am going to fix a bug I am experienceing by hacking this to work by doing
    this->yOrgin += 1;
}

void Shape::moveShapeUpOne() {
    this->yOrgin -= 1; //Useful for if an overlapping collision occurs
}
//Shape::Shape(int size, int colPair) {
//    this->size = size;
//    this->colorPair = colPair;
//}

