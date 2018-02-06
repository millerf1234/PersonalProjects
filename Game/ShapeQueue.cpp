//
//  ShapeQueue.cpp
//  Tetris_Using_NCurses
//
//  Created by Forrest Miller on 12/12/17.
//  Copyright © 2017 Forrest Miller. All rights reserved.
//

#include "ShapeQueue.hpp"

ShapeQueue::~ShapeQueue() {
    //printw("ShapeQueue Destructor called yo!\n"); //Debug line
    while (this->head != nullptr) {
        ShapeElem * temp = this->head->next;
        delete this->head;
        this->head = temp;
    }
}

int ShapeQueue::add(Shape * shape) {
    ShapeElem * QueueIter; //An iterator to iterate through the queue
    this->size += 1; //Increase the queue size by 1
    if (this->head == nullptr) { //If queue's head is null
        this->head = new ShapeElem;
        this->head->shape = shape;
        this->head->next = nullptr;
    }
    else { //Queue's head is not null, then need to traverse to end of queue
        QueueIter = this->head;
        while (QueueIter->next != nullptr) { //Iterate through queue till we reach the end
            QueueIter = QueueIter->next;
        }
        QueueIter->next = new ShapeElem; //Add a new shape to the end of the queue
        //Move QueueIter to the next shape (note this step is just to make initialization of new shape easier)
        QueueIter = QueueIter->next;
        //So now that QueueIter is set at the newly added shape, we need to set up the newly added shape
        QueueIter->shape = shape;
        QueueIter->next = nullptr;
    }
    return this->size; 
}

int ShapeQueue::getSize() {
    return this->size;
}

Shape * ShapeQueue::pop() {
    if (0 == this->size || nullptr == this->head) { //If queue is empty, then can't pop
        throw new std::bad_function_call; //Gonna just use a built in exception for now
    }
    this->size -= 1;
    Shape * temp = this->head->shape;
    //Check to see if there is anything past head, if not then make head null
    if (this->head->next == nullptr) {
        delete this->head;
        this->head = nullptr;
    }
    else { //Otherwise move the head to the next shape in the queue
        //Need a temp to temporarily hold the next shapeElem in the queue
        ShapeElem * tempShapeElem = this->head->next;
        //Need to delete the ShapeElem that is being removed from the queue
        delete this->head;
        this->head = tempShapeElem;
    }
    return temp;
}

Shape * ShapeQueue::peek() {
    //if (0 == this->size) { //More efficient, less safe
    if (0 == this->size || nullptr == this->head) {
        return nullptr;
    }
    return this->head->shape;
}

Shape * ShapeQueue::peekOnDeck() {
    //if (1 >= this->size) { //More efficent, less safe 
    if (1 >= this->size || nullptr == this->head || nullptr == this->head->next) {
        return nullptr;
    }
    return this->head->next->shape;
}

void ShapeQueue::operator+(Shape * s) {
    add(s); //Just call my dedicated 'add' function
}

int ShapeQueue::addToFront(Shape * shape) { //For God Mode
    this->size += 1;
    if (nullptr == this->head) {
        this->head = new ShapeElem;
        this->head->shape = shape;
        this->head->next = nullptr;
    }
    else { //Queue's head is not null
        ShapeElem * temp = new ShapeElem;
        temp->shape = shape;
        temp->next = this->head;
        this->head = temp;
    }
    return this->size;
}
