#ifndef TILES_H
#define TILES_H


#include <stdio.h>
#include <SDL.h>

#include "deus.h"

extern const int defaultStates[STATE_AMOUNT];

extern const char shapeNames[SHAPE_AMOUNT][8];

extern const char colorNames[COLOR_AMOUNT][12];

/**
 * @brief Sets the default tile parameters based on its shape.
 * 
 * @param tile tile
 * @param gridWidth game matrix's width 
 * @return SUCCESS on success, FAILURE on unhandled shape
 */
status_t setDefaultTileParameters(Tile* tile, const int gridWidth);

/**
 * @brief Loads a tile of given color, shape and coordinates. If `coordinates` is NULL, the position is `(0, 0)`.
 * When the tile is not needed anymore, call `freeTile(tile)` to free it, otherwise you'll get a memory leak.
 * 
 * @param color color of the tile
 * @param shape shape of the tile
 * @param coordinates pointer to Point struct of the tile
 * @param gridWidth width of the game matrix
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTile(TileColor color, TileShape shape, Point* coordinates, const int gridWidth);

/**
 * @brief Frees the given tile from memory.
 * 
 * @param tile pointer to Tile struct
 */
void freeTile(Tile* tile);

/**
 * @brief Loads a random tile. Look up `loadTile`.
 * 
 * @param coordinates pointer to Point struct of the tile
 * @param gridWidth width of the game matrix
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTileRandom(Point* coordinates, const int gridWidth);

/**
 * @brief Prints information about given tile to the defualt stream.
 * 
 * @param tile tile
 * @param stream stream
 */
void printTile(Tile* tile);


#endif