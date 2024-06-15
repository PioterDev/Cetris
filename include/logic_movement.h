#ifndef LOGIC_MOVEMENT_H
#define LOGIC_MOVEMENT_H


#include "deus.h"

/**
 * @brief Moves the tile one square left.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveLeft(int** tetrisGrid, Tile* tile);

/**
 * @brief Moves the tile one square right.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveRight(int** tetrisGrid, Tile* tile, const int tetrisGridWidth);

/**
 * @brief Moves the tile one square down.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t moveDown(int** tetrisGrid, Tile* tile, const int tetrisGridHeight);

/**
 * @brief Immediately drop a tile.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void dropHard(ProgramParameters* parameters);

/**
 * @brief Hard drop functionality. Uses moveDown.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return status 
 */
//status_t dropHard(char** tetrisGrid, Tile* tile);

/**
 * @brief Hard drop functionality.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix 
 * @return SUCCESS on success, FAILURE on failure
 */
//status_t dropHardOld(char** tetrisGrid, Tile* tile, const int tetrisGridHeight, FILE* debug);


#endif