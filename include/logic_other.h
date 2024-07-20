#ifndef LOGIC_OTHER_H
#define LOGIC_OTHER_H


#include "deus.h"

/**
 * @brief Loads the tile, given its "focal point", to the game matrix.
 * 
 * @param grid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadTileIntoGrid(int** grid, const Tile* tile, const Size gridSize);


#endif