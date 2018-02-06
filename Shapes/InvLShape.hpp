//
//  InvLShape.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef InvLShape_hpp
#define InvLShape_hpp

#include "Shape.hpp"

class InvLShape : public Shape {
    //private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    InvLShape(int colPair);
    InvLShape(int colPair, int orginY, int orginX);
    InvLShape(int colPair, int orginY, int orginX, int startingOrientation);

    virtual ~InvLShape() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining an InvLShape with its offsets:
// (the numbers represent array positioning of offsets)
//
// POS0:                      +-------+
//                            |       |
//                            |   0   |
//                            |       |
//                            +-------+
//                            |       |
//                            | orgin |
//                            |       |
//                    +-------+-------+
//                    |       |       |
//                    |   2   |   1   |
//                    |       |       |
//                    +-------+-------+
//
//
// POS1:
//
//             +-------+
//             |       |
//             |   2   |
//             |       |
//             +-------+-------+-------+
//             |       |       |       |
//             |   1   | orgin |   0   |
//             |       |       |       |
//             +-------+-------+-------+
//
//
//
// POS2:
//
//                    +-------+-------+
//                    |       |       |
//                    |   1   |   2   |
//                    |       |       |
//                    +-------+-------+
//                    |       |
//                    | orgin |
//                    |       |
//                    +-------+
//                    |       |
//                    |   0   |
//                    |       |
//                    +-------+
//
//
// POS3:
//             +-------+-------+-------+
//             |       |       |       |
//             |   0   | orgin |   1   |
//             |       |       |       |
//             +-------+-------+-------+
//                             |       |
//                             |   2   |
//                             |       |
//                             +-------+
//





#endif /* InvLShape_hpp */
