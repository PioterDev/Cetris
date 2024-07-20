#ifndef EVENTS_H
#define EVENTS_H


#include "deus.h"

/**
 * @brief Function to be invoked when the game starts.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param renderer pointer to SDL2's renderer
 * @return SUCCESS on starting the game, FAILURE on failure
 */
status_t onGameStart(ProgramParameters* parameters);

/**
 * @brief Function to be invoked when an end-of-game event is detected 
 * (can't load next tile into the game matrix, the program received an exit signal, etc.).
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param reason reason as to why the game ended
 */
void onGameEnd(ProgramParameters* parameters, GameEndReason reason);


/**
 * @brief Function to be invoked when a tile is placed.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void onPlacement(ProgramParameters* parameters);

/**
 * @brief Function to be invoked when the window is resized.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @return status_t 
 */
status_t onWindowResize(ProgramParameters* parameters, SDL_Window* window);


#endif