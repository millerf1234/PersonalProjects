//
//  LShape.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef LShape_hpp
#define LShape_hpp

#include "Shape.hpp"

class LShape : public Shape {
    //private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    LShape(int colPair); //Constructs object with orgin at 0,0
    LShape(int colPair, int orginY, int orginX);
    LShape(int colPair, int orginY, int orginX, int startingOrientation);

    virtual ~LShape() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining an LShape with its offsets:
// (the numbers represent array positioning of offsets)
// (the arrows represent how the orgin is translated between positions)
//
// POS0:              (TO POS3)
//                        ^
//                        |      (How orgin is translated)
//                        |
//                      y + 1
//
//                 +-----+
//                 |     |
//                 |  0  |
//                 |     |
//                 +-----+
//                 |     |
//                 |orgin|
//                 |     |
//                 +-----+-----+
//                 |     |     |
//                 |  1  |  2  |
//                 |     |     |
//                 +-----+-----+
//
//                      x + 0      (Old way x + 1)
//                        |
//                        |
//                        v
//
//
// POS1:
//                        ^
//                        |
//                        |
//                      x - 0     (Old way  x - 1)
//
//             +-------+-------+-------+
//             |       |       |       |
//             |   1   | orgin |   0   |
//             |       |       |       |
//             +-------+-------+-------+
//             |       |
//             |   2   |
//             |       |
//             +-------+
//
//                    No Change
//                        |
//                        |
//                        v
//
//
// POS2:
//                        ^
//                        |
//                        |
//                    No Change
//
//                 +-----+-----+
//                 |     |     |
//                 |  2  |  1  |
//                 |     |     |
//                 +-----+-----+
//                       |     |
//                       |orgin|
//                       |     |
//                       +-----+
//                       |     |
//                       |  0  |
//                       |     |
//                       +-----+
//
//                      y + 1
//                      x - 1
//                        |
//                        |
//                        v
//
//
// POS3:
//                        ^
//                        |
//                        |
//                      x + 1
//                      y - 1
//                              +-------+
//                              |       |
//                              |   2   |
//                              |       |
//              +-------+-------+-------+
//              |       |       |       |
//              |   0   | orgin |   1   |
//              |       |       |       |
//              +-------+-------+-------+
//
//                       y - 1
//                         |
//                         |
//                         v
//                     (To POS0)
//

#endif /* LShape_hpp */
