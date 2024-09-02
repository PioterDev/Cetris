#ifndef LOGIC_MOVEMENT_H
#define LOGIC_MOVEMENT_H


#include "deus.h"

/**
 * @brief Moves the tile one square left.
 * 
 * @param grid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure, INDEXOUTOFRANGE on position outside of the grid
 */
status_t moveLeft(int** grid, Tile* tile);

/**
 * @brief Moves the tile one square right.
 * 
 * @param grid game matrix
 * @param tile tile
 * @param gridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure, INDEXOUTOFRANGE on position outside of the grid
 */
status_t moveRight(int** grid, Tile* tile, const int gridWidth);

/**
 * @brief Moves the tile one square down.
 * 
 * @param grid game matrix
 * @param tile tile
 * @param gridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure, INDEXOUTOFRANGE on position outside of the grid
 */
status_t moveDown(int** grid, Tile* tile, const int gridHeight);

/**
 * @brief Immediately drop a tile.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void dropHard(ProgramParameters* parameters);

/**
 * @brief Hard drop functionality. Uses moveDown.
 * 
 * @param grid game matrix
 * @param tile tile
 * @return status 
 */
//status_t dropHard(char** grid, Tile* tile);

/**
 * @brief Hard drop functionality.
 * 
 * @param grid game matrix
 * @param tile tile
 * @param gridHeight height of the game matrix 
 * @return SUCCESS on success, FAILURE on failure
 */


#endif