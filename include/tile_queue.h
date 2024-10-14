#ifndef TILE_QUEUE_H
#define TILE_QUEUE_H


#include "deus.h"

/**
 * @brief Adds a given tile to the queue.
 * 
 * @param queue queue struct
 * @param tile tile to enqueue
 * @return SUCCESS on success, FAILURE on full queue
 */
status_t enqueueTile(TileQueue* queue, Tile* tile);

/**
 * @brief Removes the first queued tile from the queue and puts its address into a given pointer.
 * 
 * @param queue queue struct
 * @param toStore pointer to the variable where the Tile* will be stored
 * @return SUCCESS on success, FAILURE on empty queue
 */
status_t dequeueTile(TileQueue* queue, Tile** toStore);

/**
 * @brief Frees all queued elements from the queue.
 * 
 * @param queue queue struct
 */
void flushTileQueue(TileQueue* queue);

/**
 * @brief Prints the size of the queue and every queued element to the given stream.
 * 
 * @param queue queue struct
 */
void printTileQueue(TileQueue* queue);


#endif