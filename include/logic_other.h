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

/**
 * @brief Pauses or unpauses the game.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void togglePause(ProgramParameters* parameters);

/**
 * @brief 
 * 
 * @param grid 
 * @param gridSize size of the game matrix
 * @param score pointer to score variable, meant to be incremenented based on how many rows were cleared 
 */
void onPlacement(ProgramParameters* parameters);

/**
 * @brief Function to be invoked when an end-of-game event is detected 
 * (can't load next tile into the game matrix, the program received an exit signal, etc.).
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param reason reason as to why the game ended
 */
void onGameEnd(ProgramParameters* parameters, GameEndReason reason);

/**
 * @brief Function to be invoked when the game starts.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param renderer pointer to SDL2's renderer
 * @return SUCCESS on starting the game, FAILURE on failure
 */
status_t onGameStart(ProgramParameters* parameters, SDL_Renderer* renderer);


#endif