#ifndef QUEUE_H
#define QUEUE_H


#include "structs_unions.h"

/**
 * @brief Creates a queue.
 * 
 * @return pointer to Queue struct, NULL on error
 */
Queue* initQueue();

/**
 * @brief Adds element to queue.
 * 
 * @param queue queue
 * @param data pointer to data
 * @return SUCCESS on success, MEMORY_FAILURE on memory allocation error
 */
status_t enqueue(Queue* queue, void* data);

/**
 * @brief Removes first element from queue and returns pointer to its data.
 * 
 * @param queue queue
 * @return pointer to dequeued element's data, NULL on empty queue
 */
void* dequeue(Queue* queue);

/**
 * @brief Removes first element from queue and puts its data in `out`.
 * 
 * @param queue queue
 * @param out variable to store element's pointer to data
 */
char dequeueAlt(Queue* queue, void* out);

/**
 * @brief Prints pointers to elements queued. Not really useful.
 * 
 * @param queue queue
 */
void printQueue(Queue* queue);

/**
 * @brief Frees all elements and their data from the queue.
 * 
 * @param queue queue
 */
void freeQueue(Queue* queue);


#endif