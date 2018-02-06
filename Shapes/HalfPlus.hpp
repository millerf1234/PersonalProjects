//
//  HalfPlus.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#ifndef HalfPlus_hpp
#define HalfPlus_hpp

#include "Shape.hpp"

class HalfPlus : public Shape {
//private:
protected:
    virtual void translateOrgin() override ;
    virtual void computeNewOffsets() override;
public:
    HalfPlus(int colPair);
    HalfPlus(int colPair, int orginY, int orginX);
    HalfPlus(int colPair, int orginY, int orginX, int startingOrientation);

    virtual ~HalfPlus() override;
    virtual int getHeight() override;
    virtual int getWidth() override;
    virtual void rotateLeft() override;
    virtual void rotateRight() override;
};

// Note that this is how I am defining a HalfPlus with its offsets:
// (the numbers represent array positioning of offsets)
//
// POS0:               -----
//                    |  1  |
//              -------------------
//              |  0  |orgin|  2  |
//              -------------------
//
//
//
// POS1:            -----
//                 |     |
//                 |  0  |
//                 |     |
//                 |-----|-----|
//                 |     |     |
//                 |orgin|  1  |
//                 |     |     |
//                 |-----|-----|
//                 |     |
//                 |  2  |
//                 |     |
//                  -----
//
//
// POS2:
//              -------------------
//              |  2  |orgin|  0  |
//              -------------------
//                    |  1  |
//                     -----
//
//
// POS3:                  -----
//                       |     |
//                       |  0  |
//                       |     |
//                 |-----|-----|
//                 |     |     |
//                 |  1  |orgin|
//                 |     |     |
//                 |-----|-----|
//                       |     |
//                       |  2  |
//                       |     |
//                        -----

#endif /* HalfPlus_hpp */
