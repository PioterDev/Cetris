#ifndef QUEUE_H
#define QUEUE_H


#include <SDL.h>

#include "structs_unions_defines.h"

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
status_t enqueue(Queue* queue, SDL_Event event);

/**
 * @brief Removes first element from queue and sets passed event to dequeued event.
 * 
 * @param queue queue
 * @return 0 if empty queue, 1 if not
 */
char dequeue(Queue* queue, SDL_Event* event);

/**
 * @brief Frees all elements and their data from the queue.
 * 
 * @param queue queue
 */
void freeQueue(Queue* queue);


#endif