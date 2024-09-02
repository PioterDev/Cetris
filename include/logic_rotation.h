#ifndef LOGIC_ROTATION_H
#define LOGIC_ROTATION_H


#include "deus.h"

/**
 * @brief Rotates piece clockwise.
 * 
 * @param grid game matrix
 * @param tile tile
 * @param gridSize size of the game matrix
 * @return SUCCESS on success, FAILURE on failure, INDEXOUTOFRANGE on position outside of the grid
 */
status_t rotateClockwise(int** grid, Tile* tile, const Size gridSize);


/**
 * @brief Rotates piece counter-clockwise.
 * 
 * @param grid game matrix
 * @param tile tile
 * @param gridSize size of the game matrix
 * @return SUCCESS on success, FAILURE on failure, INDEXOUTOFRANGE on position outside of the grid
 */
status_t rotateCounterClockwise(int** grid, Tile* tile, const Size gridSize);


#endif