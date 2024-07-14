#ifndef TILES_H
#define TILES_H


#include <stdio.h>
#include <SDL.h>

#include "deus.h"

extern const char shapeNames[SHAPE_AMOUNT][8];

extern const char colorNames[COLOR_AMOUNT][8];

/**
 * @brief Loads a tile of given color, shape and coordinates. If `coordinates` is NULL, the position is `(0, 0)`.
 * When the tile is not needed anymore, call `freeTile(tile)` to free it, otherwise you'll get a memory leak.
 * 
 * @param renderer SDL renderer
 * @param color color of the tile
 * @param shape shape of the tile
 * @param coordinates pointer to Point struct of the tile
 * @param gridWidth width of the game matrix
 * @param flags flags for loading the tile, please refer to the TileLoadingFlags enum
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTile(SDL_Renderer* renderer, TileColor color, TileShape shape, Point* coordinates, const int gridWidth, const int flags, FILE* debug);

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
 * @param gridWidth width of the game matrix
 * @param flags flags for loading the tile, please refer to the TileLoadingFlags enum
 * @param debug debug stream, optional
 * @return pointer to Tile struct, NULL on error
 */
Tile* loadTileRandom(SDL_Renderer* renderer, Point* coordinates, const int gridWidth, const int flags, FILE* debug);

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


#endif