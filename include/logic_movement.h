#ifndef LOGIC_MOVEMENT_H
#define LOGIC_MOVEMENT_H


#include "structs_unions_defines.h"

/**
 * @brief Moves the tile one square left.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveLeft(char** tetrisGrid, Tile* tile);

/**
 * @brief Moves the tile one square right.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveRight(char** tetrisGrid, Tile* tile, const int tetrisGridWidth);

/**
 * @brief Moves the tile one square down.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t moveDown(char** tetrisGrid, Tile* tile, const int tetrisGridHeight);

/**
 * @brief 
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridSize Size struct for the matrix size
 */
void dropHard(char** tetrisGrid, Tile* tile, const Size tetrisGridSize);


#endif