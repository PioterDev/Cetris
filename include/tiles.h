#ifndef TILES_H
#define TILES_H


#include <stdio.h>
#include <SDL.h>

#include "deus.h"

/**
 * @brief Loads a tile of given color, shape and coordinates. If `coordinates` is NULL, the position is `(0, 0)`.
 * When the tile is not needed anymore, call `freeTile(tile)` to free it, otherwise you'll get a memory leak.
 * 
 * @param renderer SDL renderer
 * @param color color of the tile
 * @param shape shape of the tile
 * @param coordinates pointer to Point struct of the tile
 * @param flags flags for loading the tile, please refer to the TileLoadingFlags enum
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTile(SDL_Renderer* renderer, TileColor color, TileShape shape, Point* coordinates, const int flags, FILE* debug);

/**
 * @brief Frees the given tile from memory.
 * 
 * @param tile pointer to Tile struct
 */
void freeTile(Tile* tile);

/**
 * @brief Loads a random tile. Look up `loadTile`.
 * 
 * @param renderer renderer
 * @param coordinates pointer to Point struct of the tile
 * @param flags flags for loading the tile, please refer to the TileLoadingFlags enum
 * @param debug debug stream, optional
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTileRandom(SDL_Renderer* renderer, Point* coordinates, const int flags, FILE* debug);

void centerTileHorizontally(Tile* tile, ProgramParameters* programParameters);

void centerTileVertically(Tile* tile, ProgramParameters* programParameters);

void centerTile(Tile* tile, ProgramParameters* programParameters);

/**
 * @brief Prints information about given tile to the given stream.
 * 
 * @param tile tile
 * @param stream stream
 */
void printTile(Tile* tile, FILE* stream);

/**
 * @brief Creates a TileQueue struct.
 * 
 * @return pointer to TileQueue struct, NULL on allocation error.
 */
TileQueue* createTileQueue();

/**
 * @brief Adds a given tile to the queue.
 * 
 * @param queue queue struct
 * @param tile tile to enqueue
 * @return SUCCESS on success, MEMORY_FAILURE on memory failure
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
 * @brief Frees all queued elements from the queue and the queue itself.
 * 
 * @param queue queue struct
 */
void freeTileQueue(TileQueue* queue);

/**
 * @brief Prints the size of the queue and every queued element to the given stream.
 * 
 * @param queue queue struct
 * @param stream stream
 */
void printTileQueue(TileQueue* queue, FILE* stream);


#endif