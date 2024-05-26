#ifndef LOGIC_ROTATION_H
#define LOGIC_ROTATION_H


#include "structs_unions_defines.h"

/**
 * @brief Rotates piece clockwise.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t rotateClockwise(int** tetrisGrid, Tile* tile);


/**
 * @brief Rotates piece counter-clockwise.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t rotateCounterClockwise(int** tetrisGrid, Tile* tile);


#endif