//
//  ShapeQueue.hpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//
#include "Shape.hpp"
#include <ncurses.h>
#include <exception>
#include <functional>
#ifndef ShapeQueue_hpp
#define ShapeQueue_hpp

//This struct is a wrapper for shapes within the Queue
typedef struct ShapeElem {
    Shape * shape;
    ShapeElem * next;
} ShapeElem;

//This is the actual ShapeQueue
class ShapeQueue {
private:
    ShapeElem * head;
    int size;
public:
    //Constructor:
    ShapeQueue() {this->head = nullptr; this->size = 0;}
    //Destructor:
    ~ShapeQueue();
    //Member Functions:
    int add(Shape *); //i.e. push_back
    int addToFront(Shape *); //Skips the queue to add shape to the front
    int getSize(); //get the number of shapes stored within the queue
    Shape * pop(); //Remove from the head of the queue
    Shape * peek(); //Look at the shape at the head of the queue
    Shape * peekOnDeck(); //Look at the shape after the shape at the head of the queue
    void operator+(Shape *);
    
    //Shape * peekAt(int posInQueue);
};

#endif /* ShapeQueue_hpp */
