#ifndef LOGIC_OTHER_H
#define LOGIC_OTHER_H


#include "deus.h"

/**
 * @brief Loads the tile, given its "focal point", to the game matrix.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadTileIntoGrid(ProgramParameters* parameters);

/**
 * @brief Unloads the tile from the game matrix.
 * 
 * @param grid game matrix
 * @param tile tile 
 */
void unloadTileFromGrid(int** grid, const Tile* tile);


#endif