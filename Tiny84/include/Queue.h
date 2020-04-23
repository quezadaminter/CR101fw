/*
 * Queue.h
 *
 * Created: 8/10/2018 10:41:26
 *  Author: MQUEZADA
 */ 


#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdlib.h>
#include <stdint.h>

// define default capacity of the queue
#define SIZE 10

class Node
{
   public:
      Node *inFront = NULL;
      Node *behind = NULL;
      void *data = NULL;
};

// Class for queue
class queue
{
   uint16_t capacity;	// maximum capacity of the queue
   Node *front;		// front points to front element in the queue (if any)
   Node *rear;		// rear points to last element in the queue
   uint16_t count;		// current size of the queue

   public:
   queue(uint16_t size = SIZE);		// constructor

   void *pop();
   void push(void *x);
   void *peek();
   uint16_t size();
   bool isEmpty();
   bool isFull();
};


#endif /* QUEUE_H_ */