//
//  Square.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//
#include "Shape.hpp"
#ifndef Square_hpp
#define Square_hpp

class Square : public Shape {
//private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    Square(int colPair);
    Square(int colPair, int orginY, int orginX);
    Square(int colPair, int orginY, int orginX, int startingOrientation);
    
    virtual ~Square() override;
   
    virtual int getHeight() override { return 2;}
    virtual int getWidth() override {return 2;}
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
    
};

// Note that this is how I am defining a square with its offsets:
// (the numbers represent array positioning of offsets)
//
// 
//
// POS0:                 |
//                 Orgin |   0
//                       |
//                ----------------
//                       |
//                   2   |   1
//                       |
//
//
//
// POS1:                 |
//                   2   | Orgin
//                       |
//                ----------------
//                       |
//                   1   |   0
//                       |
//
//
//
// POS2:                 |
//                   1   |   2
//                       |
//                ----------------
//                       |
//                   0   | Orgin
//                       |
//
//
//
// POS3:                 |
//                   0   |   1
//                       |
//                ----------------
//                       |
//                 Orgin |   2
//                       |
//

#endif /* Square_hpp */
