#ifndef LOGIC_OTHER_H
#define LOGIC_OTHER_H


#include "deus.h"

/**
 * @brief Loads the tile, given its "focal point", to the game matrix.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadTileIntoGrid(int** tetrisGrid, Tile* tile);

/**
 * @brief Frees previous tile, loads a new one.
 * 
 * @param renderer renderer
 * @param tile pointer to current tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t getNewTile(SDL_Renderer* renderer, Tile* tile, const int flags, FILE* debug);

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

/**
 * @brief 
 * 
 * @param tetrisGrid 
 * @param tetrisGridSize size of the game matrix
 * @param score pointer to score variable, meant to be incremenented based on how many rows were cleared 
 */
void onPlacement(int** tetrisGrid, const Size tetrisGridSize, int* score);

/**
 * @brief Function invoked when an end-of-game is detected (can't load next tile into the game matrix, etc.)
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void onGameEnd(ProgramParameters* parameters);

void onGameStart(ProgramParameters* parameters, SDL_Renderer* renderer);


#endif