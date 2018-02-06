//
//  SBlock.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef SBlock_hpp
#define SBlock_hpp


#include "Shape.hpp"

class SBlock : public Shape {
    //private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    SBlock(int colPair);
    SBlock(int colPair, int orginY, int orginX);
    SBlock(int colPair, int orginY, int orginX, int startingOrientation);

    virtual ~SBlock() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining an InvLShape with its offsets:
// (the numbers represent array positioning of offsets)
//
// POS0:                +--------+--------+
//                      |        |        |
//                      |    1   |    2   |
//                      |        |        |
//             +--------+--------+--------+
//             |        |        |
//             |    0   |  orgin |
//             |        |        |
//             +--------+--------+
//
//
//
// POS1:            +--------+
//                  |        |
//                  |    0   |
//                  |        |
//                  +--------+--------+
//                  |        |        |
//                  |  orgin |    1   |
//                  |        |        |
//                  +--------+--------+
//                           |        |
//                           |    2   |
//                           |        |
//                           +--------+
//
//
//
// POS2:                +--------+--------+
//                      |        |        |
//                      |    1   |    2   |
//                      |        |        |
//             +--------+--------+--------+
//             |        |        |
//             |    0   |  orgin |
//             |        |        |
//             +--------+--------+
//
//
// POS3:      
//                  +--------+
//                  |        |
//                  |    0   |
//                  |        |
//                  +--------+--------+
//                  |        |        |
//                  |  orgin |    1   |
//                  |        |        |
//                  +--------+--------+
//                           |        |
//                           |    2   |
//                           |        |
//                           +--------+
//



#endif /* SBlock_hpp */
