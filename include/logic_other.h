#ifndef LOGIC_OTHER_H
#define LOGIC_OTHER_H


#include "structs_unions_defines.h"

/**
 * @brief Loads the tile, given its "focal point", to the game matrix.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadTileIntoGrid(char** tetrisGrid, Tile* tile);

/**
 * @brief Frees previous tile, loads a new one.
 * 
 * @param renderer renderer
 * @param tile pointer to current tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t getNewTile(SDL_Renderer* renderer, Tile* tile, FILE* debug);

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