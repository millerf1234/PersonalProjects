//
//  ZBlock.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/13/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef ZBlock_hpp
#define ZBlock_hpp


#include "Shape.hpp"

class ZBlock : public Shape {
    //private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    ZBlock(int colPair);
    ZBlock(int colPair, int orginY, int orginX);
    ZBlock(int colPair, int orginY, int orginX, int startingOrientation);

    virtual ~ZBlock() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining an InvLShape with its offsets:
// (the numbers represent array positioning of offsets)
//
// POS0:       +--------+--------+
//             |        |        |
//             |    2   |    1   |
//             |        |        |
//             +--------+--------+--------+
//                      |        |        |
//                      |  orgin |    0   |
//                      |        |        |
//                      +--------+--------+
//
//
//
// POS1:
//                           +--------+
//                           |        |
//                           |    2   |
//                           |        |
//                  +--------+--------+
//                  |        |        |
//                  |  orgin |    1   |
//                  |        |        |
//                  +--------+--------+
//                  |        |
//                  |    0   |
//                  |        |
//                  +--------+
//
//
//
// POS2:       +--------+--------+
//             |        |        |
//             |    2   |    1   |
//             |        |        |
//             +--------+--------+--------+
//                      |        |        |
//                      |  orgin |    0   |
//                      |        |        |
//                      +--------+--------+
//
//
// POS3:
//                           +--------+
//                           |        |
//                           |    2   |
//                           |        |
//                  +--------+--------+
//                  |        |        |
//                  |  orgin |    1   |
//                  |        |        |
//                  +--------+--------+
//                  |        |
//                  |    0   |
//                  |        |
//                  +--------+
//
//


#endif /* ZBlock_hpp */
