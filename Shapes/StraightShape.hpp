//
//  StraightShape.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef StraightShape_hpp
#define StraightShape_hpp

#include "Shape.hpp"

class StraightShape : public Shape {
    //private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    StraightShape(int colPair);
    StraightShape(int colPair, int orginY, int orginX);
    StraightShape(int colPair, int orginY, int orginX, int startingOrientation);
    
    virtual ~StraightShape() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining an InvLShape with its offsets:
// (the numbers represent array positioning of offsets)
//
// POS0:                 +-----+
//                       |     |
//                       |  2  |
//                       |     |
//                       +-----+
//                       |     |
//                       |  1  |
//                       |     |
//                       +-----+
//                       |     |
//                       |orgin|
//                       |     |
//                       +-----+
//                       |     |
//                       |  0  |
//                       |     |
//                       +-----+
//
//
// POS1:
//
//             +-------+-------+-------+------+
//             |       |       |       |      |
//             |   0   | orgin |   1   |   2  |
//             |       |       |       |      |
//             +-------+-------+-------+------+
//
//
//
//
// POS2:                +-----+
//                      |     |
//                      |  2  |
//                      |     |
//                      +-----+
//                      |     |
//                      |  1  |
//                      |     |
//                      +-----+
//                      |     |
//                      |orgin|
//                      |     |
//                      +-----+
//                      |     |
//                      |  0  |
//                      |     |
//                      +-----+
//
//
// POS3:
//
//         +--------+-------+-------+-------+
//         |        |       |       |       |
//         |    2   |   1   | orgin |   0   |
//         |        |       |       |       |
//         +--------+-------+-------+-------+






#endif /* StraightShape_hpp */
